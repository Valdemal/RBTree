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

// Перекрашивает узел
template<typename baseType>
void RBNode<baseType>::recolor() {
    this->color = color_t(!bool(this->color));
}

/* Меняет значения ключей данного узла со значением ключа узла
* node (нужно для удаления) */
template<typename baseType>
void RBNode<baseType>::swapKeys(nodePtr node) {
    baseType temp = this->_data;
    this->_data = node->_data;
    node->_data = temp;
}
// Конец реализации красно-черного узла RBNode

// Реализация красно-черного дерева RBTreeApp

template<typename baseType>
RBTree<baseType>::RBTree() {
    root = nullptr;
}

// РЕАЛИЗАЦИЯ ПОИСКА

template<typename baseType>
bool RBTree<baseType>::find(baseType key) {
    if (root == nullptr)
        return false;

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

// Вставляет узел с данными key в дерево, возвращает значение ИСТИНА,
// если данные вставлены успешно, ЛОЖЬ, если такой ключ уже есть в дереве
template<typename baseType>
bool RBTree<baseType>::insert(baseType key) {
    // Если дерево пустое
    if (root == nullptr) {
        //Создаем корень черного цвета
        root = new RBNode<baseType>(key);
        root->color = BLACK;
    } else {
        nodePtr current = root, parent;

        // Перемещение вниз и поиск места для вставки
        while (true) {
            parent = current;

            if (key < current->data()) {
                current = current->leftChild;
                if (current == nullptr) {
                    // Вставка узла
                    parent->leftChild = current = new RBNode<baseType>(key);
                    break;
                }
            } else if (key > current->data()) {
                current = current->rightChild;
                if (current == nullptr) {
                    // Вставка узла
                    parent->rightChild = current = new RBNode<baseType>(key);
                    break;
                }
            } else {
                //Значения ключей по условию не могут совпадать
                return false;
            }
            //Перекрашивание parent и его потомков при необходимости
            descentRecoloring(parent);

            // Если узел parent и его родитель красные, восстанавливает
            // сбалансированность в данном поддереве
            if (parent->color == RED && parent->parent->color == RED) {
                insertBalance(parent);
            }
        }

        current->parent = parent;

        // Балансировка дерева при нарушении красно-черных
        // правил после вставки
        if (current->parent->color == RED) {
            insertBalance(current);
        }
    }
    return true;
}

// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ВСТАВКИ

/* Если оба потомка узла parent - красные, перекрашивает потомком,
* если сам parent при этом не является корнем перекрашивает его */
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
            //Перекрашиваем родителя, если это не нарушает правило 2
            if (parent != root)
                parent->recolor();
        }
    }
}

/* Если узел balanceNode и его родитель красные,
* восстанавливает сбалансированность в данном поддереве */
template<typename baseType>
void RBTree<baseType>::insertBalance(nodePtr balanceNode) {
    nodePtr grandparent = balanceNode->parent->parent;
    bool isRightChild = balanceNode->parent->rightChild == balanceNode;
    bool isLeftChild = balanceNode->parent->leftChild == balanceNode;
    bool isLeftGrandparent = grandparent->leftChild == balanceNode->parent;
    bool isRightGrandparent = grandparent->rightChild == balanceNode->parent;

    grandparent->recolor();
    if (isRightGrandparent && isRightChild ||
        isLeftGrandparent && isLeftChild) {
        //Внешний внук
        balanceNode->parent->recolor();
        if (isLeftChild)
            rightRotate(grandparent);
        else
            leftRotate(grandparent);
    } else {
        //Внутренний внук
        balanceNode->color = BLACK;
        if (isRightChild) {
            leftRotate(balanceNode->parent);
            rightRotate(grandparent);
        } else {
            rightRotate(balanceNode->parent);
            leftRotate(grandparent);
        }
    }
}

