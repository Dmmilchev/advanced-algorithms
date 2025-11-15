#include <iostream>
#include <vector>
#include <math.h>

typedef std::vector<int> Array;
typedef std::vector<std::vector<int>> Matrix;

// input: Array with n elements
// output: Index data structure which enables you to answer queries in RMQ(i, j) = k, 
//         where A[k] = min(A[i], A[i+1], ..., A[j]) for i <= j

// RMQ1 build a matrix data structure which takes O(n^2) space and time. That's the naive approach.
Matrix RMQ1(const Array& A) {
    size_t n = A.size();
    Matrix M(n, Array(n, -1));

    for (size_t i = 0; i < n; ++i) {
        M[i][i] = i;
    }

    for (size_t i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (A[M[i][j-1]] <= A[j]) 
                M[i][j] = M[i][j-1];
            else 
                M[i][j] = j;
        }
    }

    return M;
}

int query1(const Matrix& M, size_t i, size_t j) {
    return M[i][j];
}

// But we can do better than the naive solution. RMQ2 build an index 
// with O(nlogn) space and time complexity and again O(1) query time.
Matrix RMQ2(const Array& A) {
    size_t n = A.size();
    size_t logn = std::floor(std::log2(n));
    Matrix M(n, Array(logn + 1, -1));

    for (size_t i = 0; i < n; ++i) {
        M[i][0] = i;
    }
    for (size_t d = 1; d <= logn; ++d) {
        for (size_t i = 0; i < n - 1; ++i){
            size_t l = std::pow(2, d);
            size_t j = i + l - 1;
            if (j >= n) continue;

            if (A[M[i][d - 1]] <= A[M[i + l/2][d - 1]])
                M[i][d] = M[i][d - 1];
            else   
                M[i][d] = M[i + l/2][d - 1];
        }
    }

    return M;
}

int query2(const Matrix& M, const Array& A, size_t i, size_t j) {
    if (i == j) return M[i][0];

    size_t d = std::floor(std::log2(j - i)); // d  = 0
    size_t l = std::pow(2, d); //  l = 1
    if (A[M[i][d]] <= A[M[j - l + 1][d]])
        return M[i][d];
    else
        return M[j - l + 1][d];
} 

int main() {
    Array A{1, -1, 3, 4, 3, 10, -5, 10, 22, -4};
    Array A{1, -1, -3, 4};

    Matrix M1 = RMQ1(A);

    std::cout << (A[query1(M1, 0, 9)] == -5) << std::endl;
    std::cout << (A[query1(M1, 0, 3)] == -1) << std::endl;
    std::cout << (A[query1(M1, 3, 4)] == 3) << std::endl;
    std::cout << (A[query1(M1, 3, 5)] == 3) << std::endl;
    std::cout << (A[query1(M1, 3, 6)] == -5) << std::endl;
    std::cout << (A[query1(M1, 3, 7)] == -5) << std::endl;
    std::cout << (A[query1(M1, 3, 9)] == -5) << std::endl;

    Matrix M2 = RMQ2(A);

    std::cout << "RMQ2" << std::endl;
    std::cout << (A[query2(M2, A, 0, 9)] == -5) << std::endl;
    std::cout << (A[query2(M2, A, 0, 3)] == -1) << std::endl;
    std::cout << (A[query2(M2, A, 3, 4)] == 3) << std::endl;
    std::cout << (A[query2(M2, A, 3, 5)] == 3) << std::endl;
    std::cout << (A[query2(M2, A, 3, 6)] == -5) << std::endl;
    std::cout << (A[query2(M2, A, 3, 7)] == -5) << std::endl;
    std::cout << (A[query2(M2, A, 3, 9)] == -5) << std::endl;
    return 0;
}