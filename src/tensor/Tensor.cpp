#include "tc/tensor/Tensor.h"
#include <algorithm>
#include <stdexcept>

namespace tc {

    void Tensor::compute_strides(){
        // check shape is non empty
        if (shape_.empty()) return;

        size_ = 1; // make sure size starts at 1 and not 0
        strides_.resize(shape_.size()); // initialize strides_ vector to have same elements as shape
        // last element of stride is always 1
        strides_[shape_.size() - 1] = 1;

        // compute total num of elements
        for(size_t i = 0; i < shape_.size(); i++) {
            size_ *= shape_[i];
        }

        // compute strides
        for(size_t i = shape_.size() - 1; i > 0; i --){
            strides_[i-1] = strides_[i] * shape_[i];
        }
    }

    // constructor
    Tensor::Tensor(const std::vector<size_t> &shape){
        shape_ = shape;
        compute_strides();          
        data_ = new float[size_](); // heap allocation; () zeroes it
    }

    //overloading constructor - wraps float array into Tensor
    Tensor::Tensor(const std::vector<size_t> &shape, const float* data){
        shape_ = shape;
        compute_strides();  
        data_ = new float[size_]; // allocate first        
        std::copy(data, data + size_, data_); // now we can copy into it
    }

    // Rule of 5
    // destructor
    Tensor::~Tensor(){
        delete[] data_;
    }

    // copy constructor
    Tensor::Tensor(const Tensor &other){
        shape_ = other.shape_;
        compute_strides();
        data_ = new float[other.size_];
        std::copy(other.data_, other.data_ + size_, data_);
    }

    // assignment constructor
    Tensor& Tensor::operator=(const Tensor& rhs){
        // self assignment check
        if (this == &rhs) 
            return *this; 
        delete[] data_;    
        shape_ = rhs.shape_;
        compute_strides();
        data_ = new float[rhs.size_];
        std::copy(rhs.data_, rhs.data_ + size_, data_);
        return *this;
    }

    // move constructor
    Tensor::Tensor(Tensor&& other) noexcept{
        data_ = other.data_;
        shape_ = std::move(other.shape_);
        strides_ = std::move(other.strides_);
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // move assignment
    Tensor& Tensor::operator=(Tensor&& rhs) noexcept{
        if(this == &rhs)
            return *this;
        delete[] data_;
        data_ = rhs.data_;
        shape_ = std::move(rhs.shape_);
        strides_ = std::move(rhs.strides_);
        size_ = rhs.size_;
        rhs.data_ = nullptr;
        rhs.size_ = 0;
        return *this;
    }

    // at() - index of element in flat array

    // for writing
    float& Tensor::at(const std::vector<size_t>& indices){
        // check bounds
        if (indices.size() != shape_.size()) {
            throw std::out_of_range("wrong number of indices");
        }
        // compute offset
        size_t offset = 0;
        for(size_t i = 0; i < indices.size(); i ++){
            offset += indices[i] * strides_[i];
        }
        return data_[offset];
    }
    // for reading on const Tensors
    const float& Tensor::at(const std::vector<size_t>& indices) const{
        // check bounds
        if (indices.size() != shape_.size()) {
            throw std::out_of_range("wrong number of indices");
        }
        // compute offset
        size_t offset = 0;
        for(size_t i = 0; i < indices.size(); i ++){
            offset += indices[i] * strides_[i];
        }
        return data_[offset];
    }

    // shape queries
    const std::vector<size_t>& Tensor::shape() const{
        return shape_;
    }

    size_t Tensor::rank() const{
        return shape_.size();
    }

    size_t Tensor::size() const{
        return size_;
    }

    const std::vector<size_t>& Tensor::strides() const{
        return strides_;
    }

    float* Tensor::data(){
        return data_;
    }

    const float* Tensor::data() const{
        return data_;
    }

    // operations on Tensors
    // my compiler will opitmize these
    Tensor add(const Tensor& a, const Tensor& b){
        // a and b need to have same dimension
        if(a.shape() != b.shape()){
            throw std::invalid_argument("tensors must have same dimenstions for addition");
        }
        Tensor res(a.shape());
        for(size_t i = 0; i < a.size(); i ++){
            res.data()[i] = a.data()[i] + b.data()[i];
        }
        return res;
    }

    // relu : negatives turn into 0's
    Tensor relu(const Tensor& tensor){
        Tensor res(tensor.shape());
        for(size_t i = 0; i < tensor.size(); i++){
            if (tensor.data()[i] < 0){
                res.data()[i] = 0;
            }
            else{
                res.data()[i] = tensor.data()[i];
            }
        }
        return res;
    }

    // reshape: change dimensions of tensor and return
    Tensor reshape(const Tensor& tensor, const std::vector<size_t>& new_shape){
        size_t new_shape_size = 1;
        // new_shape but have the same amount of elements as tesnor
        for(size_t i = 0; i < new_shape.size(); i++){
            new_shape_size *= new_shape[i];          
        }

        if(new_shape_size != tensor.size()){
            throw std::invalid_argument("new shape must hold same number of elements as original tensor");
        }    
        Tensor res(new_shape, tensor.data());
        return res;
    }

    // 2d mul
    // {M, K} * {K, N} = {M, N}
    Tensor matmul(const Tensor& a, const Tensor& b){
        // check tensor rank 2
        if(a.rank() != 2 || b.rank() != 2){
            throw std::invalid_argument("both tensors must be rank 2");
        }
        // {M, K} * {K, N} -> inner dimensions must match
        if(a.shape()[1] != b.shape()[0]){
            throw std::invalid_argument("inner dimensions must match for matmul -> {M, K} * {K, N}");
        }

        size_t M = a.shape()[0];
        size_t K = a.shape()[1];
        size_t N = b.shape()[1];

        Tensor res({M,N});

        for(size_t i = 0; i < M; i++){
            for(size_t j = 0; j< N; j++){
                for(size_t k = 0; k < K; k++){
                    // result[i][j] += a[i][k] * b[k][j]
                    res.data()[i*N + j] += a.data()[i*K + k] * b.data()[k*N +j];
                }
            }
        }

        return res;
    }


} 