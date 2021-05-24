#include "../include/avl.h"

#include <iostream>

using namespace std;

int main() {
    AVL avl(10);
    Node* root = avl.get_root();
    if (root != nullptr) {
        cout << "Root value is " << root->get_key() << endl;
    }

    cout << "Random AVL: " << endl;
    cout << avl << endl << endl;

    AVL copy(avl);
    cout << "Copy of random AVL: " << endl;
    cout << copy << endl << endl;

    avl = copy;
    cout << "Assigned from copy of random AVL: " << endl;
    cout << avl << endl << endl;

    std::string action = "x";
    cout << "[a]dd, [d]elete, [f]ind, [p]rint, [q]uit" << endl;
    cout << "ma[x], mi[n], ne[w] tree, [h]elp" << endl;
    cout << "[l]evel, [i]n order, p[r]e order, r[e]v in order (traverse)" << endl;
    do {
        cout << ">";
        cin >> action;

        if (action == "a") {
            int32_t key;
            // cout << "Enter key:";
            cin >> key;

            avl.add(key);

        } else if (action == "h") {
            cout << "[a]dd, [d]elete, [f]ind, [p]rint, [q]uit" << endl;
            cout << "ma[x], mi[n], ne[w] tree, [h]elp" << endl;
            cout << "[l]evel, [i]n order, p[r]e order, r[e]v in order (traverse)" << endl;

        } else if (action == "l") {
            std::list<Node*> list;
            avl.trav_level_order(list);

            for (auto node : list) {
                cout << node->get_key() << " ";
            }
            cout << endl;

        } else if (action == "i") {
            std::list<Node*> list;
            avl.trav_in_order(list);

            for (auto node : list) {
                cout << node->get_key() << " ";
            }
            cout << endl;

        } else if (action == "r") {
            std::list<Node*> list;
            avl.trav_pre_order(list);

            for (auto node : list) {
                cout << node->get_key() << " ";
            }
            cout << endl;

        }  else if (action == "e") {
            std::list<Node*> list;
            avl.trav_rev_in_order(list);

            for (auto node : list) {
                cout << node->get_key() << " ";
            }
            cout << endl;

        } else if (action == "d") {
            int32_t key;
            // cout << "Enter key:";
            cin >> key;

            avl.del(key);

        } else if (action == "p") {
            cout << endl << avl << endl << endl;

        } else if (action == "f") {
            int32_t key;
            // cout << "Enter key:";
            cin >> key;

            Node* node = avl.search(key);

            if (node == nullptr) {
                cout << "Not found" << endl;
            } else {
                cout << "Found" << endl;
            }

        } else if (action == "x") {
            Node* max = avl.get_max();
            if (max == nullptr) {
                cout << "Not found" << endl;
            } else {
                cout << "Max is " << max->get_key() << endl;
            }

        } else if (action == "n") {
            Node* min = avl.get_min();
            if (min == nullptr) {
                cout << "Not found" << endl;
            } else {
                cout << "Min is " << min->get_key() << endl;
            }

        } else if (action == "w") {
            const size_t size = 10;
            auto arr = new int32_t[size];

            cout << "Enter 10 elements" << endl;
            for (size_t i = 0; i < size; i++) {
                cin >> arr[i];
            }

            avl = AVL(arr, size);
            delete[] arr;

        } else {
            cout << "Unknown action" << endl;
        }

    } while (action != "q");

    return 0;
}