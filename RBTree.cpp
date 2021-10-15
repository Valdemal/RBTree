#include "RBTree.h"

// Реализация красно-черного узла RBNode

template<typename baseType>
RBNode<baseType>::RBNode(baseType data) {
    this->_data = data;
    this->parent = this->leftChild = this->rightChild = nullptr;
}

template<typename baseType>
inline baseType RBNode<baseType>::data() {
    return _data;
}

template<typename baseType>
void RBNode<baseType>::recolor() {
    this->color = color_t(!bool(this->color));
}

template<typename baseType>
void RBNode<baseType>::swapKeys(nodePtr node) {
    baseType temp = this->_data;
    this->_data = node->_data;
    node->_data = temp;
}
// Конец реализации красно-черного узла RBNode

// Реализация красно-черного дерева RBTree

template<typename baseType>
RBTree<baseType>::RBTree() {
    root = nullptr;
}

// РЕАЛИЗАЦИЯ ПОИСКА

template<typename baseType>
bool RBTree<baseType>::find(baseType key) {
    nodePtr current = root;
    while (current->data() != key) {
        if (key < current->data())
            current = current->leftChild;
        else
            current = current->rightChild;

        if (current == nullptr)
            return false;
    }
    return true;
}

template<typename baseType>
baseType RBTree<baseType>::min() {
    nodePtr current, last;
    current = root;
    while (current != nullptr) {
        last = current;
        current = current->leftChild;
    }
    return last->data();
}

template<typename baseType>
baseType RBTree<baseType>::max() {
    nodePtr current, last;
    current = root;
    while (current != nullptr) {
        last = current;
        current = current->rightChild;
    }
    return last->data();
}

//РЕАЛИЗАЦИЯ ОБХОДА

template<typename baseType>
inline void RBTree<baseType>::_inOrder(nodePtr localRoot,
                                       void (*function)(baseType)) {
    if (localRoot != nullptr) {
        _inOrder(localRoot->leftChild, function);
        function(localRoot->data());
        _inOrder(localRoot->rightChild, function);
    }
}

template<typename baseType>
void RBTree<baseType>::inOrder(void (*function)(baseType)) {
    _inOrder(root, function);
}

//РЕАЛИЗАЦИЯ ВСТАВКИ

template<typename baseType>
void RBTree<baseType>::insert(baseType data) {
    auto newNode = new RBNode<baseType>(data); // Создание узла

    // Если дерево пустое
    if (root == nullptr) {
        //Создаем корень черного цвета
        newNode->color = BLACK;
        root = newNode;
    } else {
        nodePtr current = root, parent;

        // Перемещение вниз и поиск места для вставки
        while (true) {
            parent = current;

            //Перекрашивание parent и его потомков при необходимости
            descentRecoloring(parent);

            if (data < current->data()) {
                current = current->leftChild;
                if (current == nullptr) {
                    current = parent->leftChild = newNode;
                    break;
                }
            } else if (data > current->data()) {
                current = current->rightChild;
                if (current == nullptr) {
                    current = parent->rightChild = newNode;
                    break;
                }
            } else {
                //Значения ключей по условию не могут совпадать
                delete newNode;
                return;
            }
            // Если узел parent и его родитель красные, восстанавливает
            // сбалансированность на данном участке
            descentBalance(parent);
        }

        current->parent = parent;

        // Балансировка дерева при нарушении красно-черных
        // правил во время вставки
        afterInsertBalance(current);
    }
}

// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ВСТАВКИ

template<typename baseType>
void RBTree<baseType>::descentRecoloring(nodePtr parent) {
    //Если узел черный и имеет правого и левого потомка
    if (parent->color == BLACK && parent->leftChild && parent->rightChild) {
        //Если оба потомка красные
        if (parent->leftChild->color == RED &&
            parent->rightChild->color == RED) {
            //Перекрашиваем потомком
            parent->rightChild->recolor();
            parent->leftChild->recolor();
            //Перекрашиваем родителя, если это не нарушает правило 1
            if (parent != root)
                parent->recolor();
        }
    }
}

template<typename baseType>
void RBTree<baseType>::descentBalance(nodePtr current) {
    //Если возник конфликт типа "красный-красный"
    if (current->color == RED && current->parent->color == RED) {
        nodePtr grandparent = current->parent->parent;
        if (grandparent) {
            bool isRightChild = current->parent->rightChild == current;
            bool isLeftChild = current->parent->leftChild == current;
            bool isLeftGrandparent = grandparent->leftChild == current->parent;
            bool isRightGrandparent = grandparent->rightChild == current->parent;

            grandparent->recolor();
            if (isRightGrandparent && isRightChild ||
                isLeftGrandparent && isLeftChild) {
                //Внешний внук
                current->parent->recolor();

                if (isLeftChild)
                    rightRotate(grandparent);
                else
                    leftRotate(grandparent);

            } else {
                //Внутренний внук

                current->recolor();

                //Если узел-нарушитель левый потомок,
                // выполняем правый поворот, иначе правый поворот
                if (current->parent->rightChild == current)
                    leftRotate(current->parent);
                else
                    rightRotate(current->parent);

                //Поворот в строну предка
                if (current->parent->rightChild == current)
                    rightRotate(current->parent);
                else
                    leftRotate(current->parent);
            }
        }
    }
}


