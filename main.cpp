#include "libs/treelib/RBTree.h"

void printInt(int x) {
    cout << x << " ";
}

int main() {
    RBTree<int> tree;

    for (int i = 0; i < 100000000; ++i) {
        tree.insert(rand());
    }

    delete &tree;
}

