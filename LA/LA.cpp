#include <iostream>
#include <vector>
#include <random>
#include <deque>
#include <cassert>
#include <algorithm>

struct TreeNode {
    static int nextID;
    int ID;
    int value;
    std::vector<TreeNode*> children;

    TreeNode(int value) {
        ID = nextID;
        nextID++;
        this->value = value;
    }
    TreeNode(int value, std::vector<TreeNode*> children) {
        ID = nextID;
        nextID++;
        this->value = value;
        this->children = children;
    }
};
int TreeNode::nextID = 0;

struct Tree{
    TreeNode* root;
    size_t size;
    std::vector<size_t> heights;
    std::vector<TreeNode*> parents;
    std::vector<TreeNode*> pointers;
    std::vector<std::vector<size_t>> ladders;
    std::vector<size_t> depths;
    std::vector<size_t> vertexToLadder;
    std::vector<size_t> indexInLadder;

    Tree(TreeNode* root, size_t size) {

    }
};


int min(int a, int b) {
    if (a < b) return a;
    else       return b;
}

int boundedRand(int a, int b) {
    if (a == b) return a;
    int s = std::rand() % (b - a);
    return s + a;
}

void createTreeRec(size_t numNodes, TreeNode* root) {
    std::deque<TreeNode*> q;
    q.push_back(root);

    while(numNodes > 0) {
        TreeNode* v = q.front();
        q.pop_front();
        size_t c = boundedRand(1, min(3, numNodes));
        
        for (int i = 0; i < c; ++i) {
            TreeNode* child = new TreeNode(boundedRand(0, 100));
            v->children.push_back(child);
            q.push_back(child);
        }

        numNodes -=c;
    }
}

TreeNode* createTree(size_t numNodes) {
    TreeNode* root = new TreeNode(boundedRand(0, 100));
    numNodes--;
    createTreeRec(numNodes, root);
    return root;
}

void printTree(const TreeNode* node, const std::string& prefix = "", bool isLast = true) {
    if (!node) return;
    std::cout << prefix;
    std::cout << (isLast ? "└── " : "├── ");
    std::cout << "Node(ID=" << node->ID << ", value=" << node->value << ")\n";
    std::string newPrefix = prefix + (isLast ? "    " : "│   ");
    for (size_t i = 0; i < node->children.size(); ++i) {
        printTree(node->children[i], newPrefix, i == node->children.size() - 1);
    }
}

void getPointersRec(TreeNode* v, std::vector<TreeNode*>& pointers) {
    pointers[v->ID] = v;
    for (TreeNode* child : v ->children) {
        getPointersRec(child, pointers);
    }
}

std::vector<TreeNode*> getPointers(TreeNode* root, size_t treeSize) {
    std::vector<TreeNode*> pointers(treeSize);
    getPointersRec(root, pointers);
    return pointers;
}
namespace quadratic {

    void DFSIndex(TreeNode* v, std::vector<std::vector<size_t>>& index, std::vector<size_t>& path) {
        path.push_back(v->ID);
        index[v->ID] = path;

        for (TreeNode* child : v->children) {
            DFSIndex(child, index, path);
            path.pop_back();
        }
    }

    std::vector<std::vector<size_t>> createIndex(TreeNode* root, size_t treeSize) {
        std::vector<std::vector<size_t>> index(treeSize);
        std::vector<size_t> path;
        DFSIndex(root, index, path);
        return index;
    }

    size_t query(TreeNode* v, size_t d, const std::vector<std::vector<size_t>>& index) {
        if (d > index[v->ID].size() - 1) {
            throw std::invalid_argument("Invalid depth");
        }

        return index[v->ID][d];
    }

}

namespace jumps {

    size_t treeSize = 100;
    size_t logTreeSize = 7; // rounded up

    std::vector<size_t> pow = [] {
        std::vector<size_t> v(logTreeSize);
        size_t pow = 1;
        for (int i = 0; i < v.size(); ++i) {
            v[i] = pow;
            pow += 2;
        }
        return v;
    }();

