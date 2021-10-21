#ifndef RBTREE_RBTREE_H
#define RBTREE_RBTREE_H

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

//Тип - цвет узла
typedef enum {
    RED, BLACK
} color_t;

template<typename baseType>
class RBNode { //Класс "красно-черный" узел
private:
    //Тип - указатель на красно-черный узел с базовым типом типом baseType
    typedef RBNode<baseType> *nodePtr;

    baseType _data; // Данные, которые хранит узел
public:
    color_t color = RED; // Цвет узла
    nodePtr parent; // Родитель узла
    nodePtr leftChild; // Левый потомок
    nodePtr rightChild; // Правый потомок

    // Конструктор красно-черного узла
    explicit RBNode(baseType data);

    // Возвращает данные, которые хранит узел
    inline baseType data();

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

    // Удаление узла хранящего данные key из дерева
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

    /* Если узел current и его родитель красные,
     * восстанавливает сбалансированность в данном поддереве */
    void insertBalance(nodePtr current);

    //ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ УДАЛЕНИЯ

    // Возвращает указатель на узел-приемник узла delNode
    nodePtr getSuccessor(nodePtr delNode);

    // Удаление узла delNode с 1 потомком
    void removeNode1Child(nodePtr delNode);

    // Удаление delNode узла без потомков
    void removeNode0Children(nodePtr delNode);

    // Выполняет балансировку узла balanceNode,
    // черный потомок которого был удален
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
    int checkRule4();
    int _checkRule4(nodePtr localRoot);
    int checkRule3();
    int _checkRule3(nodePtr localRoot);

}; //Конец класса RBTree


#endif //RBTREE_RBTREE_H
