#include "RBTree.cpp"

int blackHeight;

void printInt(int x) {
    cout << x << " ";
}

void insertAndCheck(RBTree<int> &t, int key){
    t.insert(key);
    blackHeight = t.checkRool4();
    t.checkRool3();
}


void removeAndCheck(RBTree<int> &t, int key){
    t.remove(key);
    blackHeight = t.checkRool4();
    t.checkRool3();
}

int main() {
    RBTree<int> tree;

    int n = 100;
//    int *a = new int[n];
//    for (int i = 0; i < n; ++i) {
//        a[i] = rand();
//        tree.insert(a[i]);
//    }
//
    tree.insert(16);
    tree.insert(10);
    tree.insert(20);
    tree.insert(4);
    tree.insert(14);
    tree.insert(2);
    tree.insert(5);
    tree.insert(12);
    tree.insert(15);
    tree.insert(11);
    tree.insert(17);
    tree.insert(25);
    tree.insert(19);
    tree.insert(23);
    tree.insert(30);

    tree.remove(19);
    tree.remove(23);
    tree.remove(30);
    tree.remove(11);

    tree.remove(2);
    tree.remove(5);
    tree.inOrder(printInt);
    cout << endl;
    tree.remove(15);
//    tree.remove(12);

    tree.checkRool3();
    tree.checkRool4();
    tree.inOrder(printInt);

//    delete [] a;
    delete &tree;
}