    std::vector<size_t> logs = [] {
        std::vector<size_t> v(treeSize + 1);
        v[0] = -1; v[1] = 0;
        size_t log = 1, i = 2, nextPow = 4;
        while (i < treeSize + 1) {
            while(i < nextPow && i < treeSize + 1) {
                v[i] = log;
                ++i;
            }
            log++;
            nextPow *= 2;
        }
        return v;
    }();

    void DFSIndex(TreeNode* v, std::vector<std::vector<size_t>>& index, std::vector<size_t>& path) {
        path.push_back(v->ID);

        for (size_t pow = 1; pow <= path.size(); pow *= 2) {
            index[v->ID].push_back(path[path.size() - pow]);
        }

        for (TreeNode* child : v->children) {
            DFSIndex(child, index, path);
            path.pop_back();
        }
    }

    void DFSDepths(TreeNode* v, std::vector<size_t>& depths, size_t currDepth) {
        depths[v->ID] = currDepth;

        for (TreeNode* child : v->children) {
            DFSDepths(child, depths, currDepth + 1);
        }
    }

    std::vector<size_t> getDepths(TreeNode* root, size_t treeSize) {
        std::vector<size_t> depths(treeSize);
        DFSDepths(root, depths, 0);
        return depths;
    }

    std::vector<std::vector<size_t>> createIndex(TreeNode* root, size_t treeSize) {
        std::vector<std::vector<size_t>> index(treeSize);
        std::vector<size_t> path;
        DFSIndex(root, index, path);
        return index;
    }

    bool isPowerOf2(size_t n) {
    return n != 0 && (n & (n - 1)) == 0;
    }

    size_t query(TreeNode* v, size_t d, const std::vector<std::vector<size_t>>& index, const std::vector<size_t>& depths, const std::vector<TreeNode*>& pointers){
        if (depths[v->ID]  < d) {
            throw std::invalid_argument("Invalid depth");
        }
        if (depths[v->ID] == d) {
            return v->ID;
        }

        size_t dist = depths[v->ID] - d + 1;
        size_t jump = logs[dist];
        if (isPowerOf2(dist)) {
            return index[v->ID][jump];
        }
        else {
            TreeNode* next = pointers[index[v->ID][jump]];
            return query(next, d, index, depths, pointers);
        }
    }
    
}

namespace ladders {

    void getHeightsRec(TreeNode* v, std::vector<size_t>& heights) {
        if (v->children.size() == 0) {
            heights[v->ID] = 0;
            return;
        }

        for (TreeNode* child : v->children) {
            getHeightsRec(child, heights);
        }

        size_t maxHeights = 0;
        for (TreeNode* child : v->children) {
            maxHeights = std::max(maxHeights, heights[child->ID]);
        }
        heights[v->ID] = maxHeights + 1;
    }

    std::vector<size_t> getHeights(TreeNode* root, size_t treeSize) {
        std::vector<size_t> heights(treeSize);
        getHeightsRec(root, heights);
        return heights;
    }

    void findLongestPath(TreeNode* v, std::vector<size_t>& path, std::vector<TreeNode*>& roots, const std::vector<size_t>& heights) {
        path.push_back(v->ID);
        if (v->children.size() == 0) {
            return;
        }

        TreeNode* s;
        size_t maxHeight = 0;
        for (TreeNode* child : v->children) {
            if (heights[child->ID] >= maxHeight) {
                maxHeight = heights[child->ID];
                s = child;
            }
        }

        for (TreeNode* child : v->children) {
            if (child != s) {
                roots.push_back(child);
            }
        }

        findLongestPath(s, path, roots, heights);
    }   

    std::vector<std::vector<size_t>> splitTree(TreeNode* root, size_t treeSize) {
        std::vector<TreeNode*> roots;
        std::vector<std::vector<size_t>> paths;
        std::vector<size_t> heights = getHeights(root, treeSize);
        roots.push_back(root);
    
        while (!roots.empty()) {
            TreeNode* r = roots.back(); roots.pop_back();
            std::vector<size_t> path;
            findLongestPath(r, path, roots, heights);
            paths.push_back(path);
        }

        return paths;
    }

