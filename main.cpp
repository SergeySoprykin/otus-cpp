#include "allocator.h"
#include "container.h"

#include <map>
#include <functional>
#include <iostream>

int factorial(int number) {
    int res = 1;
    for (int i = 2; i <= number; ++i) {
        res *= i;
    }
    return res;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& stream, const std::pair<T, U>& pair) {
    stream << pair.first << ' ' << pair.second;
    return stream;
}

template <typename Container>
void print(Container& container) {
    for (auto& element : container) {
        std::cout << element << std::endl;
    }
}

int main() {
    std::map<int, int> map_std_alloc;
    std::map<int, int, std::less<int>, SimpleArrayAllocator<std::pair<const int, int>, 10>> map_custom_alloc;
    ForwardList<int> list_std_alloc;
    ForwardList<int, SimpleArrayAllocator<int, 10>> list_custom_alloc;

    for (int i = 0; i < 10; ++i) {
        map_std_alloc.emplace(i, factorial(i));
    }
    for (int i = 0; i < 10; ++i) {
        map_custom_alloc.emplace(i, factorial(i));
    }

    auto last = list_std_alloc.begin();
    for (int i = 0; i < 10; ++i) {
        last = list_std_alloc.insert_after(last.get_node(), i);
    }

    auto last_custom = list_custom_alloc.begin();
    for (int i = 0; i < 10; ++i) {
        last_custom = list_custom_alloc.insert_after(last_custom.get_node(), i);
    }

    print(map_std_alloc);
    print(map_custom_alloc);
    print(list_std_alloc);
    print(list_custom_alloc);
}
