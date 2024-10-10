#include"../include/single_linked_list.h"

// Функция для создания нового узла списка
Node *create_node(const char *value) {
    // Выделение памяти для нового узла
    Node *new_node = (Node*)malloc(sizeof(Node));
    // Выделение памяти для данных узла
    new_node->data = (char*)malloc(strlen(value) + 1);

    // Копирование строки в новый узел
    strcpy(new_node->data, value);
    // Установка указателя на следующий узел в NULL
    new_node->next = NULL;

    // Возвращение указателя на новый узел
    return new_node;
}

// Функция для создания нового списка
LinkedList *create_list() {
    // Выделение памяти для нового списка
    LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
    // Установка указателя на голову списка в NULL
    list->head = NULL;
    // Возвращение указателя на новый список
    return list;
}

// Функция добавления элемента в конец списка
void push_back(LinkedList *list, const char *value) {
    // Создание нового узла
    Node *new_node = create_node(value);

    // Если список пуст, новый узел становится головой списка
    if (list->head == NULL) {
        list->head = new_node;
    } 
    // Иначе, новый узел добавляется в конец списка
    else {
        Node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Функция добавления элемента в начало списка
void push_front(LinkedList *list, const char *value) {
    // Создание нового узла
    Node *new_node = create_node(value);
    // Новый узел указывает на текущую голову списка
    new_node->next = list->head;
    // Новый узел становится головой списка
    list->head = new_node;
}

// Функция удаления элемента с начала списка
void pop_front(LinkedList *list) {
    // Если список пуст, ничего не делаем
    if (list->head == NULL) return;

    // Сохранение указателя на текущую голову списка
    Node *tmp = list->head;
    // Перемещение указателя головы на следующий элемент
    list->head = list->head->next;
    // Освобождение памяти, выделенной под данные и узел
    free(tmp->data);
    free(tmp);
}

// Функция удаления элемента с конца списка
void pop_back(LinkedList *list) {
    // Если список пуст, ничего не делаем
    if (list->head == NULL) return;

    // Если в списке один элемент
    if (list->head->next == NULL) {
        // Освобождение памяти, выделенной под данные и узел
        free(list->head->data);
        free(list->head);
        // Установка указателя головы в NULL
        list->head = NULL;
    } 
    // Иначе, ищем предпоследний узел
    else {
        Node *current = list->head;
        while (current->next->next != NULL) {
            current = current->next;
        }
        // Освобождение памяти, выделенной под данные и последний узел
        free(current->next->data);
        free(current->next);
        // Предпоследний узел становится последним
        current->next = NULL;
    }
}

// Функция поиска элемента по значению
Node* find_value(LinkedList *list, const char *value) {
    // Начало обхода списка с головы
    Node *current = list->head;
    // Поиск узла с указанным значением
    while (current != NULL) {
        if (strcmp(current->data, value) == 0) {
            // Возвращаем узел, если значение найдено
            return current;
        }
        current = current->next;
    }
    // Если элемент не найден, возвращаем NULL
    return NULL;
}

// Функция удаления элемента по значению
void remove_value(LinkedList *list, const char *value) {
    // Если список пуст, ничего не делаем
    if (list->head == NULL) return;

    // Если удаляемый элемент — первый
    if (strcmp(list->head->data, value) == 0) {
        Node *tmp = list->head;
        list->head = list->head->next;
        free(tmp->data);
        free(tmp);
        return;
    }

    // Ищем элемент для удаления
    Node *current = list->head;
    while (current->next != NULL && strcmp(current->next->data, value) != 0) {
        current = current->next;
    }

    // Если элемент не найден, выводим сообщение
    if (current->next == NULL) {
        printf("Элемент с значением '%s' не найден.\n", value);
        return;
    }

    // Удаляем элемент
    Node *tmp = current->next;
    current->next = current->next->next;
    free(tmp->data);
    free(tmp);
}

// Функция вывода списка на экран
void print_list(LinkedList *list) {
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

// Функция освобождения памяти списка
void free_list(LinkedList *list) {
    // Начало обхода списка с головы
    Node *current = list->head;
    // Освобождение памяти для каждого узла
    while (current != NULL) {
        Node *tmp = current;
        current = current->next;
        free(tmp->data);
        free(tmp);
    }
    // Освобождаем память структуры списка
    free(list);
}
