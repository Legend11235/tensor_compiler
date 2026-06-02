#include "tc/tensor/Tensor.h"
#include <cassert>
#include <iostream>

using namespace tc;

int main() {
    std::cout << "Running tests...\n";
    
    Tensor t({3, 4});

    // tensor properties
    assert(t.rank() == 2);
    assert(t.size() == 12);
    assert(t.shape() == std::vector<size_t>({3, 4}));
    assert(t.strides() == std::vector<size_t>({4, 1}));

    // element access test
    Tensor t2({3, 4});
    t2.at({1, 2}) = 7.0f;
    assert(t2.at({1, 2}) == 7.0f);

    //copy constructor test
    Tensor t3({3, 4});
    t3.at({0, 0}) = 5.0f;

    Tensor t4(t3); // this is copy construsctor not copy assignment
    t4.at({0, 0}) = 99.0f;   

    assert(t3.at({0, 0}) == 5.0f); // original shuold be unchanged
    assert(t4.at({0, 0}) == 99.0f); // copy should have a new value

    //copy assignment test
    Tensor t5({2, 2});
    t5.at({0, 0}) = 3.0f;

    Tensor t6({2, 2}); // t6 already exists
    t6 = t5; // copy assignment
    t6.at({0, 0}) = 77.0f;

    assert(t5.at({0, 0}) == 3.0f); // original unchanged
    assert(t6.at({0, 0}) == 77.0f); // copy has new value


    // Move Constructor Test
    Tensor t7({2,2});
    t7.at({0,0}) = 42.0f;
    Tensor t8 = std::move(t7); //move constructor
    assert(t8.at({0,0}) == 42.0f); // new tensor should have the same data
    assert(t7.data() == nullptr); // source should be nullptr
    

    // Move Assignment Test
    Tensor t9({6,6});
    t9.at({0,0}) = 17.0f;
    Tensor t10({6,6});
    t10 = std::move(t9);
    assert(t10.at({0,0}) == 17.0f); // new tensor should have the same data
    assert(t9.data() == nullptr); // source should be nullptr


    // Tensor operation tests
    //add
    float data[] = {1.0f, 2.0f, 3.0f, 4.0f};
    Tensor t11({2, 2}, data);
    Tensor t12({2, 2}, data);
    Tensor t13 = add(t11, t12);
    assert(t13.at({0, 0}) == 2.0f);
    assert(t13.at({0, 1}) == 4.0f);
    assert(t13.at({1, 0}) == 6.0f);
    assert(t13.at({1, 1}) == 8.0f);

    // Relu
    float data2[] = {-1.0f, 2.0f, -3.0f, 4.0f};
    Tensor t14({2,2}, data2);
    Tensor t15 = relu(t14);
    assert(t15.at({0, 0}) == 0.0f);
    assert(t15.at({0, 1}) == 2.0f);
    assert(t15.at({1, 0}) == 0.0f);
    assert(t15.at({1, 1}) == 4.0f);

    //matmul
    float data3[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    float data4[] = {7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f};
    Tensor t16({2,3}, data3);
    Tensor t17({3,2}, data4);
    Tensor t18 = matmul(t16, t17);
    assert(t18.at({0, 0}) == 58.0f);
    assert(t18.at({0, 1}) == 64.0f);
    assert(t18.at({1, 0}) == 139.0f);
    assert(t18.at({1, 1}) == 154.0f);


    std::cout << "All tests passed!\n";
    return 0;
}




