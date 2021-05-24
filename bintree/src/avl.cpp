#include "include/avl.h"

#include <random>
#include <ctime>

static std::mt19937& mt() {
    // initialize once per thread
    thread_local static std::mt19937 mt(static_cast<uint32_t>(time(nullptr)));
    return mt;
}

AVL::AVL(size_t nodes) {
    if (nodes == 0) {
        root = nullptr;
        return;
    }

    auto rand = mt();

    // create root
    auto key = int32_t(rand() % 100);
    root = new Node(key);
    nodes--;

    while (nodes --> 0) {
        key = int32_t(rand() % 100);
        add(key);
    }
}

AVL::AVL(int32_t* arr, size_t size) {
    if (size == 0) {
        root = nullptr;
        return;
    }

    root = new Node(arr[0]);
    for (size_t i = 1; i < size; i++) {
        add(arr[i]);
    }
}

// rec_add returns new root after balancing
Node* AVL::rec_add(Node* to, int32_t key) {
    if (to == nullptr) {
        return new Node(key);
    }

    if (key < to->key) {
        to->left = rec_add(to->left, key);
    } else {
        to->right = rec_add(to->right, key);
    }
    return balance(to);
}

void AVL::add(int32_t key) {
    root = rec_add(root, key);
}

Node* AVL::del_min(Node* from) {
    if (from->left == nullptr) {
        return from->right;
    }
    from->left = del_min(from->left);
    return balance(from);
}

Node* AVL::rec_del(Node* node, int32_t key) {
    if (node == nullptr) {
        return nullptr;
    }

    if (key < node->key) {
        node->left = rec_del(node->left, key);
    } else if (key > node->key) {
        node->right = rec_del(node->right, key);
    } else {
        Node* l = node->left;
        Node* r = node->right;
        delete node;

        if (r == nullptr) {
            return l;
        }

        Node* min = r;
        while (min->left != nullptr) {
            min = min->left;
        }

        min->right = del_min(r);
        min->left = l;
        return balance(min);
    }

    return balance(node);
}

void AVL::del(int32_t key) {
    root = rec_del(root, key);
}
