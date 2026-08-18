#include "tc/parser/Lexer.h"
#include "tc/parser/Parser.h"
#include "tc/mlirgen/MLIRGen.h"
#include "tc/dialect/TensorDialect.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>

int main() {
    mlir::MLIRContext context;
    context.getOrLoadDialect<mlir::tc::TensorDialect>();
    context.getOrLoadDialect<mlir::func::FuncDialect>();
    context.getOrLoadDialect<mlir::tensor::TensorDialect>();

    std::string source = "add(relu(x1), transpose(x2, [1, 0]))";
    tc::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    tc::Parser parser(std::move(tokens));
    auto ast = parser.parse();

    mlir::ModuleOp module = tc::mlirGen(context, *ast);

    if (mlir::failed(mlir::verify(module))) {
        std::cerr << "Module verification failed!\n";
        return 1;
    }

    module.print(llvm::outs());
    return 0;
}
