func.func @test_relu(%a: tensor<3x4xf32>) -> tensor<3x4xf32> {
  %0 = tc.relu %a : tensor<3x4xf32>
  return %0 : tensor<3x4xf32>
}
