#include "../include/single_linked_list.h"  
#include "../include/dbms.h"                
#include "../include/tree.h"                
#include <stdio.h>                          
#include <string.h>                        
#include <stdlib.h>                         

QueryData parse_query(const char* query) {
    QueryData data;                          // Объявляем структуру для хранения данных запроса
    memset(&data, 0, sizeof(data));          // Инициализируем структуру нулями

    if (sscanf(query, "%s %s %s %s", data.command, data.variable, data.key, data.value) == 4 ||
        sscanf(query, "%s %s %s", data.command, data.variable, data.value) == 3 ||
        sscanf(query, "%s %s", data.command, data.variable) == 2) {
        return data;                         // Парсим запрос и возвращаем структуру с данными
    }

    fprintf(stderr, "Invalid query format\n"); // Выводим сообщение об ошибке, если формат запроса некорректен
    data.command[0] = '\0';                  // Очищаем команду в структуре
    return data;                             // Возвращаем структуру с пустой командой
}

void read_tokens(DBMS *dbms, char *value) {
    char *token = strtok(value, " ");        // Разбиваем строку на токены по пробелам
    while (token != NULL) {
        switch (dbms->v_type) {              // Определяем тип структуры данных
            case _STACK: dbms->data.stack = push(dbms->data.stack, token); break; // Добавляем токен в стек
            case _SINGLE_LINKED_LIST: push_back(dbms->data.list, token); break; // Добавляем токен в односвязный список
            case _ARR: append_string(dbms->data.arr, token); break; // Добавляем токен в массив строк
            case _HASH_MAP: {
                char key[50], val[50];
                if (sscanf(token, "%49[^:]:%49s", key, val) == 2) {
                    dbms->data.hash_map = hash_map_insert(dbms->data.hash_map, key, val); // Добавляем пару ключ-значение в хеш-таблицу
                }
                break;
            }
            case _QUEUE: push_queue(dbms->data.queue, token); break; // Добавляем токен в очередь
            case _DOUBLE_LINKED_LIST: addToDoublyLinkedListTail(dbms->data.dlist, token); break; // Добавляем токен в двусвязный список
            case _TREE: insertElement(dbms->data.tree, atoi(token)); break; // Добавляем токен в дерево
            case _SET: set_add(dbms->data.set, atoi(token)); break; // Добавляем токен в множество
        }
        token = strtok(NULL, " ");           // Переходим к следующему токену
    }
}

DBMS *read_from_file(DBMS *dbms, QueryData q_data, const char* filename) {
    FILE* file = fopen(filename, "r");       // Открываем файл для чтения
    if (!file) {
        perror("Failed to open file");       // Выводим сообщение об ошибке, если файл не открыт
        return NULL;                         // Возвращаем NULL, если файл не открыт
    }

    char line[256], name[256], value[256];
    while (fgets(line, sizeof(line), file)) { // Читаем файл построчно
        line[strcspn(line, "\n")] = 0;       // Удаляем символ новой строки
        if (sscanf(line, "%s = %[^\n]", name, value) == 2 && strcmp(name, q_data.variable) == 0) {
            read_tokens(dbms, value);        // Если имя переменной совпадает, читаем токены
        }
    }

    fclose(file);                            // Закрываем файл
    return dbms;                             // Возвращаем обновленный объект DBMS
}

int is_empty(DBMS *dbms) {
    switch (dbms->v_type) {                  // Определяем тип структуры данных
        case _STACK: return dbms->data.stack == NULL; // Проверяем, пуст ли стек
        case _SINGLE_LINKED_LIST: return dbms->data.list->head == NULL; // Проверяем, пуст ли односвязный список
        case _ARR: return dbms->data.arr->size == 0; // Проверяем, пуст ли массив строк
        case _HASH_MAP: {
            for (size_t i = 0; i < dbms->data.hash_map->size; i++) {
                if (dbms->data.hash_map->entries[i].key) return 0; // Проверяем, пуста ли хеш-таблица
            }
            return 1;
        }
        case _QUEUE: return dbms->data.queue->front == NULL; // Проверяем, пуста ли очередь
        case _DOUBLE_LINKED_LIST: return dbms->data.dlist->head == NULL; // Проверяем, пуст ли двусвязный список
        case _TREE: return dbms->data.tree->root == NULL; // Проверяем, пусто ли дерево
        case _SET: return dbms->data.set->size == 0; // Проверяем, пусто ли множество
    }
    return 1;                                // Возвращаем 1, если структура данных пуста
}

