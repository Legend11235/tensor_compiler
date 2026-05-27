#include "tc/tensor/Tensor.h"
#include <algorithm>

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
        



} 