#include "SparseMatrix.h"
#include <iostream>

void print_matrix(const SparseInfiniteMatrix<int, 0>& matrix) {
    for (std::size_t i = 1; i <= 8; ++i){
        for (std::size_t j = 1; j <= 8; ++j) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << "Matrix size: " << matrix.size() << std::endl;
}

int main()
{
    SparseInfiniteMatrix<int, 0> matrix;
    for (std::size_t i = 0; i < 9; ++i){
        matrix[i][i] = i;
        matrix[i][9 - i] = 9 - i;
    }

    print_matrix(matrix);
    
    matrix.forEach([](auto i, auto j, auto x){std::cout << "[" << i << ", " << j << "] = " << x << "; ";});
   
    return 0;
}
