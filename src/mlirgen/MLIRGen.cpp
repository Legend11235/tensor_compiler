#include "tc/mlirgen/MLIRGen.h"
#include "tc/dialect/TensorOps.h"
#include "mlir/IR/Builders.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/Diagnostics.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include <map>
#include <string>
#include <vector>
#include <algorithm>

// helper to combine multiple lambdas into one overloaded callable for
// std::visit lets each AST node type get its own lambda dispatched
// automatically
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace tc {

class MLIRGenImpl {
public:
  MLIRGenImpl(mlir::MLIRContext &context) : builder(&context) {}

  mlir::ModuleOp mlirGen(const Expr &topLevelExpr) {
    module = mlir::ModuleOp::create(builder.getUnknownLoc());
    builder.setInsertionPointToEnd(module.getBody());

    std::vector<std::string> varNames;
    collectVarNames(topLevelExpr, varNames);

    auto tensorType = mlir::UnrankedTensorType::get(builder.getF32Type()); // get type of tensor
    std::vector<mlir::Type> argTypes(varNames.size(), tensorType); // makes vector of size of varNames and make it the same type
    auto funcType = builder.getFunctionType(argTypes, tensorType); // builds actual function signature type

    // create the func op
    auto funcOp = mlir::func::FuncOp::create(builder.getUnknownLoc(), "main", funcType); // 'main' is just a place holder default name
    module.push_back(funcOp); // declares funcOp as a top level op inside the module. funcOp is just the 'main' we just defined
    mlir::Block &entryBlock = *funcOp.addEntryBlock(); // every function needs one block
    builder.setInsertionPointToStart(&entryBlock); // builders cursor moves to function body

    // populate symbolTable
    for (size_t i = 0; i < varNames.size(); i++) {
        symbolTable[varNames[i]] = entryBlock.getArgument(static_cast<unsigned>(i));
    }

    mlir::Value result = mlirGenExpr(topLevelExpr);
    if (!result) {
    return module;
    }

    if (result.getType() != tensorType) {
        result = builder.create<mlir::tensor::CastOp>(loc(topLevelExpr.loc), tensorType, result);
    }

    builder.create<mlir::func::ReturnOp>(loc(topLevelExpr.loc), result);

    return module;

  }

private:
  mlir::OpBuilder builder;
  mlir::ModuleOp module;
  std::map<std::string, mlir::Value> symbolTable;

  mlir::Location loc(const Location &astLoc) {
    return mlir::FileLineColLoc::get(builder.getContext(), astLoc.file,
                                     static_cast<unsigned>(astLoc.line),
                                     static_cast<unsigned>(astLoc.col));
  }

  // record all variable nodes and skip duplicates
  void collectVarNames(const Expr &expr, std::vector<std::string> &names) {
    std::visit(
        overloaded{
            [&](const VarNode &n) {
              if (std::find(names.begin(), names.end(), n.name) ==
                  names.end()) {
                names.push_back(n.name);
              }
            },
            [&](const MatmulNode &n) {
              collectVarNames(*n.left, names);
              collectVarNames(*n.right, names);
            },
            [&](const AddNode &n) {
              collectVarNames(*n.left, names);
              collectVarNames(*n.right, names);
            },
            [&](const ReluNode &n) { collectVarNames(*n.tensor, names); },
            [&](const TransposeNode &n) { collectVarNames(*n.tensor, names); },
            [&](const ReshapeNode &n) { collectVarNames(*n.tensor, names); },
        },
        expr.node);
  }


  // recursive worker MlirGen
  mlir::Value mlirGenExpr(const Expr &expr) {
    return std::visit(
        overloaded{
            [&](const VarNode &n) -> mlir::Value {
                auto x = symbolTable.find(n.name);
                if (x == symbolTable.end()){
                    emitError(loc(expr.loc)) << "unknown variable '" << n.name << "'";
                    return nullptr;
                }
                return x -> second;
            },
            [&](const MatmulNode &n) -> mlir::Value {
                mlir::Value lhs = mlirGenExpr(*n.left);
                if (!lhs) return nullptr;

                mlir::Value rhs = mlirGenExpr(*n.right);
                if (!rhs) return nullptr;

                auto ranked2D = mlir::RankedTensorType::get(
                    {mlir::ShapedType::kDynamic, mlir::ShapedType::kDynamic}, builder.getF32Type());

                mlir::Value lhsCast = builder.create<mlir::tensor::CastOp>(loc(expr.loc), ranked2D, lhs);
                mlir::Value rhsCast = builder.create<mlir::tensor::CastOp>(loc(expr.loc), ranked2D, rhs);

                return builder.create<mlir::tc::MatMulOp>(loc(expr.loc), ranked2D, lhsCast, rhsCast);
            },
            [&](const AddNode &n) -> mlir::Value {
                mlir::Value lhs = mlirGenExpr(*n.left);
                if (!lhs) return nullptr;

                mlir::Value rhs = mlirGenExpr(*n.right);
                if (!rhs) return nullptr;

                return builder.create<mlir::tc::AddOp>(loc(expr.loc), lhs, rhs);
            },
            [&](const ReluNode &n) -> mlir::Value {
                mlir::Value input = mlirGenExpr(*n.tensor);
                if (!input) return nullptr;
                return builder.create<mlir::tc::ReluOp>(loc(expr.loc), input);
            },
            [&](const TransposeNode &n) -> mlir::Value {
                mlir::Value input = mlirGenExpr(*n.tensor);
                if (!input) return nullptr;

                auto permAttr = builder.getDenseI64ArrayAttr(n.perm);
                auto resultType = mlir::UnrankedTensorType::get(builder.getF32Type());

                return builder.create<mlir::tc::TransposeOp>(loc(expr.loc), resultType, input, permAttr);
            },
            [&](const ReshapeNode &n) -> mlir::Value {
                mlir::Value input = mlirGenExpr(*n.tensor);
                if (!input) return nullptr;

                auto shapeAttr = builder.getDenseI64ArrayAttr(n.new_shape);
                auto resultType = mlir::UnrankedTensorType::get(builder.getF32Type());

                return builder.create<mlir::tc::ReshapeOp>(loc(expr.loc), resultType, input, shapeAttr);
            },
        },
        expr.node);
}

};

// free function; public interface
mlir::ModuleOp mlirGen(mlir::MLIRContext &context, const Expr &expr) {
  return MLIRGenImpl(context).mlirGen(expr);
}

} // namespace tc
