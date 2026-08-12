func.func @test_matmul_bad(%a: tensor<2x3xf32>, %b: tensor<9x4xf32>) -> tensor<2x4xf32> {
  %0 = tc.matmul %a, %b : tensor<2x3xf32>, tensor<9x4xf32> -> tensor<2x4xf32>
  return %0 : tensor<2x4xf32>
}
