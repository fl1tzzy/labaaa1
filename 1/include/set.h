#ifndef SET_H
#define SET_H   

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Set {
    char **elements;    // Массив строк (оригинальный массив для хранения элементов)
    size_t capacity;    // Ёмкость множества
    size_t size;        // Текущий размер множества
    char **hash_table;  // Хэш-таблица для поиска
    size_t hash_capacity; // Ёмкость хэш-таблицы
} Set;

Set *create_set(int capacity);// Объявляем функцию для создания нового множества с заданной емкостью.
bool contains(Set *set, const char *element);// Объявляем функцию для проверки наличия элемента в множестве.
void set_add(Set *set, const char *element);// Объявляем функцию для добавления элемента в множество.
void set_at(Set *set, const char *element);// Объявляем функцию для поиска элемента в множестве и вывода сообщения о его наличии.
void set_del(Set *set, const char *element); // Объявляем функцию для удаления элемента из множества.
void free_set(Set *set);

#endif

