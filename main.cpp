#include <iostream>

using namespace std;

template<typename baseType>
class RBNode {
private:
    typedef RBNode<baseType> node;
    baseType _data;

public:
    node *leftChild, *rightChild;
    bool isRed = true;

    explicit RBNode(baseType data) {
        this->_data = data;
        this->leftChild = this->rightChild = nullptr;
    }

    inline baseType data() {
        return _data;
    }
};

template<typename baseType>
class RBTree {
private:
    typedef RBNode<baseType> node;
    node *root;
public:
    RBTree() {
        root = nullptr;
    }

    void insert(baseType data) {
        auto *newNode = new node(data); // Создание узла

        if (root == nullptr) { // Если дерево пустое
            newNode->isRed = false;
            root = newNode;
        } else {
            node *current = root, *parent = nullptr,
                    *grandparent = nullptr;

            // Перемещение вниз и поиск места для вставки
            while (true) {
                grandparent = parent;
                parent = current;

                if (data < current->data()) {

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
    void rightRotate(node *localRoot) {
        node *newRoot = localRoot->leftChild;
        localRoot->leftChild = newRoot->rightChild;
        newRoot->rightChild = localRoot;
    }

    void leftRotate(node *localRoot) {
        node *newRoot = localRoot->rightChild;
        localRoot->rightChild = newRoot->leftChild;
        newRoot->leftChild = localRoot;
    }
    void balance(node *current, node *parent,
                 node *grandparent){
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
    node *find(baseType key) {
        node *current = root;
        while (current->data() != key) {
            if (key < current->data())
                current = current->leftChild;
            else
                current = current->rightChild;

            if (current == nullptr)
                return nullptr;
        }
    }

    baseType max() {
        node *current, *last;
        current = root;
        while (current != nullptr) {
            last = current;
            current = current->rightChild;
        }
        return last->data();
    }

    baseType min() {
        node *current, *last;
        current = root;
        while (current != nullptr) {
            last = current;
            current = current->leftChild;
        }
        return last->data();
    }

    // Выполняет симметричный обход дерева, вызывая для каждого узла функцию function
    void inOrder(void function(baseType)) {
        _inOrder(root, function);
    }

private:
    // Выполняет симметричный обход начиная с localRoot, вызывая для каждого
    // узла функцию function
    inline void _inOrder(node *localRoot, void function(baseType)) {
        if (localRoot != nullptr) {
            _inOrder(localRoot->leftChild, function);
            function(localRoot->data());
            _inOrder(localRoot->rightChild, function);
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

