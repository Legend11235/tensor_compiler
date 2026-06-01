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
        shape_ = other.shape_;
        strides_ = other.strides_;
        size_ = other.size_;
        other.data_ = nullptr;
    }

    // move assignment
    Tensor& Tensor::operator=(Tensor&& rhs) noexcept{
        if(this == &rhs)
            return *this;
        delete[] data_;
        data_ = rhs.data_;
        shape_ = rhs.shape_;
        strides_ = rhs.strides_;
        size_ = rhs.size_;
        rhs.data_ = nullptr;
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

} 