#ifndef BINTREE_NODE_H
#define BINTREE_NODE_H

#include <cstdint>
#include <list>

class Node {
    int32_t key;
    Node* left;
    Node* right;
    int8_t height;

public:
    explicit Node(int32_t key, int8_t height = 1, Node* left = nullptr, Node* right = nullptr);

    auto get_key() const {
        return key;
    }

    auto get_height() const {
        return height;
    }

    friend class BST;
    friend class AVL;
private:
    int8_t balance_factor() const;
    void update_height();

    friend Node* rotate_right(Node* around);
    friend Node* rotate_left(Node* around);
    friend Node* balance(Node* node);

    friend void rec_delete(Node* node);
    friend void rec_copy(Node* from, Node* to);
    friend void rec_trav_in_order(std::list<Node*>& list, Node* node);
    friend void rec_trav_pre_order(std::list<Node*>& list, Node* node);
    friend void rec_trav_rev_in_order(std::list<Node*>& list, Node* node);
};

#endif //BINTREE_NODE_H
