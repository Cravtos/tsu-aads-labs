#ifndef BINTREE_AVL_H
#define BINTREE_AVL_H

#include "bst.h"

class AVL: public BST {
public:
    AVL(int32_t* arr, size_t size);
    AVL(std::size_t nodes);

    void add(int32_t key);
    void del(int32_t key);
private:
    Node* del_min(Node* from);
    Node* rec_add(Node* to, int32_t key);
    Node* rec_del(Node* to, int32_t key);
};

#endif //BINTREE_AVL_H
