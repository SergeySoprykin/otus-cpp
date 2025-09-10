#include <memory>

template <typename T, typename Allocator = std::allocator<T>>
class ForwardList {
public:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };

    ForwardList() = default;
    ~ForwardList() { 
        while (head_) {
            Node* temp = head_;
            head_ = head_->next;
            allocator_.destroy(temp);
            allocator_.deallocate(temp, 1);
        }
    }

    Node* insert(const T& value) {
        Node* new_node = allocator_.allocate(1);
        allocator_.construct(new_node, value);
        new_node->next = head_;
        head_ = new_node;
        return new_node;
    }

    // Insert after specific position
    Node* insert_after(Node* after, const T& value) {
        if (!after) {
            return insert(value);
        }
        Node* new_node = allocator_.allocate(1);
        allocator_.construct(new_node, value);
        new_node->next = after->next;
        after->next = new_node;
        return new_node;
    }

    // Access
    T& front() { return head_->data; }
    bool empty() const { return head_ == nullptr; }

    // Iterator
    class Iterator {
        Node* current;
    public:
        Iterator(Node* node) : current(node) {}
        T& operator*() { return current->data; }
        Iterator& operator++() { current = current->next; return *this; }
        bool operator!=(const Iterator& other) { return current != other.current; }
        Node* get_node() { return current; }
    };

    Iterator begin() { return Iterator(head_); }
    Iterator end() { return Iterator(nullptr); }

private:

    using NodeAlloc = typename Allocator::template rebind<Node>::other;
    
    Node* head_ = nullptr;
    NodeAlloc allocator_;
};