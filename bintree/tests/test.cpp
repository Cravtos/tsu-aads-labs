#include "../include/bintree.h"

#include <iostream>

using namespace std;

int main() {
    BST bst(10);
    Node* root = bst.get_root();
    if (root != nullptr) {
        cout << "Root value is " << root->value() << endl;
    }

    cout << "Random BST: " << endl;
    bst.print(cout);
    cout << endl << endl;

    BST copy(bst);
    cout << "Copy of random BST: " << endl;
    copy.print(cout);
    cout << endl << endl;

    bst = copy;
    cout << "Assigned from copy of random BST: " << endl;
    bst.print(cout);
    cout << endl << endl;

    std::string action = "x";
    cout << "[a]dd, [d]elete, [f]ind, [p]rint, [q]uit" << endl;
    cout << "ma[x], mi[n], ne[w] tree, [h]elp" << endl;
    cout << "[l]evel, [i]n order, p[r]e order, r[e]v in order (traverse)" << endl;
    do {
        cout << ">";
        cin >> action;

        if (action == "a") {
            int32_t key;
            cout << "Enter key:";
            cin >> key;

            bst.add(key);

        } else if (action == "h") {
            cout << "[a]dd, [d]elete, [f]ind, [p]rint, [q]uit" << endl;
            cout << "ma[x], mi[n], ne[w] tree, [h]elp" << endl;
            cout << "[l]evel, [i]n order, p[r]e order, r[e]v in order (traverse)" << endl;

        } else if (action == "l") {
            std::list<Node*> list;
            bst.trav_level_order(list);

            for (auto node : list) {
                cout << node->value() << " ";
            }
            cout << endl;

        } else if (action == "i") {
            std::list<Node*> list;
            bst.trav_in_order(list);

            for (auto node : list) {
                cout << node->value() << " ";
            }
            cout << endl;

        } else if (action == "r") {
            std::list<Node*> list;
            bst.trav_pre_order(list);

            for (auto node : list) {
                cout << node->value() << " ";
            }
            cout << endl;

        }  else if (action == "e") {
            std::list<Node*> list;
            bst.trav_rev_in_order(list);

            for (auto node : list) {
                cout << node->value() << " ";
            }
            cout << endl;

        } else if (action == "d") {
            int32_t key;
            cout << "Enter key:";
            cin >> key;

            bst.del(key);

        } else if (action == "p") {
            cout << endl;
            bst.print(cout);
            cout << endl << endl;

        } else if (action == "f") {
            int32_t key;
            cout << "Enter key:";
            cin >> key;

            Node* node = bst.search(key);

            if (node == nullptr) {
                cout << "Not found" << endl;
            } else {
                cout << "Found" << endl;
            }

        } else if (action == "x") {
            Node* max = bst.get_max();
            if (max == nullptr) {
                cout << "Not found" << endl;
            } else {
                cout << "Max is " << max->value() << endl;
            }

        } else if (action == "n") {
            Node* min = bst.get_min();
            if (min == nullptr) {
                cout << "Not found" << endl;
            } else {
                cout << "Min is " << min->value() << endl;
            }

        } else if (action == "w") {
            const size_t size = 10;
            auto arr = new int32_t[size];

            cout << "Enter 10 elements" << endl;
            for (size_t i = 0; i < size; i++) {
                cin >> arr[i];
            }

            bst = BST(arr, size);
            delete[] arr;

        } else {
            cout << "Unknown action" << endl;
        }

    } while (action != "q");

    return 0;
}