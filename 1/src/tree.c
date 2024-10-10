#include "../include/tree.h"

// Функция для создания нового узла
TreeNode* newNode(int data) {
    // Выделение памяти для нового узла
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    // Установка данных узла
    node->data = data;
    // Установка указателей на потомков в NULL
    node->left = NULL;
    node->right = NULL;
    // Возвращение указателя на новый узел
    return node;
}

// Функция для добавления элемента в полное бинарное дерево
TreeNode* insert(TreeNode* root, int data) {
    // Если дерево пустое, создаем новый узел
    if (root == NULL)
        return newNode(data);

    // Если левое поддерево пустое, добавляем элемент туда
    if (root->left == NULL) {
        root->left = insert(root->left, data);
    } 
    // Если правое поддерево пустое, добавляем элемент туда
    else if (root->right == NULL) {
        root->right = insert(root->right, data);
    } 
    // Если у корня есть оба потомка, добавляем в левое поддерево
    else {
        root->left = insert(root->left, data);
    }
    // Возвращение корня дерева
    return root;
}

// Функция для поиска элемента в дереве
bool search(TreeNode* root, int data) {
    // Если дерево пустое, элемент не найден
    if (root == NULL)
        return false;

    // Если данные узла совпадают с искомым значением, элемент найден
    if (root->data == data)
        return true;

    // Рекурсивный поиск в левом и правом поддеревьях
    return search(root->left, data) || search(root->right, data);
}

// Функция для проверки, является ли дерево полным бинарным деревом
bool isFullBinaryTree(TreeNode* root) {
    // Если дерево пустое, считаем его полным
    if (root == NULL)
        return true;

    // Проверка, что узел либо лист, либо имеет два потомка
    if ((root->left == NULL && root->right != NULL) || (root->left != NULL && root->right == NULL))
        return false;

    // Рекурсивная проверка левого и правого поддеревьев
    return isFullBinaryTree(root->left) && isFullBinaryTree(root->right);
}

// Функция для вывода дерева (обход в прямом порядке)
void printTree(TreeNode* root, FILE* file) {
    // Если дерево пустое, выходим из функции
    if (root == NULL)
        return;

    // Вывод данных узла
    fprintf(file, "%d ", root->data);
    // Рекурсивный вывод левого поддерева
    printTree(root->left, file);
    // Рекурсивный вывод правого поддерева
    printTree(root->right, file);
}

// Функция для создания нового дерева
BinaryTree* createBinaryTree() {
    // Выделение памяти для нового дерева
    BinaryTree* tree = (BinaryTree*)malloc(sizeof(BinaryTree));
    // Установка корня дерева в NULL
    tree->root = NULL;
    // Возвращение указателя на новое дерево
    return tree;
}

// Функция для добавления элемента в дерево
void insertElement(BinaryTree* tree, int data) {
    // Вызов функции добавления элемента с корнем дерева
    tree->root = insert(tree->root, data);
}

// Функция для поиска элемента в дереве
bool searchElement(BinaryTree* tree, int data) {
    // Вызов функции поиска элемента с корнем дерева
    return search(tree->root, data);
}

// Функция для проверки, является ли дерево полным бинарным деревом
bool isFull(BinaryTree* tree) {
    // Вызов функции проверки с корнем дерева
    return isFullBinaryTree(tree->root);
}

// Функция для вывода дерева
void print(BinaryTree* tree, FILE* file) {
    // Вызов функции вывода дерева с корнем дерева
    printTree(tree->root, file);
    // Вывод пустой строки для разделения
    fprintf(file, "\n");
}
