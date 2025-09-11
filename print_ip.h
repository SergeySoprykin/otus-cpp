#pragma once

#include "specializations.h"

#include <type_traits>
#include <iostream>
#include <cstdint>
#include <iterator>
#include <algorithm>


template <typename T,
          typename = std::enable_if_t<std::is_integral_v<T>>>
void print_ip(T ip) {
    const auto* byte_ptr = reinterpret_cast<const unsigned char*>(&ip);
    for (size_t i = 0; i < sizeof(T); ++i) {
        std::cout << static_cast<int>(byte_ptr[sizeof(T)-1 - i]);
        if (i < sizeof(T) - 1) {
            std::cout << ".";
        }
    }
    std::cout << std::endl;
}

template <typename T,
          typename = std::enable_if_t<is_vector_or_list_v<T>>>
void print_ip(const T& ip) {
    if (ip.empty()) {
        return;
    }
    std::copy(ip.begin(), std::prev(ip.end()),
              std::ostream_iterator<typename T::value_type>(std::cout, "."));
    std::cout << *ip.rbegin() << std::endl;
}

inline void print_ip(const std::string& ip){
    std::cout << ip << std::endl;
}
