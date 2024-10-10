#include "../include/stack.h"

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 

// Функция для добавления элемента в стек
stack* push(stack* top, const char* value) {
    // Выделение памяти для нового узла стека
    stack* ptr = (stack*)malloc(sizeof(stack));
    // Копирование строки в новый узел
    ptr->data = strdup(value);
    // Установка указателя на следующий элемент
    ptr->next = top;
    // Возвращение нового узла как вершины стека
    return ptr;
}

// Функция для удаления элемента из стека
stack* pop(stack* top) {
    // Если стек пуст, возвращаем NULL
    if (top == NULL) return NULL;

    // Сохранение указателя на следующий элемент
    stack* ptr = top->next;
    // Освобождение памяти, выделенной под данные и узел
    free(top->data);
    free(top);
    // Возвращение новой вершины стека
    return ptr;
}

// Функция для вывода содержимого стека
void show(const stack* top) {
    // Начало обхода стека с вершины
    const stack* current = top;

    // Вывод данных каждого узла
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }

    // Вывод пустой строки для разделения
    printf("\n");
}

// Функция для освобождения памяти, выделенной под стек
void free_stack(stack* top) {
    // Пока стек не пуст, удаляем элементы
    while (top != NULL) {
        top = pop(top);
    }
}

// Функция для реверсирования стека
stack* reverse_stack(stack* top) {
    // Создание нового стека для хранения элементов в обратном порядке
    stack* new_top = NULL;
    // Пока исходный стек не пуст, переносим элементы в новый стек
    while (top != NULL) {
        new_top = push(new_top, top->data);
        top = pop(top);
    }
    // Возвращение новой вершины реверсированного стека
    return new_top;
}
