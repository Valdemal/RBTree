/*
    КРАСНО-ЧЕРНЫЕ ПРАВИЛА
    1. Каждый узел имеет красный или черный цвет
    2. Корень всегда черный
    3. Потомки красного узла всегда черные
    4. Черная высота левого и правого поддеревьев для любого узла должна быть
       одинакова
*/

#include <iostream>

using namespace std;

typedef enum {
    RED, BLACK
} color_t; //Тип - цвет узла


template<typename baseType>
class RBNode { //Класс "красно-черный" узел
private:
    //Тип - указатель на красно-черный узел с базовым типом типом baseType
    typedef RBNode<baseType> *nodePtr;

    baseType _data; // Данные, которые хранит узел
public:
    color_t color = RED; // Цвет узла(по умолчанию красный)

    nodePtr parent; // Родитель узла
    nodePtr leftChild; // Левый потомок
    nodePtr rightChild; // Правый потомок

    // Конструктор красно-черного узла
    explicit RBNode(baseType data) {
        this->_data = data;
        this->parent = this->leftChild = this->rightChild = nullptr;
    }

    // Перекрашивает узел
    void recolor() {
        this->color = color_t(!bool(this->color));
    }

    // Возвращает данные, которые хранит узел
    inline baseType data() {
        return _data;
    }
}; // Конец класса RBNode


template<typename baseType>
class RBTree { //Класс "красно-черное" дерево
private:
    //Тип - указатель на красно-черный узел с базовым типом типом baseType
    typedef RBNode<baseType> *nodePtr;

    nodePtr root; // Корень дерева

    // Выполняет левое вращаение относительно узла localRoot
    void leftRotate(nodePtr localRoot) {
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

    // Выполняет левое вращаение относительно узла localRoot
    void rightRotate(nodePtr localRoot) {
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

    /* Восстанавливает сбаланисированность дерева,
     * если красно-черные правило 3 нарушено при вставке узла */
    void afterInsertBalance(nodePtr current) {
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

    /* Если оба потомка узла parent - красные, перекрашивает потомком,
     * если сам parent при этом не является корнем перекрашивает его */
    void descentRecoloring(nodePtr parent) {
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

    /* Если узел current и его родитель красные,
     * восстанавливает сбалансированность на данном участке */
    void descentBalance(nodePtr current) {
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

public:
    // Конструктор красно-черного дерева
    RBTree() {
        root = nullptr;
    }

    // Вставляет узел с данными data в дерево
    void insert(baseType data) {
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

    // Возвращает значение ИСТИНА, если узел с ключем key есть в дереве,
    // иначе возвращает ЛОЖЬ
    bool find(baseType key) {
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

    // Возвращает максимальное значение ключа в дереве
    baseType max() {
        nodePtr current, last;
        current = root;
        while (current != nullptr) {
            last = current;
            current = current->rightChild;
        }
        return last->data();
    }

    // Возвращает минимальное значение ключа в дереве
    baseType min() {
        nodePtr current, last;
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
    inline void _inOrder(nodePtr localRoot, void function(baseType)) {
        if (localRoot != nullptr) {
            _inOrder(localRoot->leftChild, function);
            function(localRoot->data());
            _inOrder(localRoot->rightChild, function);
        }
    }

}; //Конец класса RBTree

void printInt(int x) {
    cout << x << " ";
}

int main() {
    RBTree<int> tree;

    for (int i = 0; i < 20; ++i) {
        tree.insert(i);
    }

    tree.inOrder(printInt);
}

