func.func @test_transpose(%a: tensor<2x3xf32>) -> tensor<3x2xf32> {
  %0 = tc.transpose %a, [1, 0] : tensor<2x3xf32> -> tensor<3x2xf32>
  return %0 : tensor<3x2xf32>
}
