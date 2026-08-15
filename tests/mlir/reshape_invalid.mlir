func.func @test_reshape_bad(%a: tensor<2x6xf32>) -> tensor<3x5xf32> {
  %0 = tc.reshape %a, [3, 5] : tensor<2x6xf32> -> tensor<3x5xf32>
  return %0 : tensor<3x5xf32>
}
