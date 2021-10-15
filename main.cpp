#include "RBTree.cpp"

void printInt(int x) {
    cout << x << " ";
}

int main() {
    RBTree<int> tree;

    tree.insert(20);
    tree.insert(10);
    tree.insert(25);
    tree.insert(23);
    tree.insert(30);
    tree.insert(4);
    tree.insert(16);
    tree.insert(14);
    tree.insert(17);
    tree.insert(2);
    tree.insert(5);
    tree.insert(3);
    tree.insert(12);
    tree.insert(15);
    tree.insert(11);
    tree.insert(19);
    tree.inOrder(printInt);
}

