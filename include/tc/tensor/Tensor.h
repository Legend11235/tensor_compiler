#pragma once
#include <cstddef>
#include <vector>

namespace tc {

class Tensor {

    public:
    Tensor(std::vector<size_t> shape);
    Tensor(std::vector<size_t> shape, const float *data);

    // rule of 5 for move semantics
    ~Tensor(); // destructor
    Tensor(const Tensor &other); // copy constructor
    Tensor& operator=(const Tensor& rhs); // copy assignment
    Tensor(Tensor&& other) noexcept; // move constructor
    Tensor& operator=(Tensor&& rhs) noexcept; // move assignment

    // other methods



    // private fields
    // size_t is a type used for non-negative counts and sizes
    private:
    size_t size_;
    std::vector<size_t> shape_; //dimension
    std::vector<size_t> strides_; // how many elemnts are skipped to move through 1 dimension. ex for 3D tensor: {x,y,1}
    float* data_; // pointer to the flat array storing the tensor


};

}