    void getParentsRec(TreeNode* v, std::vector<TreeNode*>& parents) {
        for (TreeNode* child : v->children) {
            parents[child->ID] = v;
            getParentsRec(child, parents);
        }
    }

    std::vector<TreeNode*> getParents(TreeNode* root, size_t treeSize) {
        std::vector<TreeNode*> parents(treeSize);
        parents[root->ID] = root;
        getParentsRec(root, parents);
        return parents;
    }

    void doubleLadders(std::vector<std::vector<size_t>>& ladders, const std::vector<TreeNode*>& parents, const std::vector<TreeNode*> pointers) {
        for (int i = 0; i < ladders.size(); ++i) {
            std::reverse(ladders[i].begin(), ladders[i].end());
            size_t n = ladders[i].size();
            TreeNode* v = parents[ladders[i][n-1]];

            for (int j = 0; j < n; ++j) {
                ladders[i].push_back(v->ID);
                if (parents[v->ID] == v) break;
                v = parents[v->ID];
            }
        }
    }

    std::vector<size_t> getVertexToLadder(const std::vector<std::vector<size_t>>& ladders, size_t treeSize) {
        std::vector<size_t> l(treeSize);
        for (size_t i = 0; i < ladders.size(); ++i) {
            for (size_t vID : ladders[i]) {
                l[vID] = i;
            }
        }
        return l;
    }

    std::vector<size_t> getVertexIndexInLadder(const std::vector<std::vector<size_t>>& ladders, size_t treeSize) {
        std::vector<size_t> indexInLadder(treeSize);
        for (size_t i = 0; i < ladders.size(); ++i) {
            for (size_t j = 0; j < ladders[i].size(); ++j) {
                indexInLadder[ladders[i][j]] = j;
            }
        }
        return indexInLadder;
    }

    size_t query(TreeNode*v, size_t d, size_t treeSize, const std::vector<std::vector<size_t>>& ladders, const std::vector<TreeNode*>& parents, const std::vector<size_t>& depths, const std::vector<size_t>& heights, const std::vector<size_t>& vertexToLadder, std::vector<size_t>& indexInLadder) {
        if (depths[v->ID] < d) {
            throw std::invalid_argument("Invalid query ");
        }
        if (depths[v->ID] == d) {
            return v->ID;
        }

        size_t l = vertexToLadder[v->ID];
        size_t dist = depths[v->ID] - d;
        if (indexInLadder[v->ID] + dist < ladders[l].size()) {
            return ladders[l][indexInLadder[v->ID] + dist];
        }
        else {

            return query(parents[ladders[l].back()], d, treeSize, ladders, parents, depths, heights, vertexToLadder, indexInLadder);
        }
    }   
}



int main() {
    size_t treeSize = 100000;
    TreeNode* root = createTree(treeSize);
    std::vector<TreeNode*> pointers = getPointers(root, treeSize);
    std::vector<std::vector<size_t>> index1 = quadratic::createIndex(root, treeSize);
    std::vector<std::vector<size_t>> jumps = jumps::createIndex(root, treeSize);
    std::vector<size_t> depths = jumps::getDepths(root, treeSize);
    std::vector<size_t> heights = ladders::getHeights(root, treeSize);
    std::vector<TreeNode*> parents = ladders::getParents(root, treeSize);
    std::vector<std::vector<size_t>> ladders = ladders::splitTree(root, treeSize);
    ladders::doubleLadders(ladders, parents, pointers);
    std::vector<size_t> indexInLadders = ladders::getVertexIndexInLadder(ladders, treeSize);
    std::vector<size_t> vertexToLadder = ladders::getVertexToLadder(ladders, treeSize);


    for (int i = 0; i < treeSize; ++i) {
        size_t d = boundedRand(0, depths[i]);
        size_t v1 = quadratic::query(pointers[i], d, index1);
        size_t v2 = ladders::query(pointers[i], d, treeSize, ladders, parents, depths, heights, vertexToLadder, indexInLadders);
        size_t v3 = jumps::query(pointers[i], d, jumps, depths, pointers);
        assert(v1 == v3);
        assert(v1 == v2);
    }
}