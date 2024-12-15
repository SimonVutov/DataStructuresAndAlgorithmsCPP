/************************************************************
 * advanced_data_structures.cpp
 *
 * A single-file codebase illustrating advanced data structures:
 *  1) Self-Balancing BSTs (AVL, Splay, Red-Black, etc.)
 *  2) Disjoint Set (Union-Find)
 *  3) Trie (prefix tree)
 *  4) Segment Tree + Lazy Propagation stub
 *  5) Fenwick Tree (Binary Indexed Tree)
 *  6) Suffix Array (simplified)
 *  7) Suffix Tree (stub)
 *  8) K-D Tree (skeleton)
 *  9) Other DS stubs: Treap, B-Tree, Interval Tree, Palindromic Tree, etc.
 * 10) Advanced Linked Lists: XOR, SkipList stubs
 * 11) N-ary Trees
 *
 * Each data structure is condensed to show main logic or stubs.
 * For real production usage, these structures would require more
 * thorough implementations and testing.
 ************************************************************/

#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <queue>
#include <string>
#include <limits>
#include <memory>

/**************************************
 * 1) SELF-BALANCING BSTs
 **************************************/

//============== AVL TREE (Simplified) ===================
struct AVLNode {
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

int avlHeight(AVLNode* n) {
    return n ? n->height : 0;
}

int avlBalanceFactor(AVLNode* n) {
    return n ? avlHeight(n->left) - avlHeight(n->right) : 0;
}

AVLNode* avlRightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = std::max(avlHeight(y->left), avlHeight(y->right)) + 1;
    x->height = std::max(avlHeight(x->left), avlHeight(x->right)) + 1;
    return x;
}

AVLNode* avlLeftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = std::max(avlHeight(x->left), avlHeight(x->right)) + 1;
    y->height = std::max(avlHeight(y->left), avlHeight(y->right)) + 1;
    return y;
}

AVLNode* avlInsert(AVLNode* root, int key) {
    if (!root) return new AVLNode(key);
    if (key < root->key) {
        root->left = avlInsert(root->left, key);
    } else if (key > root->key) {
        root->right = avlInsert(root->right, key);
    } else {
        // duplicate keys: optional logic
        return root;
    }
    root->height = 1 + std::max(avlHeight(root->left), avlHeight(root->right));

    int balance = avlBalanceFactor(root);

    // Left Left
    if (balance > 1 && key < root->left->key) {
        return avlRightRotate(root);
    }
    // Right Right
    if (balance < -1 && key > root->right->key) {
        return avlLeftRotate(root);
    }
    // Left Right
    if (balance > 1 && key > root->left->key) {
        root->left = avlLeftRotate(root->left);
        return avlRightRotate(root);
    }
    // Right Left
    if (balance < -1 && key < root->right->key) {
        root->right = avlRightRotate(root->right);
        return avlLeftRotate(root);
    }
    return root;
}

// Deletion is more involved, omitted for brevity

void avlPreOrder(AVLNode* root) {
    if (!root) return;
    std::cout << root->key << " ";
    avlPreOrder(root->left);
    avlPreOrder(root->right);
}

//============== SPLAY TREE (Skeleton) ===================

struct SplayNode {
    int key;
    SplayNode *left, *right;
    SplayNode(int k) : key(k), left(nullptr), right(nullptr) {}
};

// Splay function: move key to root if found
// (Implementation detail is large, here is a stub)
SplayNode* splay(SplayNode* root, int key) {
    if (!root || root->key == key) return root;
    // *Real splay logic with rotations not fully shown*
    return root; // stub
}

// Insert: splay the tree around the newly inserted node
SplayNode* splayInsert(SplayNode* root, int key) {
    if (!root) return new SplayNode(key);
    // Do normal BST insert, then splay
    if (key < root->key) {
        root->left = splayInsert(root->left, key);
    } else if (key > root->key) {
        root->right = splayInsert(root->right, key);
    }
    root = splay(root, key);
    return root;
}

// Searching also uses splay. Deletions also exist, omitted for brevity.

//============== B-TREE (Skeleton) ===================
/* 
   Typical B-Tree node contains multiple keys, children, etc.
   Implementation is quite extensive. 
   We'll demonstrate a minimal structure & stub:
*/
static const int T = 3; // B-Tree Minimum degree (Example: T=3 => max 2T-1=5 keys)

struct BTreeNode {
    int keys[2*T-1];
    BTreeNode* children[2*T];
    int n;
    bool leaf;
    BTreeNode(bool isLeaf) : n(0), leaf(isLeaf) {
        for(int i=0; i<2*T; i++) children[i] = nullptr;
    }
};

