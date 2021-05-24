#include "include/node.h"

Node::Node(int32_t key, int8_t height, Node* left, Node* right) {
    this->key = key;
    this->height = height;
    this->left = left;
    this->right = right;
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

void rec_copy(Node* from, Node* to) {
    if (from->left != nullptr) {
        to->left = new Node(from->left->key, from->left->height);
        rec_copy(from->left, to->left);
    }

    if (from->right != nullptr) {
        to->right = new Node(from->right->key, from->right->height);
        rec_copy(from->right, to->right);
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

int8_t Node::balance_factor() const {
    auto rbal = right ? right->get_height() : 0;
    auto lbal = left ? left->get_height() : 0;
    return int8_t(rbal - lbal);
}

void Node::update_height() {
    auto lheight = left ? left->height : 0;
    auto rheight = right ? right->height : 0;
    auto max = (lheight > rheight) ? lheight : rheight;
    height = max + 1;
}

Node* rotate_right(Node* around) {
    Node* new_root = around->left;
    if (new_root == nullptr) {
        return around;
    }
    around->left = new_root->right;
    new_root->right = around;
    around->update_height();
    new_root->update_height();
    return new_root;
}

Node* rotate_left(Node* around) {
    Node* new_root = around->right;
    if (new_root == nullptr) {
        return around;
    }
    around->right = new_root->left;
    new_root->left = around;
    around->update_height();
    new_root->update_height();
    return new_root;
}

Node* balance(Node* node) {
    node->update_height();
    if (node->balance_factor() > 1) {
        if (node->right->balance_factor() < 0) {
            node->right = rotate_right(node->right);
        }
        return rotate_left(node);
    }

    if (node->balance_factor() < -1) {
        if (node->left->balance_factor() > 0) {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }

    return node;
}