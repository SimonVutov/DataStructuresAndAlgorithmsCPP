#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <cstddef>   // for size_t
#include <utility>   // for std::pair

//===================================================
// Global operator<< for std::pair<A,B> 
//===================================================
template<typename A, typename B>
std::ostream &operator<<(std::ostream &os, const std::pair<A, B> &p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

//===================================================
// Forward declaration
//===================================================
template<typename T> class LinkedList;

//===================================================
// Node Class (templated)
//===================================================
template<typename T>
class Node {
private:
    T val;
    Node<T>* next;

public:
    Node(const T& val) : val(val), next(nullptr) {}
    T& getVal()             { return val; }
    void setVal(const T& v) { val = v; }
    Node<T>* getNext()      { return next; }
    void setNext(Node<T>* n){ next = n; }
};

//===================================================
// Templated LinkedList
//===================================================
template<typename T>
class LinkedList {
    static int totalLinkedLists;

protected:
    Node<T>* head;
    Node<T>* tail;

    // Return pointer to node at index n
    Node<T>* nodeAtIndex(std::size_t n) {
        Node<T>* temp = head;
        for (std::size_t i = 0; i < n; i++) {
            if (!temp) throw std::out_of_range("Index out of range");
            temp = temp->getNext();
        }
        return temp;
    }

    // Merge two sorted singly linked lists (for mergesort)
    Node<T>* mergeLists(Node<T>* left, Node<T>* right) {
        if (!left)  return right;
        if (!right) return left;
        if (left->getVal() <= right->getVal()) {
            left->setNext(mergeLists(left->getNext(), right));
            return left;
        } else {
            right->setNext(mergeLists(left, right->getNext()));
            return right;
        }
    }

    // Recursively splits the list for mergesort
    Node<T>* mergeSort(Node<T>* start) {
        if (!start || !start->getNext()) return start;
        // Split the list in the middle
        Node<T>* slow = start;
        Node<T>* fast = start->getNext();
        while (fast && fast->getNext()) {
            slow = slow->getNext();
            fast = fast->getNext()->getNext();
        }
        Node<T>* secondHalf = slow->getNext();
        slow->setNext(nullptr);

        Node<T>* left  = mergeSort(start);
        Node<T>* right = mergeSort(secondHalf);
        return mergeLists(left, right);
    }

    // Add content of LinkedList 'l' to 'this' at end
    void add(LinkedList<T>* l) {
        if (!this->head) {
            this->head = l->head;
            this->tail = l->tail;
        } else {
            this->tail->setNext(l->head);
            if (l->tail) this->tail = l->tail;
        }
        // Clear the other list
        l->head = nullptr;
        l->tail = nullptr;
    }

public:
    LinkedList() : head(nullptr), tail(nullptr) { totalLinkedLists++; }

    LinkedList(std::initializer_list<T> init_list) : head(nullptr), tail(nullptr) {
        for (const T& val : init_list) {
            push(val);
        }
        totalLinkedLists++;
    }

    virtual ~LinkedList() {
        totalLinkedLists--;
        clear();
    }

    void clear() {
        Node<T>* current = head;
        while (current) {
            Node<T>* tmp = current;
            current = current->getNext();
            delete tmp;
        }
        head = nullptr;
        tail = nullptr;
    }

    std::size_t size() const {
        std::size_t count = 0;
        Node<T>* temp = head;
        while (temp) {
            count++;
            temp = temp->getNext();
        }
        return count;
    }

    bool empty() const {
        return (head == nullptr);
    }

    virtual void push(const T& val) {
        if (!head) {
            head = new Node<T>(val);
            tail = head;
            return;
        }
        tail->setNext(new Node<T>(val));
        tail = tail->getNext();
    }

    virtual T pop() {
        // pop from the back
        if (!head) {
            throw std::underflow_error("LinkedList is empty, cannot pop.");
        }
        if (head == tail) {
            T returnVal = head->getVal();
            delete head;
            head = nullptr;
            tail = nullptr;
            return returnVal;
        }
        // more than one node
        Node<T>* temp = head;
        while (temp->getNext() != tail) {
            temp = temp->getNext();
        }
        T returnVal = tail->getVal();
        delete tail;
        tail = temp;
        tail->setNext(nullptr);
        return returnVal;
    }

    virtual T pop_front() {
        if (!head) {
            throw std::underflow_error("LinkedList is empty, cannot pop_front.");
        }
        Node<T>* temp = head;
        T returnVal = head->getVal();
        if (head == tail) {
            head = nullptr;
            tail = nullptr;
        } else {
            head = head->getNext();
        }
        delete temp;
        return returnVal;
    }

    T& front() const {
        if (!head) throw std::underflow_error("LinkedList is empty, cannot view front.");
        return head->getVal();
    }

    T& back() const {
        if (!tail) throw std::underflow_error("LinkedList is empty, cannot view back.");
        return tail->getVal();
    }

    void sort() {
        // mergesort
        head = mergeSort(head);
        // reassign tail
        if (!head) {
            tail = nullptr;
        } else {
            Node<T>* temp = head;
            while (temp->getNext()) temp = temp->getNext();
            tail = temp;
        }
    }

    virtual void print() {
        Node<T>* temp = head;
        while (temp) {
            std::cout << temp->getVal(); // uses operator<< for T
            temp = temp->getNext();
            if (temp) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    T valAtIndex(std::size_t n) {
        Node<T>* temp = nodeAtIndex(n);
        return temp->getVal();
    }

    int find(const T& x) {
        Node<T>* temp = head;
        int idx = 0;
        while (temp) {
            if (temp->getVal() == x) return idx;
            temp = temp->getNext();
            idx++;
        }
        return -1;
    }

    void popAtIndex(std::size_t n) {
        if (n == 0) {
            pop_front();
            return;
        }
        Node<T>* prev = nodeAtIndex(n - 1);
        Node<T>* toDelete = prev->getNext();
        if (!toDelete) throw std::out_of_range("Index out of range in popAtIndex");
        if (toDelete == tail) tail = prev;
        prev->setNext(toDelete->getNext());
        delete toDelete;
    }

    // Overload operator[]
    Node<T>* operator[](std::size_t n) {
        return nodeAtIndex(n);
    }

    // Overload + operator (concatenate two linked lists)
    LinkedList<T>* operator+(LinkedList<T>* other) {
        LinkedList<T>* res = new LinkedList<T>();
        res->add(this);   // add content of this
        res->add(other);  // add content of other
        return res;
    }

    // Overload == operator
    bool operator==(LinkedList<T>* other) {
        if (size() != other->size()) return false;
        Node<T>* t1 = head;
        Node<T>* t2 = other->head;
        while (t1 && t2) {
            if (t1->getVal() != t2->getVal()) return false;
            t1 = t1->getNext();
            t2 = t2->getNext();
        }
        return true;
    }

    // Overload != operator
    bool operator!=(LinkedList<T>* other) {
        return !(*this == other);
    }
};

template<typename T>
int LinkedList<T>::totalLinkedLists = 0;

//===================================================
// Templated Set Class
//===================================================
template<typename T>
class Set : public LinkedList<T> {
public:
    Set() : LinkedList<T>() {}
    Set(std::initializer_list<T> init_list) : LinkedList<T>() {
        for (const T& val : init_list) {
            add(val);
        }
    }
    void add(const T& val) {
        if (this->find(val) == -1) {
            this->push(val);
        }
    }
    void remove(const T& val) {
        int idx = this->find(val);
        if (idx != -1) this->popAtIndex(idx);
    }
    bool contains(const T& val) {
        return (this->find(val) != -1);
    }
    void print() override {
        std::cout << "Set: ";
        LinkedList<T>::print();
    }
};

//===================================================
// Templated Stack Class
//===================================================
template<typename T>
class Stack : public LinkedList<T> {
public:
    Stack() : LinkedList<T>() {}
    void push(const T& val) override {
        // push at the back
        LinkedList<T>::push(val);
    }
    T pop() override {
        // pop from back
        return LinkedList<T>::pop();
    }
    T& top() {
        return this->back();
    }
    bool empty() const {
        return LinkedList<T>::empty();
    }
    void print() override {
        std::cout << "Stack (top->bottom): ";
        LinkedList<T>::print();
    }
};

//===================================================
// Templated Queue Class
//===================================================
template<typename T>
class Queue : public LinkedList<T> {
public:
    Queue() : LinkedList<T>() {}
    void push(const T& val) override {
        LinkedList<T>::push(val);
    }
    T pop() override {
        return LinkedList<T>::pop_front();
    }
    T& front() {
        return LinkedList<T>::front();
    }
    bool empty() const {
        return LinkedList<T>::empty();
    }
    T& back() {
        return LinkedList<T>::back();
    }
    void print() override {
        std::cout << "Queue (front->back): ";
        LinkedList<T>::print();
    }
};

//===================================================
// Templated PriorityQueue Class
//===================================================
template<typename T>
class PriorityQueue : public LinkedList<T> {
public:
    PriorityQueue() : LinkedList<T>() {}

    void push(const T& val) override {
        if (this->empty()) {
            LinkedList<T>::push(val);
            return;
        }
        Node<T>* temp = this->head;
        Node<T>* prev = nullptr;
        while (temp && temp->getVal() < val) {
            prev = temp;
            temp = temp->getNext();
        }
        Node<T>* newNode = new Node<T>(val);
        if (!prev) {
            // Insert at head
            newNode->setNext(this->head);
            this->head = newNode;
            if (!this->tail) this->tail = newNode;
        } else {
            newNode->setNext(temp);
            prev->setNext(newNode);
            if (!temp) this->tail = newNode;
        }
    }

    T pop() override {
        // pop the largest element (which is at the back)
        return LinkedList<T>::pop();
    }

    T& top() {
        // The largest is at the back
        return this->back();
    }

    void print() override {
        std::cout << "PriorityQueue (smallest->largest): ";
        LinkedList<T>::print();
    }
};

//===================================================
// Templated CircularLinkedList
//===================================================
template<typename T>
class CircularLinkedList : public LinkedList<T> {
public:
    CircularLinkedList() : LinkedList<T>() {}
    ~CircularLinkedList() {
        if (!this->head) return;
        Node<T>* temp = this->head;
        do {
            Node<T>* toDelete = temp;
            temp = temp->getNext();
            delete toDelete;
        } while (temp != this->head);

        this->head = nullptr;
        this->tail = nullptr;
    }

    void push(const T& val) override {
        if (!this->head) {
            this->head = new Node<T>(val);
            this->head->setNext(this->head);
            this->tail = this->head;
            return;
        }
        Node<T>* newNode = new Node<T>(val);
        this->tail->setNext(newNode);
        newNode->setNext(this->head);
        this->tail = newNode;
    }

    T pop() override {
        if (!this->head) {
            throw std::underflow_error("CircularLinkedList is empty, cannot pop.");
        }
        if (this->head == this->tail) {
            T returnVal = this->head->getVal();
            delete this->head;
            this->head = nullptr;
            this->tail = nullptr;
            return returnVal;
        }
        Node<T>* temp = this->head;
        while (temp->getNext() != this->tail) {
            temp = temp->getNext();
        }
        T returnVal = this->tail->getVal();
        delete this->tail;
        this->tail = temp;
        this->tail->setNext(this->head);
        return returnVal;
    }

    void print() override {
        if (!this->head) {
            std::cout << "(empty circular list)" << std::endl;
            return;
        }
        std::cout << "CircularLinkedList: ";
        Node<T>* temp = this->head;
        do {
            std::cout << temp->getVal();
            temp = temp->getNext();
            if (temp != this->head) std::cout << ", ";
        } while (temp != this->head);
        std::cout << std::endl;
    }
};

//===================================================
// Templated Map (linked-list based, storing key-value pairs)
//===================================================
template<typename K, typename V>
class Map : public LinkedList<std::pair<K, V>> {
public:
    Map() : LinkedList<std::pair<K, V>>() {}

    void insert(const K& key, const V& val) {
        int idx = findKey(key);
        if (idx != -1) {
            // key exists, update
            Node<std::pair<K, V>>* temp = this->operator[](idx);
            temp->getVal().second = val;
        } else {
            this->push(std::make_pair(key, val));
        }
    }

    V get(const K& key) {
        Node<std::pair<K, V>>* temp = this->head;
        while (temp) {
            if (temp->getVal().first == key) {
                return temp->getVal().second;
            }
            temp = temp->getNext();
        }
        throw std::out_of_range("Key not found in Map");
    }

    void remove(const K& key) {
        int idx = findKey(key);
        if (idx != -1) {
            this->popAtIndex(idx);
        }
    }

    bool contains(const K& key) {
        return (findKey(key) != -1);
    }

    int findKey(const K& key) {
        Node<std::pair<K, V>>* temp = this->head;
        int idx = 0;
        while (temp) {
            if (temp->getVal().first == key) return idx;
            temp = temp->getNext();
            idx++;
        }
        return -1;
    }

    void print() override {
        std::cout << "Map (key->value): ";
        Node<std::pair<K, V>>* temp = this->head;
        while (temp) {
            std::cout << temp->getVal(); 
            // uses operator<<(std::pair<K,V>) we defined globally
            temp = temp->getNext();
            if (temp) std::cout << ", ";
        }
        std::cout << std::endl;
    }
};

//===================================================
// UnorderedMap (Naive singly linked list chaining)
//===================================================
template<typename K, typename V>
class KeyValuePair {
public:
    K key;
    V value;
    KeyValuePair<K,V>* next;
    KeyValuePair(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
};

template<typename K, typename V>
class UnorderedMap {
private:
    KeyValuePair<K,V>* head;
public:
    UnorderedMap() : head(nullptr) {}
    ~UnorderedMap() {
        KeyValuePair<K,V>* temp = head;
        while (temp) {
            KeyValuePair<K,V>* toDel = temp;
            temp = temp->next;
            delete toDel;
        }
    }

    void insert(const K& key, const V& val) {
        KeyValuePair<K,V>* temp = head;
        while (temp) {
            if (temp->key == key) {
                temp->value = val;
                return;
            }
            temp = temp->next;
        }
        KeyValuePair<K,V>* newNode = new KeyValuePair<K,V>(key, val);
        newNode->next = head;
        head = newNode;
    }

    V get(const K& key) {
        KeyValuePair<K,V>* temp = head;
        while (temp) {
            if (temp->key == key) {
                return temp->value;
            }
            temp = temp->next;
        }
        throw std::out_of_range("Key not found in UnorderedMap");
    }

    void remove(const K& key) {
        KeyValuePair<K,V>* temp = head;
        KeyValuePair<K,V>* prev = nullptr;
        while (temp) {
            if (temp->key == key) {
                if (!prev) head = temp->next;
                else prev->next = temp->next;
                delete temp;
                return;
            }
            prev = temp;
            temp = temp->next;
        }
    }

    bool contains(const K& key) {
        KeyValuePair<K,V>* temp = head;
        while (temp) {
            if (temp->key == key) return true;
            temp = temp->next;
        }
        return false;
    }

    void print() {
        std::cout << "UnorderedMap: ";
        KeyValuePair<K,V>* temp = head;
        while (temp) {
            std::cout << "(" << temp->key << ", " << temp->value << ")";
            temp = temp->next;
            if (temp) std::cout << ", ";
        }
        std::cout << std::endl;
    }
};

//===================================================
// Matrix (templated 2D array)
//===================================================
template<typename T>
class Matrix {
public:
    Matrix(std::size_t rows, std::size_t cols)
    : m_(rows), n_(cols) {
        data_ = new T[m_ * n_];
        for (std::size_t i = 0; i < m_ * n_; i++) {
            data_[i] = T();
        }
    }

    Matrix(const Matrix &orig) : m_(orig.m_), n_(orig.n_) {
        data_ = new T[m_ * n_];
        for (std::size_t i = 0; i < m_ * n_; i++) {
            data_[i] = orig.data_[i];
        }
    }

    ~Matrix() {
        delete[] data_;
        data_ = nullptr;
    }

    T &operator()(std::size_t r, std::size_t c) {
        if (r >= m_ || c >= n_) throw std::out_of_range("Matrix index out of range");
        return data_[r * n_ + c];
    }

    Matrix &operator*=(const T& scalar) {
        for (std::size_t i = 0; i < m_ * n_; i++) {
            data_[i] *= scalar;
        }
        return *this;
    }

    void print() {
        std::cout << "Matrix " << m_ << "x" << n_ << ":\n";
        for (std::size_t r = 0; r < m_; r++) {
            for (std::size_t c = 0; c < n_; c++) {
                std::cout << data_[r * n_ + c] << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    std::size_t m_;
    std::size_t n_;
    T* data_;
};

//===================================================
// Templated Dynamic Array
//===================================================
template<typename T>
class Array {
public:
    Array(std::size_t cap)
    : cap_(cap) {
        arr_ = new T[cap_];
        for (std::size_t i = 0; i < cap_; i++) {
            arr_[i] = T();
        }
    }

    Array(const Array &orig) : cap_(orig.cap_) {
        arr_ = new T[cap_];
        for (std::size_t i = 0; i < cap_; i++) {
            arr_[i] = orig.arr_[i];
        }
    }

    ~Array() {
        delete[] arr_;
        arr_ = nullptr;
    }

    void interpolate() {
        std::size_t oldCap = cap_;
        cap_ *= 2;
        T *newArr = new T[cap_];
        for (std::size_t i = 0; i < cap_; i++) {
            newArr[i] = arr_[i / 2];
        }
        delete[] arr_;
        arr_ = newArr;
    }

    std::size_t capacity() const { return cap_; }

    void print() {
        std::cout << "Array: ";
        for (std::size_t i = 0; i < cap_; i++) {
            std::cout << arr_[i];
            if (i < cap_ - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }

private:
    std::size_t cap_;
    T* arr_;
};

//===================================================
// Additional Minimal "Standard-Like" Containers
//===================================================

//---------------------------------------------------
// Bitset<N>
template<std::size_t N>
class Bitset {
private:
    static const std::size_t BITS_PER_BLOCK = 8 * sizeof(unsigned long long);
    unsigned long long block;

public:
    Bitset() : block(0ULL) {
        static_assert(N <= BITS_PER_BLOCK, "Bitset dimension exceeds block size in this example.");
    }
    void set(std::size_t pos) {
        if (pos >= N) throw std::out_of_range("Bitset index out of range");
        block |= (1ULL << pos);
    }
    void reset(std::size_t pos) {
        if (pos >= N) throw std::out_of_range("Bitset index out of range");
        block &= ~(1ULL << pos);
    }
    bool test(std::size_t pos) const {
        if (pos >= N) throw std::out_of_range("Bitset index out of range");
        return (block & (1ULL << pos)) != 0ULL;
    }
    void print() {
        std::cout << "Bitset<" << N << ">: ";
        for (std::size_t i = 0; i < N; i++) {
            std::cout << (test(i) ? 1 : 0);
        }
        std::cout << std::endl;
    }
};

//---------------------------------------------------
// Deque<T> - ring buffer
template<typename T>
class Deque {
private:
    T* data;
    std::size_t capacity;
    std::size_t frontIdx;
    std::size_t backIdx;
    std::size_t count;

public:
    Deque(std::size_t cap = 16)
    : capacity(cap), frontIdx(0), backIdx(0), count(0) {
        data = new T[capacity];
    }
    ~Deque() { delete[] data; }

    bool empty() const { return count == 0; }
    std::size_t size() const { return count; }

    void push_back(const T& val) {
        if (count == capacity) throw std::overflow_error("Deque is full");
        data[backIdx] = val;
        backIdx = (backIdx + 1) % capacity;
        count++;
    }

    void push_front(const T& val) {
        if (count == capacity) throw std::overflow_error("Deque is full");
        frontIdx = (frontIdx == 0) ? capacity - 1 : frontIdx - 1;
        data[frontIdx] = val;
        count++;
    }

    T pop_back() {
        if (empty()) throw std::underflow_error("Deque is empty");
        backIdx = (backIdx == 0) ? capacity - 1 : backIdx - 1;
        T val = data[backIdx];
        count--;
        return val;
    }

    T pop_front() {
        if (empty()) throw std::underflow_error("Deque is empty");
        T val = data[frontIdx];
        frontIdx = (frontIdx + 1) % capacity;
        count--;
        return val;
    }

    T& front() {
        if (empty()) throw std::underflow_error("Deque is empty");
        return data[frontIdx];
    }

    T& back() {
        if (empty()) throw std::underflow_error("Deque is empty");
        std::size_t idx = (backIdx == 0) ? capacity - 1 : backIdx - 1;
        return data[idx];
    }

    void print() {
        std::cout << "Deque: ";
        for (std::size_t i = 0; i < count; i++) {
            std::size_t idx = (frontIdx + i) % capacity;
            std::cout << data[idx];
            if (i < count - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
};

//---------------------------------------------------
// ForwardList<T>
template<typename T>
class ForwardList {
private:
    Node<T>* head;

public:
    ForwardList() : head(nullptr) {}
    ~ForwardList() {
        Node<T>* temp = head;
        while (temp) {
            Node<T>* toDel = temp;
            temp = temp->getNext();
            delete toDel;
        }
    }

    void push_front(const T& val) {
        Node<T>* newNode = new Node<T>(val);
        newNode->setNext(head);
        head = newNode;
    }

    T pop_front() {
        if (!head) throw std::underflow_error("ForwardList is empty");
        Node<T>* temp = head;
        T retVal = temp->getVal();
        head = head->getNext();
        delete temp;
        return retVal;
    }

    bool empty() const { return head == nullptr; }

    void print() {
        std::cout << "ForwardList: ";
        Node<T>* temp = head;
        while (temp) {
            std::cout << temp->getVal();
            temp = temp->getNext();
            if (temp) std::cout << ", ";
        }
        std::cout << std::endl;
    }
};

//---------------------------------------------------
// Vector<T> - dynamic array
template<typename T>
class Vector {
private:
    T* data;
    std::size_t capacity;
    std::size_t length;

    void reallocate() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (std::size_t i = 0; i < length; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    Vector(std::size_t cap = 4)
    : capacity(cap), length(0) {
        data = new T[capacity];
    }

    ~Vector() { delete[] data; }

    void push_back(const T& val) {
        if (length == capacity) {
            reallocate();
        }
        data[length++] = val;
    }

    T pop_back() {
        if (length == 0) throw std::underflow_error("Vector empty");
        return data[--length];
    }

    T& operator[](std::size_t idx) {
        if (idx >= length) throw std::out_of_range("Vector index out of range");
        return data[idx];
    }

    std::size_t size() const { return length; }

    void print() {
        std::cout << "Vector: ";
        for (std::size_t i = 0; i < length; i++) {
            std::cout << data[i];
            if (i < length - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
};

//---------------------------------------------------
// Span<T> - pointer + size
template<typename T>
class Span {
private:
    T* ptr;
    std::size_t length;

public:
    Span(T* p, std::size_t len) : ptr(p), length(len) {}
    T& operator[](std::size_t idx) {
        if (idx >= length) throw std::out_of_range("Span index out of range");
        return ptr[idx];
    }
    std::size_t size() const { return length; }
    void print() {
        std::cout << "Span: ";
        for (std::size_t i = 0; i < length; i++) {
            std::cout << ptr[i];
            if (i < length - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
};

//---------------------------------------------------
// FlatSet<T> 
template<typename T>
class FlatSet {
private:
    Vector<T> vec;

public:
    void insert(const T& val) {
        // Insert in sorted order if not present
        for (std::size_t i = 0; i < vec.size(); i++) {
            if (vec[i] == val) return;   // already present
            if (vec[i] > val) {
                vec.push_back(val); // push at end
                // bubble up
                for (std::size_t j = vec.size() - 1; j > i; j--) {
                    T tmp = vec[j];
                    vec[j] = vec[j - 1];
                    vec[j - 1] = tmp;
                }
                return;
            }
        }
        vec.push_back(val);  // insert at end
    }

    bool contains(const T& val) {
        // naive linear search
        for (std::size_t i = 0; i < vec.size(); i++) {
            if (vec[i] == val) return true;
            if (vec[i] > val) return false;
        }
        return false;
    }

    void print() {
        std::cout << "FlatSet: ";
        vec.print();
    }
};

//---------------------------------------------------
// FlatMap<K,V> 
template<typename K, typename V>
class FlatMap {
private:
    Vector<K> keys;
    Vector<V> vals;

public:
    void insert(const K& key, const V& val) {
        // Keep keys sorted, maintain parallel 'vals'
        for (std::size_t i = 0; i < keys.size(); i++) {
            if (keys[i] == key) {
                vals[i] = val; // update
                return;
            }
            if (keys[i] > key) {
                keys.push_back(key);
                vals.push_back(val);
                // bubble up
                for (std::size_t j = keys.size() - 1; j > i; j--) {
                    K tempK = keys[j];
                    V tempV = vals[j];
                    keys[j]   = keys[j - 1];
                    vals[j]   = vals[j - 1];
                    keys[j-1] = tempK;
                    vals[j-1] = tempV;
                }
                return;
            }
        }
        // append at end
        keys.push_back(key);
        vals.push_back(val);
    }

    bool contains(const K& key) {
        for (std::size_t i = 0; i < keys.size(); i++) {
            if (keys[i] == key) return true;
            if (keys[i] > key) return false;
        }
        return false;
    }

    V get(const K& key) {
        for (std::size_t i = 0; i < keys.size(); i++) {
            if (keys[i] == key) return vals[i];
            if (keys[i] > key) break;
        }
        throw std::out_of_range("Key not found in FlatMap");
    }

    void print() {
        std::cout << "FlatMap: ";
        for (std::size_t i = 0; i < keys.size(); i++) {
            std::cout << "(" << keys[i] << "->" << vals[i] << ")";
            if (i < keys.size()-1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
};

//---------------------------------------------------
// InplaceVector<T, CAP>
template<typename T, std::size_t CAP>
class InplaceVector {
private:
    T arr[CAP];
    std::size_t length;

public:
    InplaceVector() : length(0) {}
    void push_back(const T& val) {
        if (length >= CAP) throw std::overflow_error("InplaceVector full");
        arr[length++] = val;
    }
    T pop_back() {
        if (length == 0) throw std::underflow_error("InplaceVector empty");
        return arr[--length];
    }
    T& operator[](std::size_t idx) {
        if (idx >= length) throw std::out_of_range("InplaceVector index out of range");
        return arr[idx];
    }
    std::size_t size() const { return length; }

    void print() {
        std::cout << "InplaceVector: ";
        for (std::size_t i = 0; i < length; i++) {
            std::cout << arr[i];
            if (i < length - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
};

//---------------------------------------------------
// MdSpan<T> 
template<typename T>
class MdSpan {
private:
    T* data;
    std::size_t rows;
    std::size_t cols;

public:
    MdSpan(T* ptr, std::size_t r, std::size_t c)
    : data(ptr), rows(r), cols(c) {}

    T& operator()(std::size_t i, std::size_t j) {
        if (i >= rows || j >= cols) throw std::out_of_range("MdSpan index out of range");
        return data[i*cols + j];
    }
    void print() {
        std::cout << "MdSpan " << rows << "x" << cols << ":\n";
        for (std::size_t i = 0; i < rows; i++) {
            for (std::size_t j = 0; j < cols; j++) {
                std::cout << data[i*cols + j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

//---------------------------------------------------
// UnorderedSet<T>
template<typename T>
class UnorderedSet {
private:
    Node<T>* head;

public:
    UnorderedSet() : head(nullptr) {}
    ~UnorderedSet() {
        Node<T>* temp = head;
        while (temp) {
            Node<T>* toDel = temp;
            temp = temp->getNext();
            delete toDel;
        }
    }

    bool contains(const T& val) {
        Node<T>* temp = head;
        while (temp) {
            if (temp->getVal() == val) return true;
            temp = temp->getNext();
        }
        return false;
    }
    void insert(const T& val) {
        if (contains(val)) return;
        Node<T>* newNode = new Node<T>(val);
        newNode->setNext(head);
        head = newNode;
    }
    void remove(const T& val) {
        Node<T>* temp = head;
        Node<T>* prev = nullptr;
        while (temp) {
            if (temp->getVal() == val) {
                if (!prev) head = temp->getNext();
                else prev->setNext(temp->getNext());
                delete temp;
                return;
            }
            prev = temp;
            temp = temp->getNext();
        }
    }
    void print() {
        std::cout << "UnorderedSet: ";
        Node<T>* temp = head;
        while (temp) {
            std::cout << temp->getVal();
            temp = temp->getNext();
            if (temp) std::cout << ", ";
        }
        std::cout << std::endl;
    }
};

//===================================================
// main() - Test everything
//===================================================
int main() {
    // 1) LinkedList<int>
    LinkedList<int> list{1, 2, 3, 5, 4};
    list.print();       // 1, 2, 3, 5, 4
    list.sort();
    list.print();       // 1, 2, 3, 4, 5

    // 2) CircularLinkedList<double>
    CircularLinkedList<double> clist;
    clist.push(10.5);
    clist.push(20.5);
    clist.push(30.5);
    clist.print();
    clist.pop();
    clist.print();

    // 3) Matrix<int>
    Matrix<int> mat(2,3);
    mat(0,0) = 1; mat(0,1) = 2; mat(0,2) = 3;
    mat(1,0) = 4; mat(1,1) = 5; mat(1,2) = 6;
    mat.print();

    // 4) Array<char>
    Array<char> arr(5);
    arr.print();
    arr.interpolate();
    arr.print();

    // 5) Stack<std::string>
    Stack<std::string> st;
    st.push("Hello");
    st.push("World");
    st.print();
    std::cout << "Stack pop: " << st.pop() << std::endl;
    st.print();

    // 6) Queue<int>
    Queue<int> q;
    q.push(100);
    q.push(200);
    q.print();
    q.pop();
    q.print();

    // 7) PriorityQueue<int>
    PriorityQueue<int> pq;
    pq.push(3);
    pq.push(5);
    pq.push(1);
    pq.push(4);
    pq.print();
    pq.pop();
    pq.print();

    // 8) Map<int,int>
    Map<int,int> mp;
    mp.insert(1, 100);
    mp.insert(2, 200);
    mp.print();
    mp.remove(1);
    mp.print();

    // 9) UnorderedMap<std::string,int>
    UnorderedMap<std::string,int> um;
    um.insert("ten", 10);
    um.insert("five", 5);
    um.print();
    um.remove("ten");
    um.print();

    // 10) ForwardList<double>
    ForwardList<double> fl;
    fl.push_front(3.14);
    fl.push_front(2.71);
    fl.print();
    fl.pop_front();
    fl.print();

    // 11) Vector<int>
    Vector<int> vec;
    vec.push_back(11);
    vec.push_back(22);
    vec.push_back(33);
    vec.print();

    // 12) Bitset<10>
    Bitset<10> bs;
    bs.set(0);
    bs.set(3);
    bs.print();

    // 13) Deque<int>
    Deque<int> d(5);
    d.push_front(1);
    d.push_back(2);
    d.push_back(3);
    d.print();
    d.pop_front();
    d.print();

    // 14) Span<int>
    int raw[5] = {10, 20, 30, 40, 50};
    Span<int> sp(raw, 5);
    sp.print();

    // 15) FlatSet<int>
    FlatSet<int> fs;
    fs.insert(10);
    fs.insert(5);
    fs.insert(7);
    fs.insert(5); // ignore duplicate
    fs.print();

    // 16) FlatMap<int,int>
    FlatMap<int,int> fm;
    fm.insert(2, 100);
    fm.insert(1, 50);
    fm.insert(3, 150);
    fm.insert(2, 75); // update
    fm.print();

    // 17) InplaceVector<double,5>
    InplaceVector<double,5> inv;
    inv.push_back(1.1);
    inv.push_back(2.2);
    inv.push_back(3.3);
    inv.print();

    // 18) MdSpan<double>
    double raw2D[6] = {10, 20, 30, 40, 50, 60};
    MdSpan<double> md(raw2D, 2, 3);
    md.print();

    // 19) UnorderedSet<int>
    UnorderedSet<int> us;
    us.insert(100);
    us.insert(200);
    us.insert(100); // duplicate, ignore
    us.print();
    us.remove(100);
    us.print();

    return 0;
}