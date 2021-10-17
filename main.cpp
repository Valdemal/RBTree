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
    int *a = new int[n];
    for (int i = 0; i < n; ++i) {
        a[i] = rand();
        tree.insert(a[i]);
    }


//    for (int i = 0; i < n; ++i) {
//        RBTree<int> copy = tree;
//        removeAndCheck(copy, a[i]);
//    }

    tree.checkRool3();
    tree.checkRool4();
    tree.inOrder(printInt);

    delete [] a;
    delete &tree;
}

