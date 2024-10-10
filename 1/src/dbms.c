```c
#include "../include/single_linked_list.h"
#include "../include/dbms.h"
#include "../include/tree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Функция для парсинга запроса и заполнения структуры QueryData
QueryData parse_query(const char* query) {
    QueryData data;
    // Инициализация структуры QueryData нулевыми значениями
    memset(&data, 0, sizeof(data));

    // Попытка разбора строки запроса в соответствии с ожидаемым форматом
    if (sscanf(query, "%s %s %s %s", data.command, data.variable, data.key, data.value) == 4 ||
        sscanf(query, "%s %s %s", data.command, data.variable, data.value) == 3 ||
        sscanf(query, "%s %s", data.command, data.variable) == 2) {
        return data;
    }

    // Вывод сообщения об ошибке, если формат запроса некорректен
    fprintf(stderr, "Invalid query format\n");
    data.command[0] = '\0';
    return data;
}

// Функция для чтения токенов из строки и добавления их в соответствующую структуру данных
void read_tokens(DBMS *dbms, char *value) {
    // Разделение строки на токены по пробелам
    char *token = strtok(value, " ");
    while (token != NULL) {
        // В зависимости от типа структуры данных, добавление токена в соответствующую структуру
        switch (dbms->v_type) {
            case _STACK: dbms->data.stack = push(dbms->data.stack, token); break;
            case _SINGLE_LINKED_LIST: push_back(dbms->data.list, token); break;
            case _ARR: append_string(dbms->data.arr, token); break;
            case _HASH_MAP: {
                char key[50], val[50];
                // Разбор токена на ключ и значение для хеш-карты
                if (sscanf(token, "%49[^:]:%49s", key, val) == 2) {
                    dbms->data.hash_map = hash_map_insert(dbms->data.hash_map, key, val);
                }
                break;
            }
            case _QUEUE: push_queue(dbms->data.queue, token); break;
            case _DOUBLE_LINKED_LIST: addToDoublyLinkedListTail(dbms->data.dlist, token); break;
            case _TREE: insertElement(dbms->data.tree, atoi(token)); break;
        }
        // Переход к следующему токену
        token = strtok(NULL, " ");
    }
}

// Функция для чтения данных из файла и заполнения соответствующей структуры данных
DBMS *read_from_file(DBMS *dbms, QueryData q_data, const char* filename) {
    // Открытие файла для чтения
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    char line[256], name[256], value[256];
    // Чтение строк из файла
    while (fgets(line, sizeof(line), file)) {
        // Удаление символа новой строки
        line[strcspn(line, "\n")] = 0;
        // Разбор строки на имя переменной и значение
        if (sscanf(line, "%s = %[^\n]", name, value) == 2 && strcmp(name, q_data.variable) == 0) {
            // Чтение токенов из значения и добавление их в соответствующую структуру данных
            read_tokens(dbms, value);
        }
    }

    // Закрытие файла
    fclose(file);
    return dbms;
}

// Функция для проверки, является ли структура данных пустой
int is_empty(DBMS *dbms) {
    // В зависимости от типа структуры данных, проверка на пустоту
    switch (dbms->v_type) {
        case _STACK: return dbms->data.stack == NULL;
        case _SINGLE_LINKED_LIST: return dbms->data.list->head == NULL;
        case _ARR: return dbms->data.arr->size == 0;
        case _HASH_MAP: {
            for (size_t i = 0; i < dbms->data.hash_map->size; i++) {
                if (dbms->data.hash_map->entries[i].key) return 0;
            }
            return 1;
        }
        case _QUEUE: return dbms->data.queue->front == NULL;
        case _DOUBLE_LINKED_LIST: return dbms->data.dlist->head == NULL;
        case _TREE: return dbms->data.tree->root == NULL;
    }
    return 1;
}

// Функция для записи токенов в файл
void write_tokens(FILE *file, DBMS *dbms, const char *name) {
    // Если структура данных пуста, выход из функции
    if (is_empty(dbms)) return;

    // В зависимости от типа структуры данных, запись токенов в файл
    switch (dbms->v_type) {
        case _STACK: {
            stack* current = dbms->data.stack;
            while (current) {
                fprintf(file, " %s", current->data);
                current = current->next;
            }
            break;
        }
        case _SINGLE_LINKED_LIST: {
            Node* current = dbms->data.list->head;
            while (current) {
                fprintf(file, " %s", current->data);
                current = current->next;
            }
            break;
        }
        case _ARR: {
            for (int i = 0; i < dbms->data.arr->size; i++) {
                fprintf(file, " %s", dbms->data.arr->data[i]);
            }
            break;
        }
        case _HASH_MAP: {
            for (size_t i = 0; i < dbms->data.hash_map->size; i++) {
                if (dbms->data.hash_map->entries[i].key) {
                    fprintf(file, " %s:%s", dbms->data.hash_map->entries[i].key, dbms->data.hash_map->entries[i].value);
                }
            }
            break;
        }
        case _QUEUE: {
            Node* current = dbms->data.queue->front;
            while (current) {
                fprintf(file, " %s", current->data);
                current = current->next;
            }
            break;
        }
        case _DOUBLE_LINKED_LIST: {
            Node* current = dbms->data.dlist->head;
            while (current) {
                fprintf(file, " %s", current->data);
                current = current->next;
            }
            break;
        }
        case _TREE: {
            printTree(dbms->data.tree->root, file);
            break;
        }
    }
}

// Функция для записи данных в файл
void write_to_file(DBMS *dbms, QueryData q_data, const char* filename) {
    // Открытие файла для чтения
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file for reading");
        return;
    }

    // Открытие временного файла для записи
    FILE* temp_file = fopen("temp.txt", "w");
    if (!temp_file) {
        perror("Failed to open temp file for writing");
        fclose(file);
        return;
    }

    char line[256], name[256], value[256];
    int found = 0;

    // Чтение строк из файла
    while (fgets(line, sizeof(line), file)) {
        // Удаление символа новой строки
        line[strcspn(line, "\n")] = 0;
        // Разбор строки на имя переменной и значение
        if (sscanf(line, "%s = %[^\n]", name, value) == 2) {
            // Если имя переменной совпадает с искомым, запись новых данных
            if (strcmp(name, q_data.variable) == 0) {
                if (!is_empty(dbms)) {
                    fprintf(temp_file, "%s =", name);
                    write_tokens(temp_file, dbms, name);
                    fprintf(temp_file, "\n");
                }
                found = 1;
            } else {
                // Иначе, запись строки без изменений
                fprintf(temp_file, "%s\n", line);
            }
        } else {
            // Запись строки без изменений
            fprintf(temp_file, "%s\n", line);
        }
    }

    // Если переменная не была найдена, запись новых данных в конец файла
    if (!found && !is_empty(dbms)) {
        fprintf(temp_file, "%s =", q_data.variable);
        write_tokens(temp_file, dbms, q_data.variable);
        fprintf(temp_file, "\n");
    }

    // Закрытие файлов
    fclose(file);
    fclose(temp_file);

    // Замена оригинального файла временным файлом
    if (remove(filename) != 0 || rename("temp.txt", filename) != 0) {
        perror("Failed to update file");
    }
}

// Обработчик команд для стека
void handle_stack(DBMS *dbms, QueryData data, const char* filename) {
    // Установка типа структуры данных в стек
    dbms->v_type = _STACK;
    dbms->data.stack = NULL;
    // Чтение данных из файла
    dbms = read_from_file(dbms, data, filename);

    // Обработка команды в зависимости от типа команды
    if (strcmp(data.command, "SPUSH") == 0) {
        dbms->data.stack = push(dbms->data.stack, data.value);
        show(dbms->data.stack);
        dbms->data.stack = reverse_stack(dbms->data.stack);
    } else if (strcmp(data.command, "SPOP") == 0) {
        dbms->data.stack = pop(dbms->data.stack);
        show(dbms->data.stack);
        dbms->data.stack = reverse_stack(dbms->data.stack);
    } else if (strcmp(data.command, "SSHOW") == 0) {
        show(dbms->data.stack);
    }
    // Запись данных в файл
    write_to_file(dbms, data, filename);
}

// Обработчик команд для односвязного списка
void handle_list(DBMS *dbms, QueryData data, const char* filename) {
    // Установка типа структуры данных в односвязный список
    dbms->v_type = _SINGLE_LINKED_LIST;
    dbms->data.list = create_list();
    // Чтение данных из файла
    dbms = read_from_file(dbms, data, filename);

    // Обработка команды в зависимости от типа команды
    if (strcmp(data.command, "LPUSH_B") == 0) {
        push_back(dbms->data.list, data.value);
    } else if (strcmp(data.command, "LPUSH_F") == 0) {
        push_front(dbms->data.list, data.value);
    } else if (strcmp(data.command, "LDEL_B") == 0) {
        pop_back(dbms->data.list);
    } else if (strcmp(data.command, "LDEL_F") == 0) {
        pop_front(dbms->data.list);
    } else if (strcmp(data.command, "LDEL_V") == 0) {
        remove_value(dbms->data.list, data.value);
    } else if (strcmp(data.command, "LSEARCH") == 0) {
        Node* result = find_value(dbms->data.list, data.value);
        printf(result ? "Value found: %s\n" : "Value not found\n", result ? result->data : "");
    }
    // Запись данных в файл
    write_to_file(dbms, data, filename);
}

// Обработчик команд для массива строк
void handle_array(DBMS *dbms, QueryData data, const char* filename) {
    // Установка типа структуры данных в массив строк
    dbms->v_type = _ARR;
    dbms->data.arr = create_string_array(10);
    // Чтение данных из файла
    dbms = read_from_file(dbms, data, filename);

    // Обработка команды в зависимости от типа команды
    if (strcmp(data.command, "MPUSH") == 0) {
        append_string(dbms->data.arr, data.value);
    } else if (strcmp(data.command, "MINSERT") == 0) {
        insertStringAt(dbms->data.arr, atoi(data.key), data.value);
    } else if (strcmp(data.command, "MREPLACE") == 0) {
        replaceStringAt(dbms->data.arr, atoi(data.key), data.value);
    } else if (strcmp(data.command, "MGET") == 0) {
        char* value = getStringAt(dbms->data.arr, atoi(data.value));
        printf(value ? "Value at index %d: %s\n" : "", atoi(data.value), value);
    } else if (strcmp(data.command, "MDEL_I") == 0) {
        deleteStringAt(dbms->data.arr, atoi(data.key));
    } else if (strcmp(data.command, "MSIZE") == 0) {
        printf("Array size: %d\n", getStringArraySize(dbms->data.arr));
    } else if (strcmp(data.command, "MREAD") == 0) {
        printStringArray(dbms->data.arr);
    }
    // Запись данных в файл
    write_to_file(dbms, data, filename);
}

// Обработчик команд для хеш-карты
void handle_hash_map(DBMS *dbms, QueryData data, const char* filename) {
    // Установка типа структуры данных в хеш-карту
    dbms->v_type = _HASH_MAP;
    dbms->data.hash_map = hash_map_create(10);
    // Чтение данных из файла
    dbms = read_from_file(dbms, data, filename);

    // Обработка команды в зависимости от типа команды
    if (strcmp(data.command, "HSET") == 0) {
        dbms->data.hash_map = hash_map_insert(dbms->data.hash_map, data.key, data.value);
    } else if (strcmp(data.command, "HGET") == 0) {
        char *value = hash_map_at(dbms->data.hash_map, data.key);
        printf(value ? "Value at key '%s': %s\n" : "Key '%s' not found in hash map.\n", data.key, value);
    } else if (strcmp(data.command, "HDEL") == 0) {
        hash_map_remove(dbms->data.hash_map, data.key);
    }
    // Запись данных в файл
    write_to_file(dbms, data, filename);
}

// Обработчик команд для очереди
void handle_queue(DBMS *dbms, QueryData data, const char* filename) {
    // Установка типа структуры данных в очередь
    dbms->v_type = _QUEUE;
    dbms->data.queue = (queue*)malloc(sizeof(queue));
    init_queue(dbms->data.queue);
    // Чтение данных из файла
    dbms = read_from_file(dbms, data, filename);

    // Обработка команды в зависимости от типа команды
    if (strcmp(data.command, "QPUSH") == 0) {
        push_queue(dbms->data.queue, data.value);
    } else if (strcmp(data.command, "QPOP") == 0) {
        pop_queue(dbms->data.queue);
    } else if (strcmp(data.command, "QSHOW") == 0) {
        Node* current = dbms->data.queue->front;
        while (current) {
            printf("%s ", current->data);
            current = current->next;
        }
        printf("\n");
    }
    // Запись данных в файл
    write_to_file(dbms, data, filename);
}

// Обработчик команд для двусвязного списка
void handle_doubly_linked_list(DBMS *dbms, QueryData data, const char* filename) {
    // Установка типа структуры данных в двусвязный список
    dbms->v_type = _DOUBLE_LINKED_LIST;
    dbms->data.dlist = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    initDoublyLinkedList(dbms->data.dlist);
    // Чтение данных из файла
    dbms = read_from_file(dbms, data, filename);

    // Обработка команды в зависимости от типа команды
    if (strcmp(data.command, "DPUSH_B") == 0) {
        addToDoublyLinkedListTail(dbms->data.dlist, data.value);
    } else if (strcmp(data.command, "DPUSH_F") == 0) {
        addToDoublyLinkedListHead(dbms->data.dlist, data.value);
    } else if (strcmp(data.command, "DDEL_B") == 0) {
        removeFromDoublyLinkedListTail(dbms->data.dlist);
    } else if (strcmp(data.command, "DDEL_F") == 0) {
        removeFromDoublyLinkedListHead(dbms->data.dlist);
    } else if (strcmp(data.command, "DDEL_V") == 0) {
        removeDoublyLinkedListNodeByValue(dbms->data.dlist, data.value);
    } else if (strcmp(data.command, "DSEARCH") == 0) {
        Node *result = findDoublyLinkedListNodeByValue(dbms->data.dlist, data.value);
        printf(result ? "Value found: %s\n" : "Value not found\n", result ? result->data : "");
    } else if (strcmp(data.command, "DSHOW") == 0) {
        printDoublyLinkedList(dbms->data.dlist);
    }
    // Запись данных в файл
    write_to_file(dbms, data, filename);
}

// Обработчик команд для бинарного дерева
void handle_tree(DBMS *dbms, QueryData data, const char* filename) {
    // Установка типа структуры данных в бинарное дерево
    dbms->v_type = _TREE;
    dbms->data.tree = createBinaryTree();
    // Чтение данных из файла
    dbms = read_from_file(dbms, data, filename);

    // Обработка команды в зависимости от типа команды
    if (strcmp(data.command, "TINSERT") == 0) {
        insertElement(dbms->data.tree, atoi(data.value));
    } else if (strcmp(data.command, "TSEARCH") == 0) {
        int value = atoi(data.value);
        printf(searchElement(dbms->data.tree, value) ? "Value %d found in tree.\n" : "Value %d not found in tree.\n", value);
    } else if (strcmp(data.command, "TISFULL") == 0) {
        printf(isFull(dbms->data.tree) ? "Tree is a full binary tree.\n" : "Tree is not a full binary tree.\n");
    } else if (strcmp(data.command, "TSHOW") == 0) {
        print(dbms->data.tree, stdout);
    }
    // Запись данных в файл
    write_to_file(dbms, data, filename);
}

// Основной обработчик команд
void comand_handler(DBMS *dbms, QueryData data, const char* filename) {
    // В зависимости от первого символа команды, вызов соответствующего обработчика
    switch (data.command[0]) {
        case 'S': handle_stack(dbms, data, filename); break;
        case 'L': handle_list(dbms, data, filename); break;
        case 'M': handle_array(dbms, data, filename); break;
        case 'H': handle_hash_map(dbms, data, filename); break;
        case 'Q': handle_queue(dbms, data, filename); break;
        case 'D': handle_doubly_linked_list(dbms, data, filename); break;
        case 'T': handle_tree(dbms, data, filename); break;
    }
}
```
