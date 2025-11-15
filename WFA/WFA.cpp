#include <iostream>
#include <string>
#include <vector>

// This is the Wagner-Fischer algorithm for finding the Levenstein distance between two strings
// also known as edit distance. It uses dynamic programming it's one of the classical algorithms
// in this scheme. It works in O(n1*n2) time and space. 

size_t getLevensteinDistance(const std::string& w ,const std::string& v) {
    std::vector<std::vector<int>> D(w.length() + 1, std::vector(v.length() + 1, -1));
    for (size_t i = 0; i < w.length() + 1; ++i) {
        D[i][0] = i;
    }
    for (size_t j = 0; j < v.length() + 1; ++j) {
        D[0][j] = j;
    }

    for (size_t i = 1; i < w.length() + 1; ++i) {
        for (size_t j = 1; j < v.length() + 1; ++j) {
            int m1;
            if (w[i - 1] != v[j - 1])
                m1 = D[i-1][j-1] + 1;
            else 
                m1 = D[i-1][j-1];
            int m2 = D[i-1][j] + 1;
            int m3 = D[i][j-1] +1;
            D[i][j] = std::min(m1, std::min(m2, m3));
        }
    }
    
    return D[w.length()][v.length()];
}

int main() {
    std::cout << getLevensteinDistance("cat", "bat") << std::endl;
    std::cout << getLevensteinDistance("cat", "caps") << std::endl;
    std::cout << getLevensteinDistance("cat", "dog") << std::endl;
    std::cout << getLevensteinDistance("cat", "dot") << std::endl;
    std::cout << getLevensteinDistance("cat", "cap") << std::endl;
    std::cout << getLevensteinDistance("", "cap") << std::endl;
    std::cout << getLevensteinDistance("cat", "") << std::endl;
    std::cout << getLevensteinDistance("oooooo", "cap") << std::endl;
    std::cout << getLevensteinDistance("cat", "oooooo") << std::endl;
    return 0;
}