func.func @test_transpose_notperm(%a: tensor<2x3xf32>) -> tensor<2x2xf32> {
  %0 = tc.transpose %a, [0, 0] : tensor<2x3xf32> -> tensor<2x2xf32>
  return %0 : tensor<2x2xf32>
}
