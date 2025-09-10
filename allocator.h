#pragma once

#include <memory>
#include <array>
#include <cstddef>
#include <stdexcept>

template <typename T, std::size_t N>
class SimpleArrayAllocator {
public:
    using value_type = T;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    template <typename U>
    struct rebind {
        using other = SimpleArrayAllocator<U, N>;
    };

    SimpleArrayAllocator() : free_idx_(0) {}

    T* allocate(size_type n) {
        if (free_idx_ + n > N) {
            throw std::bad_alloc();
        }
        
        T* ptr = reinterpret_cast<T*>(&memory_array_[free_idx_]);
        free_idx_ += n;
        return ptr;
    }

    void deallocate([[maybe_unused]] T* p, [[maybe_unused]] size_type n) noexcept {}

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        new(p) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* p) {
        p->~U();
    }

    size_type max_size() const noexcept {
        return N - free_idx_;
    }

private:
    std::array<std::aligned_storage_t<sizeof(T), alignof(T)>, N> memory_array_;
    size_type free_idx_;
};

template <typename T, typename U, std::size_t N>
bool operator==(const SimpleArrayAllocator<T, N>&, const SimpleArrayAllocator<U, N>&) {
    return true; 
}

template <typename T, typename U, std::size_t N>
bool operator!=(const SimpleArrayAllocator<T, N>& a, const SimpleArrayAllocator<U, N>& b) {
    return !(a == b);
}
