#include "../include/queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция для инициализации очереди
void init_queue(queue* q) {
    // Установка указателей на начало и конец очереди в NULL
    q->front = NULL;
    q->rear = NULL;
}

// Функция для добавления элемента в конец очереди
void push_queue(queue* q, const char* value) {
    // Выделение памяти для нового узла
    Node* newNode = (Node*)malloc(sizeof(Node));
    // Копирование данных в новый узел
    newNode->data = strdup(value);
    // Установка указателя на следующий узел в NULL
    newNode->next = NULL;

    // Если очередь пуста, новый узел становится и началом, и концом очереди
    if (q->rear == NULL) {
        q->front = newNode;
        q->rear = newNode;
    } 
    // Иначе, новый узел добавляется в конец очереди
    else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    // Вывод сообщения о добавлении элемента
    printf("Добавлено: %s\n", value);
}

// Функция для удаления элемента из начала очереди
char* pop_queue(queue* q) {
    // Проверка на пустоту очереди
    if (q->front == NULL) {
        printf("Очередь пуста!\n");
        return NULL;
    }

    // Извлекаем данные из первого узла
    Node* temp = q->front;
    // Копируем строку, чтобы вернуть её
    char* item = strdup(temp->data);

    // Перемещаем указатель front на следующий узел
    q->front = q->front->next;

    // Если очередь стала пустой, обновляем указатель rear
    if (q->front == NULL) {
        q->rear = NULL;
    }

    // Освобождение памяти, выделенной под данные и узел
    free(temp->data);
    free(temp);

    // Вывод сообщения об удалении элемента
    printf("Удалено: %s\n", item);
    return item;
}

// Функция для проверки, является ли очередь пустой
int isEmpty(queue* q) {
    // Возвращаем 1, если очередь пуста, иначе 0
    return (q->front == NULL);
}