template<typename baseType>
void RBTree<baseType>::afterInsertBalance(nodePtr current) {
    if (current->parent->color == RED) {
        nodePtr grandparent = current->parent->parent;
        if (grandparent) {
            bool isRightChild = current->parent->rightChild == current;
            bool isLeftChild = current->parent->leftChild == current;
            bool isLeftGrandparent = grandparent->leftChild == current->parent;
            bool isRightGrandparent = grandparent->rightChild == current->parent;

            grandparent->recolor();
            if (isRightGrandparent && isRightChild ||
                isLeftGrandparent && isLeftChild) {
                //Внешний внук
                current->parent->recolor();
                if (isLeftChild)
                    rightRotate(grandparent);
                else
                    leftRotate(grandparent);
            } else {
                //Внутренний внук
                current->color = BLACK;
                if (isRightChild) {
                    leftRotate(current->parent);
                    rightRotate(grandparent);
                } else {
                    rightRotate(current->parent);
                    leftRotate(grandparent);
                }
            }
        }
    }
}

template<typename baseType>
void RBTree<baseType>::leftRotate(nodePtr localRoot) {
    nodePtr newRoot = localRoot->rightChild;
    localRoot->rightChild = newRoot->leftChild;
    if (newRoot->leftChild != nullptr)
        newRoot->leftChild->parent = localRoot;

    newRoot->parent = localRoot->parent;
    if (localRoot->parent == nullptr)
        this->root = newRoot;
    else if (localRoot == localRoot->parent->leftChild)
        localRoot->parent->leftChild = newRoot;
    else
        localRoot->parent->rightChild = newRoot;

    newRoot->leftChild = localRoot;
    localRoot->parent = newRoot;
}


template<typename baseType>
void RBTree<baseType>::rightRotate(nodePtr localRoot) {
    nodePtr newRoot = localRoot->leftChild;
    localRoot->leftChild = newRoot->rightChild;
    if (newRoot->rightChild != nullptr) {
        newRoot->rightChild->parent = localRoot;
    }
    newRoot->parent = localRoot->parent;
    if (localRoot->parent == nullptr) {
        this->root = newRoot;
    } else if (localRoot == localRoot->parent->rightChild) {
        localRoot->parent->rightChild = newRoot;
    } else {
        localRoot->parent->leftChild = newRoot;
    }
    newRoot->rightChild = localRoot;
    localRoot->parent = newRoot;
}

//РЕАЛИЗАЦИЯ УДАЛЕНИЯ

template <typename baseType>
bool RBTree<baseType>::remove(baseType key) {
    nodePtr current = root;

    //Поиск удаляемого узла
    while (current->data() != key) {
        if (key < current->data()) {
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }
        if (current == nullptr)
            return false;
    }

    //Количество потомков узла
    size_t childCnt = bool(current->rightChild) + bool(current->leftChild);

    if (childCnt == 0)
        removeNode0Children(current);
    else if (childCnt == 1)
        removeNode1Child(current);
    else {
        nodePtr successor = getSuccessor(current);
        successor->swapKeys(current);
        childCnt = bool(successor->rightChild) + bool(successor->leftChild);
        if (childCnt == 1)
            removeNode1Child(successor);
        else
            removeNode0Children(successor);
    }
}

template <typename baseType>
void RBTree<baseType>::removeNode0Children(nodePtr delNode) {
    if (delNode->color == RED)
        delNode = nullptr;
    else {
        nodePtr parent = delNode->parent;
        delNode = nullptr;
        afterRemoveBalance(parent);
    }
}

template <typename baseType>
void RBTree<baseType>::removeNode1Child(nodePtr delNode) {
    // По свойсву красно-черного дерева один потомок может быть
    // только у черного узла
    if (delNode->color == BLACK){
        if (delNode->leftChild){
            // !!! Возможна ошибка, непонятно, что с поддеревьями
            delNode->swapKeys(delNode->leftChild);
            delNode->leftChild = nullptr;
        } else {
            delNode->swapKeys(delNode->rightChild);
            delNode->rightChild = nullptr;
        }
    }
}

template <typename baseType>
void RBTree<baseType>::afterRemoveBalance(nodePtr parent) {
//    if (parent->leftChild == nullptr){
//
//    } else {
//
//    }
    return;
}
// Конец реализации красно-черного дерева RBTree