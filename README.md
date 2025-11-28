# advanced-algorithms

* [Level Ancestor](./LA) is the problem of answering queries of the type: **for a node in a tree v and a positive number k, return the ancestor of v on depth k in the tree**. There are efficient algorithms that create an index (data structure) which can efficiently answer such queries. I have implemented the algorithm which can be found in the [The Level Ancestor Problem simplified paper](./LA/LA.pdf).

* [Range Minimum Query](./RMQ) is the problem of answering queries of the type: **given a list of numbers L and indices in the list i, j, return the minimum of the subarray L[i, ..., j]**. In the [cpp](./RMQ) I have implemented an index structure that answers such queries efficiently when the list L satisfy the following property: for each i, abs(L[i] - L[i+1]) <= 1. This is also known as +-1RMQ and it has an interesting [connection](./RMQ/LCA.pdf) with another algorithmic problem called lowest common ancestor (LCA).

* [String Perfect Hash](./SPH) is the problem of creating a perfect hash that works for strings. The hash function must be computable in time linear with respect to the input string. In the [cpp](./SPH/SPH.cpp) I have implemented a data structure (automaton) that is created in time linear with respect to the static dictionary which then enables us to compute a hash function with the properties specified. 

* [Wagner-Fischer algorithm](./WFA) is considered by some the first dynamic programming algorithm. You can check the [implementation](./WHA/WHA.cpp) and the [paper](https://par.cse.nsysu.edu.tw/resource/lab_relative/Wagn74.pdf).