void write_tokens(FILE *file, DBMS *dbms, const char *name) {
    if (is_empty(dbms)) return;              // Если структура данных пуста, выходим из функции

    switch (dbms->v_type) {                  // Определяем тип структуры данных
        case _STACK: {
            stack* current = dbms->data.stack;
            while (current) {
                fprintf(file, " %s", current->data); // Записываем данные стека в файл
                current = current->next;
            }
            break;
        }
        case _SINGLE_LINKED_LIST: {
            Node* current = dbms->data.list->head;
            while (current) {
                fprintf(file, " %s", current->data); // Записываем данные односвязного списка в файл
                current = current->next;
            }
            break;
        }
        case _ARR: {
            for (int i = 0; i < dbms->data.arr->size; i++) {
                fprintf(file, " %s", dbms->data.arr->data[i]); // Записываем данные массива строк в файл
            }
            break;
        }
        case _HASH_MAP: {
            for (size_t i = 0; i < dbms->data.hash_map->size; i++) {
                if (dbms->data.hash_map->entries[i].key) {
                    fprintf(file, " %s:%s", dbms->data.hash_map->entries[i].key, dbms->data.hash_map->entries[i].value); // Записываем данные хеш-таблицы в файл
                }
            }
            break;
        }
        case _QUEUE: {
            Node* current = dbms->data.queue->front;
            while (current) {
                fprintf(file, " %s", current->data); // Записываем данные очереди в файл
                current = current->next;
            }
            break;
        }
        case _DOUBLE_LINKED_LIST: {
            Node* current = dbms->data.dlist->head;
            while (current) {
                fprintf(file, " %s", current->data); // Записываем данные двусвязного списка в файл
                current = current->next;
            }
            break;
        }
        case _TREE: {
            printTree(dbms->data.tree->root, file); // Записываем данные дерева в файл
            break;
        }
        case _SET: {
            for (size_t i = 0; i < dbms->data.set->size; i++) {
                fprintf(file, " %d", dbms->data.set->elements[i]); // Записываем данные множества в файл
            }
            break;
        }
    }
}

void write_to_file(DBMS *dbms, QueryData q_data, const char* filename) {
    FILE* file = fopen(filename, "r");       // Открываем файл для чтения
    if (!file) {
        perror("Failed to open file for reading"); // Выводим сообщение об ошибке, если файл не открыт
        return;                              // Выходим из функции
    }

    FILE* temp_file = fopen("temp.txt", "w"); // Открываем временный файл для записи
    if (!temp_file) {
        perror("Failed to open temp file for writing"); // Выводим сообщение об ошибке, если временный файл не открыт
        fclose(file);                        // Закрываем файл для чтения
        return;                              // Выходим из функции
    }

    char line[256], name[256], value[256];
    int found = 0;

    while (fgets(line, sizeof(line), file)) { // Читаем файл построчно
        line[strcspn(line, "\n")] = 0;       // Удаляем символ новой строки
        if (sscanf(line, "%s = %[^\n]", name, value) == 2) {
            if (strcmp(name, q_data.variable) == 0) {
                if (!is_empty(dbms)) {
                    fprintf(temp_file, "%s =", name); // Записываем имя переменной во временный файл
                    write_tokens(temp_file, dbms, name); // Записываем данные во временный файл
                    fprintf(temp_file, "\n"); // Добавляем символ новой строки
                }
                found = 1;                    // Устанавливаем флаг, что переменная найдена
            } else {
                fprintf(temp_file, "%s\n", line); // Копируем строку из исходного файла во временный файл
            }
        } else {
            fprintf(temp_file, "%s\n", line); // Копируем строку из исходного файла во временный файл
        }
    }

    if (!found && !is_empty(dbms)) {
        fprintf(temp_file, "%s =", q_data.variable); // Записываем имя переменной во временный файл
        write_tokens(temp_file, dbms, q_data.variable); // Записываем данные во временный файл
        fprintf(temp_file, "\n"); // Добавляем символ новой строки
    }

    fclose(file);                            // Закрываем файл для чтения
    fclose(temp_file);                       // Закрываем временный файл

    if (remove(filename) != 0 || rename("temp.txt", filename) != 0) {
        perror("Failed to update file");     // Выводим сообщение об ошибке, если не удалось обновить файл
    }
}

