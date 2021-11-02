#include "RBTree.cpp"

void printInt(int x) {
    cout << x << " ";
}

int main() {
    RBTree<int> tree;
    for (int i = 0; i < 100; ++i) {
        tree.insert(rand());
    }
//
//    tree.insert(50);
//    tree.insert(25);
//    tree.insert(75);
//    tree.insert(12);
//    tree.insert(37);
//    tree.insert(31);
//    tree.insert(43);
//    tree.insert(28);
//
//
//    tree.remove(28);
//    tree.remove(12);
//    tree.remove(31);
//    tree.remove(43);
//    tree.remove(75);
    tree.inOrder(printInt);

    tree.checkRule3();
    tree.checkRule4();
    delete &tree;
}

