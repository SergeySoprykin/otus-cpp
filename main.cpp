#include <array>
#include <cstddef>
#include <map>
#include <iostream>

#include "container.h"

template<typename T, std::size_t MaxObjects>
class SimpleFixedAllocator {
private:
    alignas(alignof(T)) std::array<unsigned char, MaxObjects * sizeof(T)> memory_;
    std::size_t next_free_ = 0;

public:
    using value_type = T;
    
    template<typename U>
    struct rebind {
        using other = SimpleFixedAllocator<U, MaxObjects>;
    };

    SimpleFixedAllocator() noexcept = default;
    
    template<typename U>
    SimpleFixedAllocator(const SimpleFixedAllocator<U, MaxObjects>&) noexcept {}

    T* allocate(std::size_t n) {
        if (next_free_ + n > MaxObjects) {
            throw std::bad_alloc();
        }
        
        T* ptr = reinterpret_cast<T*>(memory_.data() + next_free_ * sizeof(T));
        next_free_ += n;
        return ptr;
    }

    void deallocate(T*, std::size_t) noexcept {
        // Simple version: memory is never reused
    }

    template<typename U, typename... Args>
    void construct(U* ptr, Args&&... args) {
        new (ptr) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* ptr) {
        ptr->~U();
    }

    std::size_t max_size() const noexcept {
        return MaxObjects;
    }
};

// Comparison operators
template<typename T, typename U, std::size_t N>
bool operator==(const SimpleFixedAllocator<T, N>&, const SimpleFixedAllocator<U, N>&) noexcept {
    return true;
}

template<typename T, typename U, std::size_t N>
bool operator!=(const SimpleFixedAllocator<T, N>&, const SimpleFixedAllocator<U, N>&) noexcept {
    return false;
}

int factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    constexpr std::size_t MAX_SIZE = 10;
    using MapAllocator = SimpleFixedAllocator<std::pair<const int, int>, MAX_SIZE>;
    std::map<int, int> std_map;
    std::map<int, int, std::less<int>, MapAllocator> alloc_map;

    // Fill std map
    for (int i = 0; i < 10; ++i) {
        std_map[i] = factorial(i);
    }
    // Print std map
    for (int i = 0; i < 10; ++i) {
        std::cout << i << " " << std_map[i] << std::endl;
    }
   
    // Fill alloc map
    for (int i = 0; i < 10; ++i) {
        alloc_map[i] = factorial(i);
    }
    // Print alloc map
    for (int i = 0; i < 10; ++i) {
        std::cout << i << " " << alloc_map[i] << std::endl;
    }
    
    using VectorAllocator = SimpleFixedAllocator<int, MAX_SIZE>;
    MyVector<int> my_vector;
    // Fill my_vector
    for (int i = 0; i < 10; ++i) {
        my_vector.push_back(i);
    }
    // Print my_vector
    for (int i = 0; i < 10; ++i) {
        std::cout << my_vector[i] << std::endl;
    }
    MyVector<int, VectorAllocator> my_vector_with_alloc;
    // Fill my_vector_with_alloc
    for (int i = 0; i < 10; ++i) {
        my_vector_with_alloc.push_back(i);
    }
    // Print my_vector_with_alloc
    for (int i = 0; i < 10; ++i) {
        std::cout << my_vector_with_alloc[i] << std::endl;
    }
    return 0;
}