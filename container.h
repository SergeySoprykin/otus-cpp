#include <memory>

template<typename T, typename Allocator = std::allocator<T>>
class MyVector {
private:
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    Allocator alloc_;

    void realloc() {
        size_t new_capacity = capacity_ + 1;
        T* new_data = alloc_.allocate(new_capacity);
        
        for (size_t i = 0; i < size_; ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, new_data + i, std::move(data_[i]));
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
        }
        
        if (data_) {
			alloc_.deallocate(data_, capacity_);
		}
        data_ = new_data;
        capacity_ = new_capacity;
    }

public:
    using value_type = T;
    using allocator_type = Allocator;

    MyVector() = default;
    explicit MyVector(const Allocator& alloc) : alloc_(alloc) {}

    ~MyVector() {
        clear();
        if (data_) {
			alloc_.deallocate(data_, capacity_);
		}
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
			realloc();
		}
        std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, value);
        ++size_;
    }

    void push_back(T&& value) {
        if (size_ >= capacity_) {
			realloc();
		}
        std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, std::move(value));
        ++size_;
    }

    void pop_back() {
        if (size_ > 0) {
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + size_ - 1);
            --size_;
        }
    }

    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
        }
        size_ = 0;
    }

    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    T* begin() { return data_; }
    T* end() { return data_ + size_; }
    const T* begin() const { return data_; }
    const T* end() const { return data_ + size_; }
};