// Выполняет левое вращаение относительно узла localRoot
template<typename baseType>
void RBTree<baseType>::leftRotate(nodePtr localRoot) {
    nodePtr newRoot = localRoot->rightChild;
    localRoot->rightChild = newRoot->leftChild;

    // Переназначение родителя левого потомка нового корня
    if (newRoot->leftChild != nullptr)
        newRoot->leftChild->parent = localRoot;

    newRoot->parent = localRoot->parent;

    // Переназначение родителя localRoot
    if (localRoot->parent == nullptr)
        // localRoot - корень
        this->root = newRoot;
    else if (localRoot == localRoot->parent->leftChild)
        // localRoot - левый потомок
        localRoot->parent->leftChild = newRoot;
    else
        // localRoot - правый потомок
        localRoot->parent->rightChild = newRoot;

    newRoot->leftChild = localRoot;
    localRoot->parent = newRoot;
}

// Выполняет правое вращаение относительно узла localRoot
template<typename baseType>
void RBTree<baseType>::rightRotate(nodePtr localRoot) {
    nodePtr newRoot = localRoot->leftChild;
    localRoot->leftChild = newRoot->rightChild;

    // Переназначение родителя правого потомка нового корня
    if (newRoot->rightChild != nullptr) {
        newRoot->rightChild->parent = localRoot;
    }

    newRoot->parent = localRoot->parent;

    // Переназначение родителя localRoot
    if (localRoot->parent == nullptr) {
        // localRoot - корень
        this->root = newRoot;
    } else if (localRoot == localRoot->parent->rightChild) {
        // localRoot - правый потомок
        localRoot->parent->rightChild = newRoot;
    } else {
        // localRoot - левый потомок
        localRoot->parent->leftChild = newRoot;
    }

    newRoot->rightChild = localRoot;
    localRoot->parent = newRoot;
}

//РЕАЛИЗАЦИЯ УДАЛЕНИЯ

// Удаление узла, хранящего данные key из дерева, возвращяет значение
// ИСТИНА, если узел успешно удален, ЛОЖЬ, если узел не найден
template<typename baseType>
bool RBTree<baseType>::remove(baseType key) {

    if (root == nullptr) {
        // Дерево пустое, удаление невозможно
        return false;
    }

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
        removeNode0Child(current);
    else if (childCnt == 1)
        removeNode1Child(current);
    else {
        removeNode2Child(current);
    }

    return true;
}

// Возвращает указатель на узел-приемник узла delNode
template<typename baseType>
RBNode<baseType> *RBTree<baseType>::getSuccessor(nodePtr delNode) {
    nodePtr successor = delNode;
    nodePtr current = delNode->rightChild;

    while (current != nullptr) {
        successor = current;
        current = current->leftChild;
    }

    return successor;
}

// Удаление узла delNode с 2-мя потомками
template<typename baseType>
void RBTree<baseType>::removeNode2Child(nodePtr delNode) {
    /* Сведение задачи удаления узла с двумя потомками, к удалению узла
     * с одним потомком или без потомков */

    nodePtr successor = getSuccessor(delNode); // Поиск приемника узла
    successor->swapKeys(delNode); // Обмен ключами

    // Если у приемника есть потомок (может быть только правый)
    if (successor->rightChild)
        removeNode1Child(successor);
    else
        removeNode0Child(successor);
}

