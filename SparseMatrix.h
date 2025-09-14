#pragma once

#include <unordered_map>
#include <functional>

template<typename T, T default_value = T{}>
class SparseInfiniteMatrix {
private:
    // Custom hash for std::pair to use as key in unordered_map
    struct PairHash {
        template <class T1, class T2>
        std::size_t operator() (const std::pair<T1, T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };

    std::unordered_map<std::pair<int, int>, T, PairHash> data;
	T default_val;

public:
    SparseInfiniteMatrix() :default_val{default_value}{};

    // Proxy class for the second dimension
    class RowProxy {
    private:
        SparseInfiniteMatrix<T>& matrix;
        int row;
        
    public:
        RowProxy(SparseInfiniteMatrix<T>& m, int r) : matrix(m), row(r) {}
        
        T& operator[](int col) {
            auto key = std::make_pair(row, col);
            if (matrix.data.find(key) == matrix.data.end()) {
                matrix.data[key] = default_value;
            }
            return matrix.data[key];
        }
        
        const T& operator[](int col) const {
            auto key = std::make_pair(row, col);
            auto it = matrix.data.find(key);
            if (it == matrix.data.end()) {
                return matrix.default_val;
            }
            return it->second;
        }
    };

    // First indexing returns a proxy object
    RowProxy operator[](int row) {
        return RowProxy(*this, row);
    }

    // Const version for read-only access
    class ConstRowProxy {
    private:
        const SparseInfiniteMatrix<T>& matrix;
        int row;
        
    public:
        ConstRowProxy(const SparseInfiniteMatrix<T>& m, int r) : matrix(m), row(r) {}
        
        const T& operator[](int col) const {
            auto key = std::make_pair(row, col);
            auto it = matrix.data.find(key);
            if (it == matrix.data.end()) {
                return matrix.default_val;
            }
            return it->second;
        }
    };

    ConstRowProxy operator[](int row) const {
        return ConstRowProxy(*this, row);
    }

    size_t size() const { return data.size(); }
    
    // Iterate non-default elements
    template<typename Func>
    void forEach(Func func) const {
        for (const auto& entry : data) {
            func(entry.first.first, entry.first.second, entry.second);
        }
    }
    
    // Remove element
    void erase(int row, int col) {
        data.erase(std::make_pair(row, col));
    }
    
    // Check if element is explicitly set
    bool contains(int row, int col) const {
        return data.find(std::make_pair(row, col)) != data.end();
    }
};