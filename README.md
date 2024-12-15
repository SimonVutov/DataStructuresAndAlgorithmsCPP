# C++ Data Structures and Algorithms

This repository contains two C++ files demonstrating a variety of data structures and algorithms ranging from **basic** to **advanced**.

- **basic_data_structures.cpp**  
  Implements fundamental data structures such as **linked lists**, **stacks**, **queues**, **sets**, **maps**, **priority queues**, **matrices**, **arrays**, and more. Includes a main function to demonstrate each container.

- **advanced_data_structures.cpp**  
  Showcases advanced data structures and algorithms: **AVL Trees**, **Splay Trees**, **Red-Black Trees**, **Disjoint Set (Union-Find)**, **Segment Trees**, **Fenwick Trees**, **Suffix Arrays**, **K-D Trees**, **N-ary Trees**, and additional stubs for B-Trees, Treaps, Tries, etc. Also includes a main function that performs basic testing.

---

## Instructions

### Compilation and Execution

Use **Clang** with C++17 support (GCC also works similarly):

**To Compile:**

clang++ -std=c++17 linkedList.cpp -o FILENAME

**To Run:**

./FILENAME

**Example:**

clang++ -std=c++17 linkedList.cpp -o basic_data_structures

./basic_data_structures

---

Files Overview

basic_data_structures.cpp

_Foundational data structures:_
- LinkedList (templated)
- CircularLinkedList
- Stack, Queue, PriorityQueue
- Set, Map, UnorderedMap
- Array, Matrix, Bitset, Deque, Vector, Span, etc.

advanced_data_structures.cpp

_Complex data structures:_

- Self-Balancing BSTs (AVL, Splay, Red-Black stubs)
- Disjoint Set (Union-Find)
- Tries (Prefix Trees)
- Segment Trees (with lazy propagation)
- Fenwick Trees (Binary Indexed Trees)
- Suffix Array (simplified) & Suffix Tree (stub)
- K-D Tree (2D version)
- Treap, B-Tree, Interval Tree stubs, etc.
- Advanced Linked Lists (XOR, SkipList) stubs
- N-ary Trees
