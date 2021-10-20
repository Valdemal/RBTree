#include "RBTree.h"

// Реализация красно-черного узла RBNode

template<typename baseType>
RBNode<baseType>::RBNode(baseType data) {
    this->_data = data;
    this->parent = this->leftChild = this->rightChild = nullptr;
    color = RED;
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
            if (parent->color == RED && parent->parent->color == RED) {
                insertBalance(parent);
            }
        }

        current->parent = parent;

        // Балансировка дерева при нарушении красно-черных
        // правил во время вставки
        if (current->parent->color == RED) {
            insertBalance(current);
        }
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
void RBTree<baseType>::insertBalance(nodePtr current) {
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

template<typename baseType>
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
        /* Сведение задачи удаления узла с двумя потомками, к удалению узла с од */

        nodePtr successor = getSuccessor(current); // Поиск приемника узла
        successor->swapKeys(current); // Обмен ключами

        // Если у приемника есть потомки (максимум 1)
        if (successor->rightChild || successor->leftChild)
            removeNode1Child(successor);
        else
            removeNode0Children(successor);
    }
    return true;
}

template<typename baseType>
RBNode<baseType> *RBTree<baseType>::getSuccessor(nodePtr delNode) {
    nodePtr successor = delNode;
    nodePtr current = delNode->rightChild;

    while (current != nullptr) { ;
        successor = current;
        current = current->leftChild;
    }

    return successor;
}

template<typename baseType>
void RBTree<baseType>::removeNode1Child(nodePtr delNode) {
    // По свойсву красно-черного дерева один потомок может быть
    // только у черного узла
    if (delNode->color == BLACK) {
        if (delNode->leftChild) {
            delNode->swapKeys(delNode->leftChild);
            delNode->leftChild = nullptr;
            delete delNode->leftChild;
        } else {
            delNode->swapKeys(delNode->rightChild);
            delNode->rightChild = nullptr;
            delete delNode->rightChild;
        }
    }
}

template<typename baseType>
void RBTree<baseType>::removeNode0Children(nodePtr delNode) {

    // Если удаляемый узел - корень
    if (delNode == root)
        root = nullptr;
    else {
        nodePtr parent = delNode->parent;
        nodePtr brother;
        if (parent->leftChild == delNode) {
            parent->leftChild = nullptr;
            brother = parent->rightChild;
        } else {
            parent->rightChild = nullptr;
            brother = parent->leftChild;
        }

        /* Балансировку нужно выполнять, только если удаляемый узел черный,
         * так как удаление красного узла без потомков не приводит к
         * расбалансировке */
        if (delNode->color == BLACK)
            removeBalance(parent, brother);
        // Освобождение памяти
        delete delNode;
    }
}

template<typename baseType>
void RBTree<baseType>::removeBalance(nodePtr parent, nodePtr brother) {
    bool isLeftBrother = parent->leftChild == brother;
    if (parent->color == BLACK) {
        if (brother->color == BLACK) {
            // Родитель черный брат черный
            removeBalanceBB(parent, brother, isLeftBrother);
        } else {
            // Родитель черный брат красный
            removeBalanceBR(parent, brother, isLeftBrother);
        }
    } else
        // Родитель красный брат черный
        removeBalanceRB(parent, brother, isLeftBrother);
}

template<typename baseType>
void RBTree<baseType>::removeBalanceRB(nodePtr parent, nodePtr brother,
                                       const bool &isLeftBrother) {
    // Левый потомок брата существует и он красный
    if (brother->leftChild &&
        brother->leftChild->color == RED) {
        if (isLeftBrother) {
            brother->recolor();
            parent->recolor();
            brother->leftChild->recolor();
            rightRotate(parent);
        } else {
            parent->color = BLACK;
            rightRotate(brother);
            leftRotate(parent);
        }

    } // Правый потомок брата существует и он красный
    else if (brother->rightChild &&
             brother->rightChild->color == RED) {
        if (isLeftBrother) {
            parent->color = BLACK;
            leftRotate(brother);
            rightRotate(parent);
        } else {
            brother->recolor();
            parent->recolor();
            brother->rightChild->recolor();
            leftRotate(parent);
        }
    } else { // оба потомка брата черные или пустые(тоже черные)
        parent->recolor();
        brother->recolor();
    }
}

