#pragma once
#include <cstddef>
#include <vector>

namespace tc {

class Tensor {

    public:
    Tensor(const std::vector<size_t> &shape);
    Tensor(const std::vector<size_t> &shape, const float *data);

    // rule of 5 for move semantics
    ~Tensor(); // destructor
    Tensor(const Tensor &other); // copy constructor
    Tensor& operator=(const Tensor& rhs); // copy assignment
    Tensor(Tensor&& other) noexcept; // move constructor
    Tensor& operator=(Tensor&& rhs) noexcept; // move assignment

    // other methods
    
    // read and write individual elements of a tensor
    float& at(const std::vector<size_t>& indices);
    const float& at(const std::vector <size_t>& indices) const;
    // read and write pointer of tensor
    float* data();
    const float* data() const;
    // shape queries
    const std::vector<size_t>& shape() const;
    size_t rank() const;
    size_t size() const;
    const std::vector<size_t>& strides() const;



    // private fields & helpers
    private:
    size_t size_; // size_t is a type used for non-negative counts and sizes
    std::vector<size_t> shape_; //dimension
    std::vector<size_t> strides_; // how many elemnts are skipped to move through 1 dimension. ex for 3D tensor: {x,y,1}
    float* data_; // pointer to the flat array storing the tensor

    void compute_strides();
    

    



};

// operations on Tensors
// my compiler will opitmize these
Tensor matmul(const Tensor& a, const Tensor& b);
Tensor add(const Tensor& a, const Tensor& b);
Tensor relu(const Tensor& tensor);
Tensor reshape(const Tensor& tensor, const std::vector<size_t>& new_shape);
}