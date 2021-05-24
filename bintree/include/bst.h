#ifndef BINTREE_BST_H
#define BINTREE_BST_H

#include <include/node.h>

#include <cstdint>
#include <ostream>
#include <list>


class BST {
protected:
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
protected:
    void print(std::ostream& os, size_t spaces, Node* node) const;
};

#endif //BINTREE_BST_H
