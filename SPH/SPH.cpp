#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <climits>
#include <algorithm>
#include <random>

#define INVALID UINT64_MAX

struct Func {
    std::unordered_map<size_t, std::unordered_map<char, size_t>> delta;

    void set(size_t from, char symbol, size_t res) {
        delta[from][symbol] = res;
    }

    size_t get(size_t state, char symbol) {
        if (delta.count(state) && delta[state].count(symbol))
            return delta[state][symbol];
        else
            return INVALID;
    }
};

struct Trie {
    std::string alphabet = "zyxwvutsrqponmlkjihgfedcba";
    size_t last;
    std::unordered_set<size_t> final;
    Func delta;
    Func lambda;

    Trie() = default;

    void addWord(const std::string& word) {
        size_t q = 0;
        
        int i = 0;
        for (i; i < word.size(); ++i) {
            if (delta.get(q, word[i]) != INVALID) q = delta.get(q, word[i]);
            else break;
        }

        for(i; i < word.size(); ++i) {
            ++last;
            delta.set(q, word[i], last);
            q = last;
        }

        final.insert(last);
    }

    bool traverse(const std::string& word) {
        size_t q = 0;
        for (char c : word) {
            q = delta.get(q, c);

            if (q == INVALID) return false;
        }
        return final.count(q) > 0;
    }

    size_t hash(const std::string& word) {
        size_t q = 0;
        size_t hash = 0;
        for (char c : word) {
            q = delta.get(q, c);
            if (q == INVALID) return INVALID;
            hash += lambda.get(q, c);
        }
        return hash;
    }

    void initLambdaRec(size_t q) {
        size_t k = final.count(q);
        for (char c: alphabet) {
            size_t v = delta.get(q, c);
            if (v != INVALID) {
                lambda.set(q, c, k);
                ++k;
                initLambdaRec(v);
            }
        }
    }

    void initLambda() {
        initLambdaRec(0);
    }
};

std::vector<std::string> generateRandomStrings(int N) {
    std::vector<std::string> result;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> lengthDist(5, 15);  // String length between 5-15
    std::uniform_int_distribution<> charDist(0, 25);    // For a-z
    
    for (int i = 0; i < N; ++i) {
        std::string randomStr;
        int length = lengthDist(gen);
        
        for (int j = 0; j < length; ++j) {
            randomStr += 'a' + charDist(gen);
        }
        
        result.push_back(randomStr);
    }
    
    return result;
};


int main() {
    std::vector<std::string> strings = generateRandomStrings(10);
    Trie t;
    for (auto s : strings) {
        t.addWord(s);
    }
    std::sort(strings.begin(), strings.end());
    for (auto s : strings) {
        std::cout << t.traverse(s) << std::endl;
        std::cout << t.hash(s) << std::endl;;

    }
    std::cout << strings[0];
    return 0;
}