void handle_stack(DBMS *dbms, QueryData data, const char* filename) {
    dbms->v_type = _STACK;                   // Устанавливаем тип структуры данных как стек
    dbms->data.stack = NULL;                 // Инициализируем стек
    dbms = read_from_file(dbms, data, filename); // Читаем данные из файла

    if (strcmp(data.command, "SPUSH") == 0) {
        dbms->data.stack = push(dbms->data.stack, data.value); // Добавляем элемент в стек
        show(dbms->data.stack);              // Выводим содержимое стека
        dbms->data.stack = reverse_stack(dbms->data.stack); // Переворачиваем стек
    } else if (strcmp(data.command, "SPOP") == 0) {
        dbms->data.stack = pop(dbms->data.stack); // Удаляем элемент из стека
        show(dbms->data.stack);              // Выводим содержимое стека
        dbms->data.stack = reverse_stack(dbms->data.stack); // Переворачиваем стек
    } else if (strcmp(data.command, "SSHOW") == 0) {
        show(dbms->data.stack);              // Выводим содержимое стека
    }
    write_to_file(dbms, data, filename);     // Записываем данные в файл
}

void handle_set(DBMS *dbms, QueryData data, const char* filename) {
    dbms->v_type = _SET;                     // Устанавливаем тип структуры данных как множество
    dbms->data.set = create_set(10);         // Создаем множество с начальным размером 10
    read_from_file(dbms, data, filename);    // Читаем данные из файла

    if (strcmp(data.command, "ESETADD") == 0) {
        int value = atoi(data.value);        // Преобразуем строку в число
        set_add(dbms->data.set, value);      // Добавляем элемент в множество
        printf("Элемент %d добавлен в множество.\n", value); // Выводим сообщение о добавлении элемента
    } else if (strcmp(data.command, "ESETDEL") == 0) {
        int value = atoi(data.value);        // Преобразуем строку в число
        set_del(dbms->data.set, value);      // Удаляем элемент из множества
        printf("Элемент %d удален из множества.\n", value); // Выводим сообщение об удалении элемента
    } else if (strcmp(data.command, "ESET_AT") == 0) {
        int value = atoi(data.value);        // Преобразуем строку в число
        set_at(dbms->data.set, value);       // Ищем элемент в множестве
    } else if (strcmp(data.command, "ESETSHOW") == 0) {
        printf("Элементы множества: ");      // Выводим сообщение о выводе элементов множества
        for (int i = 0; i < dbms->data.set->size; i++) {
            printf("%d ", dbms->data.set->elements[i]); // Выводим элементы множества
        }
        printf("\n");                        // Добавляем символ новой строки
    }

    write_to_file(dbms, data, filename);     // Записываем данные в файл
}

void handle_list(DBMS *dbms, QueryData data, const char* filename) {
    dbms->v_type = _SINGLE_LINKED_LIST;      // Устанавливаем тип структуры данных как односвязный список
    dbms->data.list = create_list();         // Создаем односвязный список
    dbms = read_from_file(dbms, data, filename); // Читаем данные из файла

    if (strcmp(data.command, "LPUSH_B") == 0) {
        push_back(dbms->data.list, data.value); // Добавляем элемент в конец списка
    } else if (strcmp(data.command, "LPUSH_F") == 0) {
        push_front(dbms->data.list, data.value); // Добавляем элемент в начало списка
    } else if (strcmp(data.command, "LDEL_B") == 0) {
        pop_back(dbms->data.list);           // Удаляем элемент с конца списка
    } else if (strcmp(data.command, "LDEL_F") == 0) {
        pop_front(dbms->data.list);          // Удаляем элемент с начала списка
    } else if (strcmp(data.command, "LDEL_V") == 0) {
        remove_value(dbms->data.list, data.value); // Удаляем элемент по значению
    } else if (strcmp(data.command, "LSEARCH") == 0) {
        Node* result = find_value(dbms->data.list, data.value); // Ищем элемент по значению
        printf(result ? "Value found: %s\n" : "Value not found\n", result ? result->data : ""); // Выводим результат поиска
    }
    write_to_file(dbms, data, filename);     // Записываем данные в файл
}

