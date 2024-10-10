#include "../include/doubly_linked_list.h"

// Функция для создания нового узла двусвязного списка
Node* createDoublyLinkedListNode(const char *data) {
    // Выделение памяти для нового узла
    Node *newNode = (Node*)malloc(sizeof(Node));
    // Проверка на ошибку выделения памяти
    if (newNode == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(1);
    }
    // Копирование данных в новый узел
    newNode->data = strdup(data);
    // Инициализация указателей на следующий и предыдущий узлы
    newNode->next = NULL;
    newNode->prev = NULL;
    // Возвращение указателя на новый узел
    return newNode;
}

// Функция для инициализации двусвязного списка
void initDoublyLinkedList(DoublyLinkedList *list) {
    // Установка указателей на голову и хвост списка в NULL
    list->head = NULL;
    list->tail = NULL;
}

// Функция для добавления элемента в голову двусвязного списка
void addToDoublyLinkedListHead(DoublyLinkedList *list, const char *data) {
    // Создание нового узла
    Node *newNode = createDoublyLinkedListNode(data);
    // Если список пуст, голова и хвост указывают на новый узел
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        // Иначе, новый узел становится головой списка
        newNode->next = list->head;
        list->head->prev = newNode;
        list->head = newNode;
    }
}

// Функция для добавления элемента в хвост двусвязного списка
void addToDoublyLinkedListTail(DoublyLinkedList *list, const char *data) {
    // Создание нового узла
    Node *newNode = createDoublyLinkedListNode(data);
    // Если список пуст, голова и хвост указывают на новый узел
    if (list->tail == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        // Иначе, новый узел становится хвостом списка
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }
}

// Функция для удаления элемента с головы двусвязного списка
void removeFromDoublyLinkedListHead(DoublyLinkedList *list) {
    // Если список пуст, ничего не делаем
    if (list->head == NULL) {
        return;
    }
    // Сохранение указателя на текущую голову списка
    Node *temp = list->head;
    // Перемещение указателя головы на следующий элемент
    list->head = list->head->next;
    // Если новая голова не NULL, обновляем указатель на предыдущий элемент
    if (list->head != NULL) {
        list->head->prev = NULL;
    } else {
        // Если список стал пустым, обновляем указатель на хвост
        list->tail = NULL;
    }
    // Освобождение памяти, выделенной под данные и узел
    free(temp->data);
    free(temp);
}

// Функция для удаления элемента с хвоста двусвязного списка
void removeFromDoublyLinkedListTail(DoublyLinkedList *list) {
    // Если список пуст, ничего не делаем
    if (list->tail == NULL) {
        return;
    }
    // Сохранение указателя на текущий хвост списка
    Node *temp = list->tail;
    // Перемещение указателя хвоста на предыдущий элемент
    list->tail = list->tail->prev;
    // Если новый хвост не NULL, обновляем указатель на следующий элемент
    if (list->tail != NULL) {
        list->tail->next = NULL;
    } else {
        // Если список стал пустым, обновляем указатель на голову
        list->head = NULL;
    }
    // Освобождение памяти, выделенной под данные и узел
    free(temp->data);
    free(temp);
}

// Функция для удаления элемента по значению из двусвязного списка
void removeDoublyLinkedListNodeByValue(DoublyLinkedList *list, const char *data) {
    // Начало обхода списка с головы
    Node *current = list->head;
    // Поиск узла с указанным значением
    while (current != NULL) {
        if (strcmp(current->data, data) == 0) {
            // Обновление указателей предыдущего и следующего узлов
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                list->head = current->next;
            }
            if (current->next != NULL) {
                current->next->prev = current->prev;
            } else {
                list->tail = current->prev;
            }
            // Освобождение памяти, выделенной под данные и узел
            free(current->data);
            free(current);
            return;
        }
        // Переход к следующему узлу
        current = current->next;
    }
}

// Функция для поиска элемента по значению в двусвязном списке
Node* findDoublyLinkedListNodeByValue(DoublyLinkedList *list, const char *data) {
    // Начало обхода списка с головы
    Node *current = list->head;
    // Поиск узла с указанным значением
    while (current != NULL) {
        if (strcmp(current->data, data) == 0) {
            return current;
        }
        // Переход к следующему узлу
        current = current->next;
    }
    // Если узел не найден, возвращаем NULL
    return NULL;
}

// Функция для вывода двусвязного списка
void printDoublyLinkedList(DoublyLinkedList *list) {
    // Начало обхода списка с головы
    Node *current = list->head;
    // Вывод данных каждого узла
    while (current != NULL) {
        printf("%s -> ", current->data);
        current = current->next;
    }
    // Вывод конца списка
    printf("NULL\n");
}

// Функция для освобождения памяти, выделенной под двусвязный список
void freeDoublyLinkedList(DoublyLinkedList *list) {
    // Начало обхода списка с головы
    Node *current = list->head;
    // Освобождение памяти для каждого узла
    while (current != NULL) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    // Установка указателей на голову и хвост в NULL
    list->head = NULL;
    list->tail = NULL;
}
