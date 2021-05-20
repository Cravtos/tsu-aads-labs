#include "include/bintree.h"

#include <random>
#include <ctime>
#include <sstream>

std::mt19937& mt() {
    // initialize once per thread
    thread_local static std::mt19937 mt(static_cast<uint32_t>(time(nullptr)));
    return mt;
}

Node::Node(int32_t key, Node* left, Node* right) {
    this->key = key;
    this->left = left;
    this->right = right;
}

BST::BST() {
    this->root = nullptr;
}

BST::BST(size_t nodes) {
    if (nodes == 0) {
        root = nullptr;
        return;
    }

    auto rand = mt();

    // create root
    auto key = int32_t(rand() % 100);
    root = new Node(key);
    nodes--;

    while (nodes-- > 0) {
        key = int32_t(rand() % 100);
        add(key);
    }
}

void rec_copy(Node* from, Node* to) {
    if (from->left != nullptr) {
        to->left = new Node(from->left->key);
        rec_copy(from->left, to->left);
    }

    if (from->right != nullptr) {
        to->right = new Node(from->right->key);
        rec_copy(from->right, to->right);
    }
}

BST::BST(const BST& from) {
    if (from.root == nullptr) {
        root = nullptr;
        return;
    }

    root = new Node(from.root->key);
    rec_copy(from.root, root);
}

BST::BST(int32_t* arr, size_t size) {
    if (size == 0) {
        root = nullptr;
        return;
    }

    root = new Node(arr[0]);
    for (size_t i = 1; i < size; i++) {
        add(arr[i]);
    }
}

BST& BST::operator=(const BST& from) {
    if (this == &from) {
        return *this;
    }

    this->~BST();

    if (from.root == nullptr) {
        root = nullptr;
        return *this;
    }

    root = new Node(from.root->key);
    rec_copy(from.root, root);
    return *this;
}

// search returns nullptr if key is not present
Node* BST::search(int32_t key) const {
    Node* cur = root;
    while (cur != nullptr && cur->key != key) {
        if (key < cur->key) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }

    return cur;
}

Node* BST::get_root() const {
    return root;
}

Node* BST::add(int32_t key) {
    Node* node = new Node(key);

    // edge case
    if (root == nullptr) {
        root = node;
        return node;
    }

    Node* after = root;
    while (true) {
        if (node->key < after->key) {
            if (after->left == nullptr) {
                break;
            }
            after = after->left;
        } else {
            if (after->right == nullptr) {
                break;
            }
            after = after->right;
        }
    }

    if (node->key < after->key) {
        after->left = node;
    } else {
        after->right = node;
    }

    return node;
}

void BST::del(int32_t key) {
    Node* parent = nullptr;
    Node* to_delete = root;

    // Find node to delete and its parent
    while (to_delete != nullptr && to_delete->key != key) {
        parent = to_delete;
        if (key < to_delete->key) {
            to_delete = to_delete->left;
        } else {
            to_delete = to_delete->right;
        }
    }

    // Return if no node with such key
    if (to_delete == nullptr) {
        return;
    }

    // Case: node to delete is a leaf
    if (to_delete->left == nullptr && to_delete->right == nullptr) {
        if (parent != nullptr) {
            if (parent->left == to_delete) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
        } else {
            root = nullptr;
        }
        delete to_delete;
        return;
    }

    // Case: node to delete has both children
    if (to_delete->left != nullptr && to_delete->right != nullptr) {
        Node* replace_parent = to_delete;
        Node* replace = to_delete->left;

        // Find max in left subtree
        while (replace->right != nullptr) {
            replace_parent = replace;
            replace = replace_parent->right;
        }

        // Replace node to delete with maximal node in left subtree
        to_delete->key = replace->key;
        if (replace_parent == to_delete) {
            to_delete->left = replace->left;
        } else {
            replace_parent->right = replace->left;
        }
        delete replace;

        return;
    }

    // Case: node to delete has one child
    Node* replace = to_delete->right;
    if (to_delete->left != nullptr) {
        replace = to_delete->left;
    }

    to_delete->key = replace->key;
    to_delete->left = replace->left;
    to_delete->right = replace->right;
    delete replace;
}

// Deletes everything starting from node
void rec_delete(Node* node) {
    if (node == nullptr) {
        return;
    }

    rec_delete(node->left);
    rec_delete(node->right);

    delete node;
}

BST::~BST() {
    rec_delete(root);
    root = nullptr;
}

void BST::print(std::ostream& os, size_t spaces, Node* node) const {
    if (node == nullptr) {
        return;
    }
    print(os, spaces+5, node->right);

    for (size_t i = 0; i < spaces; i++) {
        os << " ";
    }
    os << node->key << std::endl;

    print(os, spaces+5, node->left);
}

std::ostream& operator<<(std::ostream& os, const BST& bst) {
    bst.print(os, 0, bst.root);
}

Node* BST::get_min() const {
    if (root == nullptr) {
        return nullptr;
    }

    Node* min = root;
    while (min->left != nullptr) {
        min = min->left;
    }

    return min;
}

Node* BST::get_max() const {
    if (root == nullptr) {
        return nullptr;
    }

    Node* max = root;
    while (max->right != nullptr) {
        max = max->right;
    }

    return max;
}

void BST::trav_level_order(std::list<Node*>& list) const {
    if (root == nullptr) {
        return;
    }

    list.push_back(root);
    for (auto node : list) {
        if (node->left != nullptr) {
            list.push_back(node->left);
        }

        if (node->right != nullptr) {
            list.push_back(node->right);
        }
    }
}

void rec_trav_in_order(std::list<Node*>& list, Node* node) {
    if (node == nullptr) {
        return;
    }

    rec_trav_in_order(list, node->left);
    rec_trav_in_order(list, node->right);
    list.push_back(node);
}

void rec_trav_pre_order(std::list<Node*>& list, Node* node) {
    if (node == nullptr) {
        return;
    }

    list.push_back(node);
    rec_trav_pre_order(list, node->left);
    rec_trav_pre_order(list, node->right);
}

void rec_trav_rev_in_order(std::list<Node*>& list, Node* node) {
    if (node == nullptr) {
        return;
    }

    rec_trav_rev_in_order(list, node->right);
    rec_trav_rev_in_order(list, node->left);
    list.push_back(node);
}


void BST::trav_in_order(std::list<Node*>& list) const {
    rec_trav_in_order(list, root);
}

void BST::trav_pre_order(std::list<Node*>& list) const {
    rec_trav_pre_order(list, root);
}

void BST::trav_rev_in_order(std::list<Node*>& list) const {
    rec_trav_rev_in_order(list, root);
}
