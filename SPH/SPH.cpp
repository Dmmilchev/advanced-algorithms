#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <climits>
#include <algorithm>
#include <random>
#include <cassert>

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
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    size_t last;
    std::unordered_set<size_t> is_final;
    Func delta;
    Func lambda;
    std::vector<size_t> Nq;

    Trie() {
        last = 0;
        Nq = std::vector<size_t>({0});
    }

    void addWord(const std::string& word) {
        size_t q = 0;
        
        int i = 0;
        for (i; i < word.size(); ++i) {
            if (delta.get(q, word[i]) != INVALID) q = delta.get(q, word[i]);
            else break;
        }
        if (i == word.size()) { is_final.insert(q); }

        for(i; i < word.size(); ++i) {
            ++last;
            delta.set(q, word[i], last);
            q = last;
            Nq.push_back(0);
        }

        size_t v = 0;
        Nq[v]++;
        for (char c : word) {
            v = delta.get(v, c);
            Nq[v]++;
        }

        is_final.insert(last);
    }

    bool traverse(const std::string& word) {
        size_t q = 0;
        for (char c : word) {
            q = delta.get(q, c);

            if (q == INVALID) return false;
        }
        return is_final.count(q) > 0;
    }

    size_t hash(const std::string& word) {
        size_t q = 0;
        size_t hash = 0;
        for (char c : word) {
            size_t v = delta.get(q, c);
            if (v == INVALID) return INVALID;
            hash += lambda.get(q, c);
            q = v;
        }
        return hash;
    }

    void initLambdaRec(size_t q) {
        for (char c: alphabet) {
            size_t v = delta.get(q, c);

            if (v != INVALID) {
                size_t x = is_final.count(q);

                for (char c2 : alphabet) {
                    if (c2 >= c) break;
                    size_t v2 = delta.get(q, c2);
                    if (v2 == INVALID) continue;
                    x += Nq[v2];
                }
                lambda.set(q, c, x);

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
    
    std::mt19937 gen(123);
    std::uniform_int_distribution<> lengthDist(5, 15);  // String length between 5-15
    std::uniform_int_distribution<> charDist(0, 20);
    
    for (int i = 0; i < N; ++i) {
        std::string randomStr;
        int length = lengthDist(gen);
        
        for (int j = 0; j < length; ++j) {
            randomStr += 'a' + charDist(gen);
        }
        
        result.push_back(randomStr);
    }
    
    std::unordered_set<std::string> seen;
    std::vector<std::string> unique;

    unique.reserve(result.size());

    for (const auto& s : result) {
        if (seen.insert(s).second) {
            unique.push_back(s);
        }
    }

    result = std::move(unique);
    return result;
};


int main() {
    std::vector<std::string> strings = generateRandomStrings(100000);
    Trie t;
    for (auto s : strings) {
        t.addWord(s);
    }
    t.initLambda();

    std::sort(strings.begin(), strings.end());
    size_t h = 0;
    for (auto s : strings) {
        assert(h == t.hash(s));
        h++;
    }



    return 0;
}