// Удаление узла delNode с 1 потомком
template<typename baseType>
void RBTree<baseType>::removeNode1Child(nodePtr delNode) {
    // По свойсву красно-черного дерева один потомок может быть
    // только у черного узла и этот потомок красный
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

template<typename baseType>
void RBTree<baseType>::removeNode0Child(nodePtr delNode) {
    // Если удаляемый узел - корень
    if (delNode == root)
        root = nullptr;
    else {
        nodePtr parent = delNode->parent;
        // Брат удалямого узла понадобится при баласнировке
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

// Выполняет балансировку поддерева parent, потомок которого был удален,
// brother - брат удаленного узла
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

// Выполняет баласировку узла в случае если его родитель(parent) красный,
// а его брат(brother) черный.
// isLeftBrother - логическое значение, которое истинно , если брат левый,
// ложно если брат правый, передается по ссылке для экономии памяти
template<typename baseType>
void RBTree<baseType>::removeBalanceRB(nodePtr parent, nodePtr brother,
                                       const bool &isLeftBrother) {
    // Перекрашивание предка в любом случае
    parent->recolor();

    // Левый потомок брата существует и он красный
    if (brother->leftChild &&
        brother->leftChild->color == RED) {
        if (isLeftBrother) {
            // Случай 1 для левого брата
            brother->recolor();
            brother->leftChild->recolor();
            rightRotate(parent);
        } else {
            // Случай 2 для правого брата
            rightRotate(brother);
            leftRotate(parent);
        }

    }
    // Правый потомок брата существует и он красный
    else if (brother->rightChild &&
             brother->rightChild->color == RED) {
        if (isLeftBrother) {
            // Случай 2 для левого брата
            leftRotate(brother);
            rightRotate(parent);
        } else {
            // Случай 1 для правого брата
            brother->recolor();
            brother->rightChild->recolor();
            leftRotate(parent);
        }
    }
    // Оба потомка брата черные или пустые(тоже черные)
    else {
        // Общий случай для левого и правого брата
        brother->recolor();
    }
}

template<typename baseType>
void RBTree<baseType>::removeBalanceBB(nodePtr parent, nodePtr brother,
                                       const bool &isLeftBrother) {

    // Правый потомок брата существует и он красный
    if (brother->rightChild && brother->rightChild->color == RED) {
        brother->rightChild->recolor();
        if (isLeftBrother) {
            leftRotate(brother);
            rightRotate(parent);
        } else {
            leftRotate(parent);
        }
    }
    // Левый потомок брата существует и он красный
    else if (brother->leftChild && brother->leftChild->color == RED) {
        brother->leftChild->recolor();
        if (isLeftBrother) {
            rightRotate(parent);
        } else {
            rightRotate(brother);
            leftRotate(parent);
        }
    }
    // Оба потомка брата черные или пустые(тоже черные)
    else {
        brother->recolor();
        // Если parent - корень, двигаться выше уже нельзя. Заканчиваем баласировку
        if (parent->parent) {
            // Вызываем рекурсивно балансировку для родителя, в качестве
            // нового родителя передаем родителя родителя, а в качестве брата
            // - брата родителя
            brother = parent->parent->leftChild == parent ?
                      parent->parent->rightChild : parent->parent->leftChild;
            removeBalance(parent->parent, brother);
        }
    }
}

// Выполняет баласировку узла в случае если его родитель(parent) черный,
// а его брат(brother) красный.
// isLeftBrother - логическое значение, которое истинно , если брат левый,
// ложно если брат правый, передается по ссылке для экономии памяти
template<typename baseType>
void RBTree<baseType>::removeBalanceBR(nodePtr parent, nodePtr brother,
                                       const bool &isLeftBrother) {
    // Перекрашиваем брата и предка
    brother->recolor();
    parent->recolor();
    // Рекурсивно сводим к случаю, когда родитель красный, а брат черный,
    // предварительно выполняя поворот с левую или правую сторону в зависимости
    // от строны брата
    if (isLeftBrother) {
        rightRotate(parent);
        removeBalanceRB(parent, parent->leftChild, true);
    } else {
        leftRotate(parent);
        removeBalanceRB(parent, parent->rightChild, false);
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

// Конец реализации красно-черного дерева RBTreeApp

// Отладка

template<typename baseType>
int RBTree<baseType>::checkRule5() {
    return _checkRule5(root);
}


template<typename baseType>
int RBTree<baseType>::_checkRule5(nodePtr localRoot) {
    if (localRoot == nullptr)
        return 1;
    else {
        int rightBalance = _checkRule5(localRoot->rightChild);
        int leftBalance = _checkRule5(localRoot->leftChild);
        if (rightBalance != leftBalance)
            cout << "Subtree " << localRoot->data() << " is not balanced." << endl;
        return rightBalance + (localRoot->color == BLACK);
    }
}

template<typename baseType>
int RBTree<baseType>::checkRule3() {
    return _checkRule3(root);
}


template<typename baseType>
int RBTree<baseType>::_checkRule3(nodePtr localRoot) {
    if (localRoot != nullptr) {
        if (localRoot->color == RED && localRoot->parent->color == RED) {
            cerr << "Root " << localRoot->parent->data()
                 << " is red and have red child " << localRoot->data() << "!"
                 << endl;
            return 1;
        }

        return _checkRule3(localRoot->rightChild) || _checkRule3(localRoot->leftChild);
    }
    return 0;
}