template<typename baseType>
void RBTree<baseType>::removeBalanceBB(nodePtr parent, nodePtr brother,
                                       const bool &isLeftBrother) {
    if (brother->rightChild && brother->rightChild->color == RED) {
        if (isLeftBrother) {
            brother->rightChild->recolor();
            leftRotate(brother);
            rightRotate(parent);
        } else {
            brother->rightChild->recolor();
            leftRotate(parent);
        }
    } else if (brother->leftChild && brother->leftChild->color == RED) {
        if (isLeftBrother) {
            brother->leftChild->recolor();
            rightRotate(parent);
        } else {
            brother->leftChild->recolor();
            rightRotate(brother);
            leftRotate(parent);
        }
    } else {
        brother->recolor();
        // Если parent - корень и двигаться выше уже нельзя
        if (parent->parent) {
            // Вызываем рекурсивно балансировку для родителя, в качестве
            // нового родителя передаем родителя родителя, а в качестве брата
            // - брата родителя
            brother = parent == parent->parent->leftChild ?
                      parent->rightChild : parent->leftChild;
            removeBalance(parent->parent, parent);
        }
    }
}

template<typename baseType>
void RBTree<baseType>::removeBalanceBR(nodePtr parent, nodePtr brother,
                                       const bool &isLeftBrother) {
    nodePtr nephew =  brother->rightChild;
    // Такой узел brother->rightChild по-любому есть, иначе до этого
    // было нарушение КЧ правил

    if (nephew->leftChild &&
        nephew->leftChild->color == RED) {
        if (isLeftBrother){
            nephew->leftChild->recolor();
            leftRotate(brother);
            rightRotate(parent);
        } else {
            // Внимание!!! Возвможна ошибка в случае,
            // когда другой правнук тоже красный множно обойти,
            // если 3 случай поставить наверх
            nephew->leftChild->recolor();
            nephew->recolor();
            rightRotate(nephew);
            removeBalanceRB(parent, brother, isLeftBrother);
        }
    }
    //
    else if (nephew->rightChild &&
             nephew->rightChild->color == RED) {
        if (isLeftBrother){
            // Внимание!!! Возвможна ошибка в случае,
            // когда другой правнук тоже красный множно обойти,
            // если 3 случай поставить наверх
            nephew->rightChild->recolor();
            nephew->recolor();
            leftRotate(nephew);
            removeBalanceRB(parent, brother, isLeftBrother);
        } else {
            nephew->rightChild->recolor();
            rightRotate(brother);
            leftRotate(parent);
        }
    } else {
        nephew->recolor();
        brother->recolor();
        if (isLeftBrother)
            rightRotate(parent);
        else
            leftRotate(parent);
    }
}
// УДАЛЕНИЕ ДЕРЕВА

template<typename baseType>
RBTree<baseType>::~RBTree() {
    deleteTree(root);
    root = nullptr;
}

template<typename baseType>
void RBTree<baseType>::deleteTree(nodePtr localRoot) {
    if (localRoot != nullptr) {
        deleteTree(localRoot->leftChild);
        deleteTree(localRoot->rightChild);
        delete localRoot;
    }
}

// Конец реализации красно-черного дерева RBTree

// Отладка

template<typename baseType>
int RBTree<baseType>::checkRool4() {
    return _checkRool4(root);
}


template<typename baseType>
int RBTree<baseType>::_checkRool4(nodePtr localRoot) {
    if (localRoot == nullptr)
        return 1;
    else {
        int rightBalance = _checkRool4(localRoot->rightChild);
        int leftBalance = _checkRool4(localRoot->leftChild);
        if (rightBalance != leftBalance)
            cout << "Subtree " << localRoot->data() << " is not balanced." << endl;
        return rightBalance + (localRoot->color == BLACK);
    }
}

template<typename baseType>
void RBTree<baseType>::checkRool3() {
    _checkRool3(root);
}


template<typename baseType>
void RBTree<baseType>::_checkRool3(nodePtr localRoot) {
    if (localRoot != nullptr) {
        if (localRoot->color == RED && localRoot->parent->color == RED)
            cerr << "Root " << localRoot->parent->data()
                 << " is red and have red child " << localRoot->data() << "!"
                 << endl;

        _checkRool3(localRoot->rightChild);
        _checkRool3(localRoot->leftChild);
    }
}
