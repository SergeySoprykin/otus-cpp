#pragma once

#include <type_traits>
#include <string>
#include <vector>
#include <list>

template<typename T>
struct is_vector_or_list : std::false_type {};

template<typename T, typename Alloc>
struct is_vector_or_list<std::vector<T, Alloc>> : std::true_type {};

template<typename T, typename Alloc>
struct is_vector_or_list<std::list<T, Alloc>> : std::true_type {};

template<>
struct is_vector_or_list<std::string> : std::false_type {};

template<typename T>
inline constexpr bool is_vector_or_list_v = is_vector_or_list<T>::value;
