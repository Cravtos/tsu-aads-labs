#ifndef BINTREE_BINTREE_H
#define BINTREE_BINTREE_H

#include <cstdint>
#include <ostream>
#include <list>

class Node {
    int32_t key;
    Node* left;
    Node* right;

public:
    Node(int32_t key, Node* left = nullptr, Node* right = nullptr);

    int32_t value() const {
        return key;
    }

    friend class BST;
private:
    // those functions are related to BST
    friend void rec_delete(Node* node);
    friend void rec_copy(Node* from, Node* to);
    friend void rec_trav_in_order(std::list<Node*>& list, Node* node);
    friend void rec_trav_pre_order(std::list<Node*>& list, Node* node);
    friend void rec_trav_rev_in_order(std::list<Node*>& list, Node* node);
};

class BST {
    Node* root;

public:
    BST();
    BST(int32_t* arr, size_t size);
    BST(std::size_t nodes);
    BST(const BST& from);
    ~BST();

    Node* get_root() const;
    Node* get_min() const;
    Node* get_max() const;

    void trav_level_order(std::list<Node*>& list) const;
    void trav_in_order(std::list<Node*>& list) const;
    void trav_pre_order(std::list<Node*>& list) const;
    void trav_rev_in_order(std::list<Node*>& list) const;

    BST& operator=(const BST& from);

    Node* search(int32_t key) const;
    Node* add(int32_t key);
    void del(int32_t key);

    friend std::ostream& operator<<(std::ostream& os, const BST& bst);
private:
    void print(std::ostream& os, size_t spaces, Node* node) const;
};

#endif //BINTREE_BINTREE_H
