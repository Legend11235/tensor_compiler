#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Tools/mlir-opt/MlirOptMain.h"

#include "tc/dialect/TensorDialect.h"

int main(int argc, char **argv) {
  mlir::DialectRegistry registry;
  registry.insert<mlir::tc::TensorDialect, mlir::func::FuncDialect>();

  return mlir::asMainReturnCode(
      mlir::MlirOptMain(argc, argv, "tc optimizer driver\n", registry));
}