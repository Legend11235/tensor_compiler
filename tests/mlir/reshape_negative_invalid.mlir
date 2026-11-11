func.func @test_reshape_neg(%a: tensor<2x6xf32>) -> tensor<*xf32> {
  %0 = tc.reshape %a, [-3, -4] : tensor<2x6xf32> -> tensor<*xf32>
  return %0 : tensor<*xf32>
}