struct BTree {
    BTreeNode* root;
    BTree() : root(nullptr) {}
    void insert(int key) {
        // Full B-tree insertion logic omitted for brevity.
        // Typically: if root is full, split root, else insertNonFull.
    }
};

//============== RED-BLACK TREE (Skeleton) ===================
// Real RBT code is quite large. Minimal stubs:
enum Color { RED, BLACK };

struct RBNode {
    int key;
    Color color;
    RBNode *left, *right, *parent;
    RBNode(int k) : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

// RBT Insertion or Deletion is large. We provide stubs:
struct RBTree {
    RBNode* root;
    RBTree() : root(nullptr) {}
    void insert(int key) {
        // Insert BST style, then fix violations
    }
    void remove(int key) {
        // remove node if found, fix color violations
    }
};

//============== SCAPEGOAT TREE & TREAP (Skeleton) ===============
/* Scapegoat Tree uses partial rebuilds to balance.  Treap randomizes priority. */

// Treap node
struct TreapNode {
    int key, priority;
    TreapNode* left;
    TreapNode* right;
    TreapNode(int k) : key(k), priority(std::rand()), left(nullptr), right(nullptr) {}
};

TreapNode* treapRotateRight(TreapNode* y) {
    TreapNode* x = y->left;
    y->left = x->right;
    x->right = y;
    return x;
}

TreapNode* treapRotateLeft(TreapNode* x) {
    TreapNode* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

TreapNode* treapInsert(TreapNode* root, int key) {
    if (!root) return new TreapNode(key);
    if (key < root->key) {
        root->left = treapInsert(root->left, key);
        if (root->left && root->left->priority > root->priority) {
            root = treapRotateRight(root);
        }
    } else {
        root->right = treapInsert(root->right, key);
        if (root->right && root->right->priority > root->priority) {
            root = treapRotateLeft(root);
        }
    }
    return root;
}

/**************************************
 * 2) DISJOINT SET (Union-Find)
 **************************************/
class UnionFind {
private:
    std::vector<int> parent, rank;

public:
    UnionFind(int n) : parent(n), rank(n,0) {
        for(int i=0; i<n; i++) parent[i]=i;
    }
    int find(int x) {
        if(parent[x]!=x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int a, int b) {
        int ra=find(a), rb=find(b);
        if(ra==rb) return;
        if(rank[ra]<rank[rb]) parent[ra]=rb;
        else if(rank[rb]<rank[ra]) parent[rb]=ra;
        else { parent[rb]=ra; rank[ra]++; }
    }
    bool sameSet(int a,int b) {
        return find(a)==find(b);
    }
};

/**************************************
 * 3) TRIE (Prefix Tree) 
 **************************************/
struct TrieNode {
    static const int ALPH = 26;
    TrieNode* children[ALPH];
    bool endOfWord;
    TrieNode() : endOfWord(false) {
        for(int i=0; i<ALPH; i++) children[i]=nullptr;
    }
};

class Trie {
private:
    TrieNode* root;
    int charToIndex(char c) { return c - 'a'; }
public:
    Trie() { root = new TrieNode(); }
    
    void insert(const std::string &key) {
        TrieNode* cur = root;
        for(char c : key){
            int idx = charToIndex(c);
            if(!cur->children[idx]) {
                cur->children[idx] = new TrieNode();
            }
            cur = cur->children[idx];
        }
        cur->endOfWord = true;
    }

    bool search(const std::string &key) {
        TrieNode* cur = root;
        for(char c : key){
            int idx = charToIndex(c);
            if(!cur->children[idx]) return false;
            cur=cur->children[idx];
        }
        return (cur && cur->endOfWord);
    }

    // Deletion, suffix-based operations, etc. omitted for brevity
};

/**************************************
 * 4) SEGMENT TREE (Range queries)
 **************************************/
class SegmentTree {
private:
    std::vector<int> seg;
    std::vector<int> lazy;
    int n;

    void build(const std::vector<int>& arr, int idx, int left, int right){
        if(left==right){
            seg[idx] = arr[left];
            return;
        }
        int mid=(left+right)/2;
        build(arr,idx*2,left,mid);
        build(arr,idx*2+1,mid+1,right);
        seg[idx]=seg[idx*2]+seg[idx*2+1];
    }

    void propagate(int idx, int left, int right){
        if(lazy[idx]!=0){
            seg[idx]+=(right-left+1)*lazy[idx]; 
            if(left<right){ // not a leaf
                lazy[idx*2]+=lazy[idx];
                lazy[idx*2+1]+=lazy[idx];
            }
            lazy[idx]=0;
        }
    }

    int rangeQuery(int idx, int left, int right, int ql, int qr){
        propagate(idx,left,right);
        if(right<ql || left>qr) return 0;
        if(ql<=left && right<=qr) {
            return seg[idx];
        }
        int mid=(left+right)/2;
        return rangeQuery(idx*2,left,mid,ql,qr)+
               rangeQuery(idx*2+1,mid+1,right,ql,qr);
    }

    void updateRange(int idx,int left,int right,int ql,int qr,int val){
        propagate(idx,left,right);
        if(right<ql || left>qr) return;
        if(ql<=left && right<=qr){
            lazy[idx]+=val;
            propagate(idx,left,right);
            return;
        }
        int mid=(left+right)/2;
        updateRange(idx*2,left,mid,ql,qr,val);
        updateRange(idx*2+1,mid+1,right,ql,qr,val);
        seg[idx]=seg[idx*2]+seg[idx*2+1];
    }

public:
    SegmentTree(int size){
        n=size;
        seg.resize(4*n,0);
        lazy.resize(4*n,0);
    }
    void build(const std::vector<int>& arr){
        build(arr,1,0,n-1);
    }
    int query(int l,int r){
        return rangeQuery(1,0,n-1,l,r);
    }
    void update(int l,int r,int val){
        updateRange(1,0,n-1,l,r,val);
    }
};

/**************************************
 * 5) BINARY INDEXED TREE (Fenwick Tree)
 **************************************/
class FenwickTree {
private:
    std::vector<int> fenw;
    int n;

public:
    FenwickTree(int size) : n(size), fenw(size+1, 0) {}

    void update(int i, int val) {
        for(; i<=n; i+=(i & -i)) {
            fenw[i]+=val;
        }
    }

    int prefixSum(int i) const {
        int res=0;
        for(; i>0; i-=(i & -i)) {
            res+=fenw[i];
        }
        return res;
    }

    int rangeSum(int l,int r){
        return prefixSum(r)-prefixSum(l-1);
    }
};

/**************************************
 * 6) SUFFIX ARRAY (Very Simplified)
 **************************************/
std::vector<int> buildSuffixArray(const std::string &s){
    // Very naive O(n^2 log n) approach for demonstration
    std::vector<int> sa(s.size());
    for(size_t i=0; i<sa.size(); i++) sa[i]=i;
    std::sort(sa.begin(), sa.end(),[&](int a,int b){
        return s.substr(a)<s.substr(b);
    });
    return sa;
}

/**************************************
 * 7) SUFFIX TREE (Stub)
 **************************************/
// Building a Suffix Tree (Ukkonen’s algorithm) is large. We'll provide a stub:
struct SuffixTreeNode {
    // Could store edges, substring intervals, children...
};
class SuffixTree {
public:
    SuffixTree(const std::string &s) {
        // Implement Ukkonen’s or other approach
    }
};

/**************************************
 * 8) K-D TREE (2D simplified example)
 **************************************/
struct KDNode {
    std::vector<double> point; // e.g. 2D or kD
    KDNode* left;
    KDNode* right;
    KDNode(std::vector<double> pt) : point(pt), left(nullptr), right(nullptr) {}
};

KDNode* kdInsert(KDNode* root, std::vector<double> point, int depth=0){
    if(!root) return new KDNode(point);
    int cd = depth % point.size(); // current dimension
    if(point[cd] < root->point[cd]) {
        root->left = kdInsert(root->left, point, depth+1);
    } else {
        root->right= kdInsert(root->right, point, depth+1);
    }
    return root;
}

/**************************************
 * 9) OTHER DATA STRUCTURES (Stub)
 **************************************/
// Palindromic Tree, Ternary Search Tree, Interval Tree, 
// BK-Tree, Cartesian Tree, SparseSet, Gomory-Hu Tree, Persistent DS, etc.
// We only show minimal placeholders below:

// Ternary Search Tree node
struct TSTNode {
    char data;
    bool endOfString;
    TSTNode* left;
    TSTNode* eq;
    TSTNode* right;
    TSTNode(char c) : data(c), endOfString(false), left(nullptr), eq(nullptr), right(nullptr) {}
};

// Interval Tree node skeleton
struct Interval {
    int low, high;
};
struct IntervalTreeNode {
    Interval interval;
    int maxHigh;
    IntervalTreeNode *left, *right;
    IntervalTreeNode(const Interval &i) : interval(i), maxHigh(i.high), left(nullptr), right(nullptr){}
};

// Palindromic Tree, BK-Tree, etc. also not shown in detail

/**************************************
 * 10) ADVANCED LINKED LISTS (Stubs)
 **************************************/
// XOR Linked List
// Skip List
// Self-Organizing List
// Unrolled Linked List
// 
// Each is quite large. Minimal placeholders follow:

// XOR LL node
struct XORNode {
    int val;
    XORNode* xorPtr; // xors next and prev
    XORNode(int v): val(v), xorPtr(nullptr) {}
};

// SkipList node
struct SkipNode {
    int key;
    std::vector<SkipNode*> forward; // multiple levels
    SkipNode(int k,int level) : key(k), forward(level+1,nullptr) {}
};

/**************************************
 * 11) N-ARY TREE
 **************************************/
struct NaryNode {
    int val;
    std::vector<NaryNode*> children;
    NaryNode(int v) : val(v) {}
};

void naryDFS(NaryNode* root) {
    if(!root) return;
    std::cout << root->val << " ";
    for(auto* child : root->children) {
        naryDFS(child);
    }
}

/**************************************
 * int main() - Basic Demo
 **************************************/
int main(){
    // 1) AVL Tree demo
    AVLNode* avlRoot = nullptr;
    avlRoot = avlInsert(avlRoot, 10);
    avlRoot = avlInsert(avlRoot, 20);
    avlRoot = avlInsert(avlRoot, 5);
    std::cout << "AVL Preorder: ";
    avlPreOrder(avlRoot); 
    std::cout << std::endl;

    // 2) Disjoint Set
    UnionFind uf(5);
    uf.unite(0,1);
    uf.unite(3,4);
    std::cout << "0 and 1 same set? " << (uf.sameSet(0,1) ? "Yes":"No") << std::endl;
    std::cout << "2 and 4 same set? " << (uf.sameSet(2,4) ? "Yes":"No") << std::endl;

    // 3) Trie
    Trie trie;
    trie.insert("hello");
    trie.insert("world");
    std::cout << "Trie search 'hello': " << trie.search("hello") << std::endl;

    // 4) Segment Tree
    std::vector<int> arr = {1, 2, 3, 4, 5};
    SegmentTree st(arr.size());
    st.build(arr);
    std::cout << "Segment Tree query(0,4): " << st.query(0,4) << std::endl;
    st.update(1,3,1); // add 1 to elements in range [1..3]
    std::cout << "Segment Tree query(0,4) after update: " << st.query(0,4) << std::endl;

    // 5) Fenwick Tree
    FenwickTree fenw(5);
    fenw.update(1,1);
    fenw.update(2,2);
    fenw.update(3,3);
    std::cout << "Fenw rangeSum(1,3): " << fenw.rangeSum(1,3) << std::endl;

    // 6) Suffix Array
    std::string s = "banana";
    auto sa = buildSuffixArray(s);
    std::cout << "Suffix Array for 'banana': ";
    for(auto x: sa) std::cout << x << " ";
    std::cout << std::endl;

    // 7) K-D Tree (2D example)
    KDNode* kdRoot = nullptr;
    kdRoot = kdInsert(kdRoot, {2.0,3.0});
    kdRoot = kdInsert(kdRoot, {5.0,4.0});
    kdRoot = kdInsert(kdRoot, {9.0,6.0});
    kdRoot = kdInsert(kdRoot, {4.0,7.0});
    std::cout << "K-D Tree build done (stub)" << std::endl;

    // 8) Treap
    TreapNode* treapRoot = nullptr;
    treapRoot = treapInsert(treapRoot, 50);
    treapRoot = treapInsert(treapRoot, 30);
    treapRoot = treapInsert(treapRoot, 70);
    std::cout << "Treap Insert done (stub)" << std::endl;

    // 9) N-ary Tree DFS
    NaryNode* rootN = new NaryNode(1);
    rootN->children.push_back(new NaryNode(2));
    rootN->children.push_back(new NaryNode(3));
    rootN->children.push_back(new NaryNode(4));
    std::cout << "N-ary Tree DFS: ";
    naryDFS(rootN);
    std::cout << std::endl;

    // (We skip code for Ternary Search Tree, Interval Tree, SkipList, etc. stubs)

    std::cout << "\nAll advanced data structures compiled successfully." << std::endl;
    return 0;
}