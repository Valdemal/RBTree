#ifndef RBTREE_RBTREE_H
#define RBTREE_RBTREE_H

#include <iostream>

using namespace std;

template<typename baseType>
class RBNode { //Класс "красно-черный" узел
private:
    //Тип - указатель на красно-черный узел с базовым типом типом baseType
    typedef RBNode<baseType> *nodePtr;

    baseType _data; // Данные, которые хранит узел
    bool _isRed = true; // Цвет узла (по умолчанию красный)
public:
    nodePtr parent; // Родитель узла
    nodePtr leftChild; // Левый потомок
    nodePtr rightChild; // Правый потомок

    // Конструктор красно-черного узла
    explicit RBNode(baseType data);

    // Возвращает данные, которые хранит узел
    inline baseType data();

    // Возвращает значение ИСТИНА, если цвет узла - красный, иначе ЛОЖЬ
    inline bool isRed();

    // Возвращает значение ИСТИНА, если цвет узла - черный, иначе ЛОЖЬ
    inline bool isBlack();

    // Перекрашивает узел в противоположный цвет
    void recolor();

    /* Меняет значения ключей данного узла со значением ключа узла
     * node (нужно для удаления) */
    void swapKeys(nodePtr node);
}; // Конец класса RBNode

template<typename baseType>
class RBTree { //Класс "красно-черное" дерево
private:
    //Тип - указатель на красно-черный узел с базовым типом типом baseType
    typedef RBNode<baseType> *nodePtr;
    nodePtr root; // Корень дерева
public:
    // Конструктор красно-черного дерева
    RBTree();

    // Деструктор красно-черного дерева
    ~RBTree();

    // Вставляет узел с данными key в дерево, возвращает значение ИСТИНА,
    // если данные вставлены успешно, ЛОЖЬ, если такой ключ уже есть в дереве
    bool insert(baseType key);

    // Удаление узла, хранящего данные key из дерева, возвращяет значение
    // ИСТИНА, если узел успешно удален, ЛОЖЬ, если узел не найден
    bool remove(baseType key);

    // Возвращает значение ИСТИНА, если узел с ключем key есть в дереве,
    // иначе возвращает ЛОЖЬ
    bool find(baseType key);

    // Возвращает максимальное значение ключа в дереве
    baseType max();

    // Возвращает минимальное значение ключа в дереве
    baseType min();

    // Выполняет симметричный обход дерева, вызывая для каждого узла функцию function
    void inOrder(void function(baseType));
private:
    // Выполняет симметричный обход начиная с localRoot, вызывая для каждого
    // узла функцию function
    inline void _inOrder(nodePtr localRoot, void function(baseType));

    //ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ВСТАВКИ

    // Выполняет левое вращаение относительно узла localRoot
    void leftRotate(nodePtr localRoot);

    // Выполняет левое вращаение относительно узла localRoot
    void rightRotate(nodePtr localRoot);

    /* Если оба потомка узла parent - красные, перекрашивает потомком,
     * если сам parent при этом не является корнем перекрашивает его */
    void descentRecoloring(nodePtr parent);

    /* Если узел balanceNode и его родитель красные,
     * восстанавливает сбалансированность в данном поддереве */
    void insertBalance(nodePtr balanceNode);

    //ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ УДАЛЕНИЯ

    // Возвращает указатель на узел-приемник узла delNode
    nodePtr getSuccessor(nodePtr delNode);

    // Удаление delNode узла без потомков
    void removeNode0Child(nodePtr delNode);

    // Удаление узла delNode с 1 потомком
    void removeNode1Child(nodePtr delNode);

    // Удаление узла delNode с 2 потомком
    void removeNode2Child(nodePtr delNode);

    // Выполняет балансировку поддерева parent, потомок которого был удален,
    // brother - брат удаленного узла
    void removeBalance(nodePtr parent, nodePtr brother);

    // Выполняет баласировку узла в случае если его родитель(parent) красный,
    // а его брат(brother) черный.
    // isLeftBrother - логическое значение, которое истинно , если брат левый,
    // ложно если брат правый, передается по ссылке для экономии памяти
    void removeBalanceRB(nodePtr parent, nodePtr brother,
                         const bool &isLeftBrother);

    // Выполняет баласировку узла в случае если его родитель(parent) черный,
    // а его брат(brother) красный.
    // isLeftBrother - логическое значение, которое истинно , если брат левый,
    // ложно если брат правый, передается по ссылке для экономии памяти
    void removeBalanceBR(nodePtr parent, nodePtr brother,
                         const bool &isLeftBrother);

    // Выполняет баласировку узла в случае если его родитель(parent) черный и
    // брат(brother) тоже черный.
    // isLeftBrother - логическое значение, которое истинно , если брат левый,
    // ложно если брат правый, передается по ссылке для экономии памяти
    void removeBalanceBB(nodePtr parent, nodePtr brother,
                         const bool &isLeftBrother);

    //Освобождает память, которую занимает поддерево с корнем в узле localRoot
    void deleteTree(nodePtr localRoot);

public:
    // Отладка и диагностика

    // Проверка правила равенства черной высоты
    // левого и правого поддеревьев для каждого узла
    int checkRule5();
    int _checkRule5(nodePtr localRoot);

    // Проверка правила 3(потомки красного вседа черные) для всех узлов
    int checkRule3();
    int _checkRule3(nodePtr localRoot);

}; //Конец класса RBTreeApp

#include "RBTree.tpp"

#endif //RBTREE_RBTREE_H
