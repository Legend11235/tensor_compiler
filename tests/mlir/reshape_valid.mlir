func.func @test_reshape(%a: tensor<2x6xf32>) -> tensor<3x4xf32> {
  %0 = tc.reshape %a, [3, 4] : tensor<2x6xf32> -> tensor<3x4xf32>
  return %0 : tensor<3x4xf32>
}