void handle_array(DBMS *dbms, QueryData data, const char* filename) {
    dbms->v_type = _ARR;                     // Устанавливаем тип структуры данных как массив строк
    dbms->data.arr = create_string_array(10); // Создаем массив строк с начальным размером 10
    dbms = read_from_file(dbms, data, filename); // Читаем данные из файла

    if (strcmp(data.command, "MPUSH") == 0) {
        append_string(dbms->data.arr, data.value); // Добавляем строку в массив
    } else if (strcmp(data.command, "MINSERT") == 0) {
        insertStringAt(dbms->data.arr, atoi(data.key), data.value); // Вставляем строку в массив по индексу
    } else if (strcmp(data.command, "MREPLACE") == 0) {
        replaceStringAt(dbms->data.arr, atoi(data.key), data.value); // Заменяем строку в массиве по индексу
    } else if (strcmp(data.command, "MGET") == 0) {
        char* value = getStringAt(dbms->data.arr, atoi(data.value)); // Получаем строку по индексу
        printf(value ? "Value at index %d: %s\n" : "", atoi(data.value), value); // Выводим строку
    } else if (strcmp(data.command, "MDEL_I") == 0) {
        deleteStringAt(dbms->data.arr, atoi(data.key)); // Удаляем строку по индексу
    } else if (strcmp(data.command, "MSIZE") == 0) {
        printf("Array size: %d\n", getStringArraySize(dbms->data.arr)); // Выводим размер массива
    } else if (strcmp(data.command, "MREAD") == 0) {
        printStringArray(dbms->data.arr);    // Выводим содержимое массива
    }
    write_to_file(dbms, data, filename);     // Записываем данные в файл
}

void handle_hash_map(DBMS *dbms, QueryData data, const char* filename) {
    dbms->v_type = _HASH_MAP;                // Устанавливаем тип структуры данных как хеш-таблица
    dbms->data.hash_map = hash_map_create(10); // Создаем хеш-таблицу с начальным размером 10
    dbms = read_from_file(dbms, data, filename); // Читаем данные из файла

    if (strcmp(data.command, "HSET") == 0) {
        dbms->data.hash_map = hash_map_insert(dbms->data.hash_map, data.key, data.value); // Добавляем пару ключ-значение в хеш-таблицу
    } else if (strcmp(data.command, "HGET") == 0) {
        char *value = hash_map_at(dbms->data.hash_map, data.key); // Получаем значение по ключу
        printf(value ? "Value at key '%s': %s\n" : "Key '%s' not found in hash map.\n", data.key, value); // Выводим значение
    } else if (strcmp(data.command, "HDEL") == 0) {
        hash_map_remove(dbms->data.hash_map, data.key); // Удаляем пару ключ-значение из хеш-таблицы
    }
    write_to_file(dbms, data, filename);     // Записываем данные в файл
}

void handle_queue(DBMS *dbms, QueryData data, const char* filename) {
    dbms->v_type = _QUEUE;                   // Устанавливаем тип структуры данных как очередь
    dbms->data.queue = (queue*)malloc(sizeof(queue)); // Выделяем память для очереди
    init_queue(dbms->data.queue);            // Инициализируем очередь
    dbms = read_from_file(dbms, data, filename); // Читаем данные из файла

    if (strcmp(data.command, "QPUSH") == 0) {
        push_queue(dbms->data.queue, data.value); // Добавляем элемент в очередь
    } else if (strcmp(data.command, "QPOP") == 0) {
        pop_queue(dbms->data.queue);         // Удаляем элемент из очереди
    } else if (strcmp(data.command, "QSHOW") == 0) {
        Node* current = dbms->data.queue->front;
        while (current) {
            printf("%s ", current->data);    // Выводим элементы очереди
            current = current->next;
        }
        printf("\n");                        // Добавляем символ новой строки
    }
    write_to_file(dbms, data, filename);     // Записываем данные в файл
}

