#include "lib.h"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>

std::vector<std::string> split(const std::string &str, char d) {
    std::vector<std::string> result;
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos) {
        result.push_back(str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }
    result.push_back(str.substr(start));
    return result;
}

void print_ips(const std::vector<std::vector<std::string>>& ips) {
    for(auto ip = ips.cbegin(); ip != ips.cend(); ++ip) {
        for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part) {
            if (ip_part != ip->cbegin()) {
                std::cout << ".";
            }
            std::cout << *ip_part;
        }
        std::cout << std::endl;
    }
}


int main([[maybe_unused]]int argc, [[maybe_unused]]char const *argv[])
{
    try{
        std::vector<std::vector<std::string>> ip_pool;
        for(std::string line; std::getline(std::cin, line);) {
            std::vector<std::string> ip_address = split(line, '\t');
            ip_pool.push_back(split(ip_address.at(0), '.'));
        }
        
        std::sort(ip_pool.begin(), ip_pool.end(), [](auto& a, auto& b){ 
            return std::make_tuple(std::stoi(a[0]), std::stoi(a[1]), std::stoi(a[2]), std::stoi(a[3])) >
                        std::make_tuple(std::stoi(b[0]), std::stoi(b[1]), std::stoi(b[2]), std::stoi(b[3]));});
        print_ips(ip_pool);

        auto sorted_vector = ip_pool;
        std::string request = "1";
        sorted_vector.erase(std::remove_if(sorted_vector.begin(), sorted_vector.end(),
            [request](const auto& val){return val[0] != request;}), sorted_vector.end());
        print_ips(sorted_vector);

        sorted_vector = ip_pool;
        request = "46";
        std::string request2 = "70";
        sorted_vector.erase(std::remove_if(sorted_vector.begin(), sorted_vector.end(),
            [request, request2](const auto& val){return val[0] != request || val[1] != request2;}), sorted_vector.end());
        print_ips(sorted_vector);

        sorted_vector = ip_pool;
        sorted_vector.erase(std::remove_if(sorted_vector.begin(), sorted_vector.end(),
        [request](const auto& val){return std::none_of(val.begin(), val.end(),
            [request](const auto& sub_val){return sub_val == request;});}), sorted_vector.end());
        print_ips(sorted_vector);
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
