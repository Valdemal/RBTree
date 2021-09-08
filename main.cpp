#include <iostream>

using namespace std;

template<typename baseType>
class RBNode {
private:
    baseType data;
public:
    RBNode<baseType> *leftChild, *rightChild;
    bool isRed = true;

    explicit RBNode(baseType data) {
        this->data = data;
        this->leftChild = this->rightChild = nullptr;
    }

    baseType getData() {
        return data;
    }
};

template<typename baseType>
class RBTree {
private:
    RBNode<baseType> *root;
public:
    RBTree() {
        root = nullptr;
    }

    void insert(baseType data) {
        auto *newNode = new RBNode<baseType>(data); // Создание узла

        if (root == nullptr) { // Если дерево пустое
            newNode->isRed = false;
            root = newNode;
        } else {
            RBNode<baseType> *current = root, *parent = nullptr,
                    *grandparent = nullptr;

            // Перемещение вниз и поиск места для вставки
            while (true) {
                grandparent = parent;
                parent = current;

                if (data < current->getData()) {

                    current = current->leftChild;

                    if (current == nullptr) {
                        parent->leftChild = newNode;
                        break;
                    }
                } else {
                    current = current->rightChild;
                    if (current == nullptr) {
                        parent->rightChild = newNode;
                        break;
                    }
                }
            }

            // Балансировка дерева при нарушении красно-черных правил во
            // время вставки
            if (parent->isRed)
                balance(current, parent, grandparent);
        }
    }

private:
    void rightRotate(RBNode<baseType> *localRoot) {
        RBNode<baseType> *newRoot = localRoot->leftChild;
        localRoot->leftChild = newRoot->rightChild;
        newRoot->rightChild = localRoot;
    }

    void leftRotate(RBNode<baseType> *localRoot) {
        RBNode<baseType> *newRoot = localRoot->rightChild;
        localRoot->rightChild = newRoot->leftChild;
        newRoot->leftChild = localRoot;
    }
    void balance(RBNode<baseType> *current, RBNode<baseType> *parent,
                 RBNode<baseType> *grandparent){
        if (parent != nullptr || current != nullptr || grandparent)
            return;

        bool isRightChild = parent->rightChild == current;
        bool isLeftChild = parent->leftChild == current;
        bool isLeftGrandparent = grandparent->leftChild == parent;
        bool isRightGrandparent = grandparent->rightChild == parent;

        grandparent->isRed = !grandparent->isRed;

        if (isRightGrandparent && isRightChild ||
            isLeftGrandparent && isLeftChild) {
            //Внешний внук
            parent->isRed = !parent->isRed;
            if (isLeftChild)
                rightRotate(grandparent);
            else
                leftRotate(grandparent);
        } else {
            //Внутренний внук
            current->isRed = false;
            if(isRightChild){
                leftRotate(parent);
                rightRotate(grandparent);
            } else {
                rightRotate(parent);
                leftRotate(grandparent);
            }
        }
    }
public:
    RBNode<baseType> *find(baseType key) {
        RBNode<baseType> *current = root;
        while (current->getData() != key) {
            if (key < current->getData())
                current = current->leftChild;
            else
                current = current->rightChild;

            if (current == nullptr)
                return nullptr;
        }
    }

    baseType max() {
        RBNode<baseType> *current, *last;
        current = root;
        while (current != nullptr) {
            last = current;
            current = current->rightChild;
        }
        return last->getData();
    }

    baseType min() {
        RBNode<baseType> *current, *last;
        current = root;
        while (current != nullptr) {
            last = current;
            current = current->leftChild;
        }
        return last->getData();
    }

    // Выполняет симметричный обход дерева, вызывая для каждого узла функцию function
    void inOrder(void function(baseType)) {
        inOrderInner(root, function);
    }

    // Выполняет прямой обход дерева, вызывая для каждого узла функцию function
    void preOrder(void function(baseType)) {
        preOrderInner(root, function);
    }

    // Выполняет обратный обход дерева, вызывая для каждого узла функцию function
    void postOrder(void function(baseType)) {
        postOrderInner(root, function);
    }

private:
    // Выполняет симметричный обход начиная с localRoot, вызывая для каждого
    // узла функцию function
    void inOrderInner(RBNode<baseType> *localRoot, void function(baseType)) {
        if (localRoot != nullptr) {
            inOrderInner(localRoot->leftChild, function);
            function(localRoot->getData());
            inOrderInner(localRoot->rightChild, function);
        }
    }

    // Выполняет прямой обход начиная с localRoot, вызывая для каждого узла
    // функцию function
    void preOrderInner(RBNode<baseType> *localRoot, void function(baseType)) {
        if (localRoot != nullptr) {
            function(localRoot->getData());
            preOrderInner(localRoot->leftChild, function);
            preOrderInner(localRoot->rightChild, function);
        }
    }

    // Выполняет обратный обход начиная с localRoot, вызывая для каждого узла
    // функцию function
    void postOrderInner(RBNode<baseType> *localRoot, void function(baseType)) {
        if (localRoot != nullptr) {
            postOrderInner(localRoot->leftChild, function);
            postOrderInner(localRoot->rightChild, function);
            function(localRoot->getData());
        }
    }


};

void printInt(int x){
    cout << x << " ";
}

int main() {
    RBTree<int> tree;

    tree.insert(50);
    tree.insert(25);
    tree.insert(75);
    tree.insert(12);
    tree.insert(37);
    tree.insert(6);
    tree.insert(18);

    tree.inOrder(printInt);
}