void handle_doubly_linked_list(DBMS *dbms, QueryData data, const char* filename) {
    dbms->v_type = _DOUBLE_LINKED_LIST;      // Устанавливаем тип структуры данных как двусвязный список
    dbms->data.dlist = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList)); // Выделяем память для двусвязного списка
    initDoublyLinkedList(dbms->data.dlist);  // Инициализируем двусвязный список
    dbms = read_from_file(dbms, data, filename); // Читаем данные из файла

    if (strcmp(data.command, "DPUSH_B") == 0) {
        addToDoublyLinkedListTail(dbms->data.dlist, data.value); // Добавляем элемент в конец двусвязного списка
    } else if (strcmp(data.command, "DPUSH_F") == 0) {
        addToDoublyLinkedListHead(dbms->data.dlist, data.value); // Добавляем элемент в начало двусвязного списка
    } else if (strcmp(data.command, "DDEL_B") == 0) {
        removeFromDoublyLinkedListTail(dbms->data.dlist); // Удаляем элемент с конца двусвязного списка
    } else if (strcmp(data.command, "DDEL_F") == 0) {
        removeFromDoublyLinkedListHead(dbms->data.dlist); // Удаляем элемент с начала двусвязного списка
    } else if (strcmp(data.command, "DDEL_V") == 0) {
        removeDoublyLinkedListNodeByValue(dbms->data.dlist, data.value); // Удаляем элемент по значению
    } else if (strcmp(data.command, "DSEARCH") == 0) {
        Node *result = findDoublyLinkedListNodeByValue(dbms->data.dlist, data.value); // Ищем элемент по значению
        printf(result ? "Value found: %s\n" : "Value not found\n", result ? result->data : ""); // Выводим результат поиска
    } else if (strcmp(data.command, "DSHOW") == 0) {
        printDoublyLinkedList(dbms->data.dlist); // Выводим содержимое двусвязного списка
    }
    write_to_file(dbms, data, filename);     // Записываем данные в файл
}

void handle_tree(DBMS *dbms, QueryData data, const char* filename) {
    dbms->v_type = _TREE;                    // Устанавливаем тип структуры данных как дерево
    dbms->data.tree = createBinaryTree();    // Создаем бинарное дерево
    dbms = read_from_file(dbms, data, filename); // Читаем данные из файла

    if (strcmp(data.command, "TINSERT") == 0) {
        insertElement(dbms->data.tree, atoi(data.value)); // Добавляем элемент в дерево
    } else if (strcmp(data.command, "TSEARCH") == 0) {
        int value = atoi(data.value);        // Преобразуем строку в число
        printf(searchElement(dbms->data.tree, value) ? "Value %d found in tree.\n" : "Value %d not found in tree.\n", value); // Ищем элемент в дереве
    } else if (strcmp(data.command, "TISFULL") == 0) {
        printf(isFull(dbms->data.tree) ? "Tree is a full binary tree.\n" : "Tree is not a full binary tree.\n"); // Проверяем, является ли дерево полным бинарным деревом
    } else if (strcmp(data.command, "TSHOW") == 0) {
        print(dbms->data.tree, stdout);      // Выводим содержимое дерева
    }
    write_to_file(dbms, data, filename);     // Записываем данные в файл
}

void comand_handler(DBMS *dbms, QueryData data, const char* filename) {
    switch (data.command[0]) {               // Определяем первую букву команды
        case 'S': handle_stack(dbms, data, filename); break; // Обрабатываем команду для стека
        case 'L': handle_list(dbms, data, filename); break; // Обрабатываем команду для односвязного списка
        case 'M': handle_array(dbms, data, filename); break; // Обрабатываем команду для массива строк
        case 'H': handle_hash_map(dbms, data, filename); break; // Обрабатываем команду для хеш-таблицы
        case 'Q': handle_queue(dbms, data, filename); break; // Обрабатываем команду для очереди
        case 'D': handle_doubly_linked_list(dbms, data, filename); break; // Обрабатываем команду для двусвязного списка
        case 'T': handle_tree(dbms, data, filename); break; // Обрабатываем команду для дерева
        case 'E': handle_set(dbms, data, filename); break; // Обрабатываем команду для множества
    }
}
