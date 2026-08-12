func.func @test_add(%a: tensor<3x4xf32>, %b: tensor<3x4xf32>) -> tensor<3x4xf32> {
  %0 = tc.add %a, %b : tensor<3x4xf32>
  return %0 : tensor<3x4xf32>
}
