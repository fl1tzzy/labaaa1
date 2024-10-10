#include "../include/arr.h"

// Функция для создания массива строк с заданной начальной емкостью
struct StringArray* create_string_array(size_t capacity) {
    // Выделение памяти для структуры StringArray
    StringArray* arr = (StringArray*)malloc(sizeof(StringArray));
    // Выделение памяти для массива указателей на строки
    arr->data = (char**)malloc(capacity * sizeof(char*));
    // Инициализация размера и емкости массива
    arr->size = 0;
    arr->capacity = capacity;
    // Возвращение указателя на созданный массив строк
    return arr;
}

// Функция для изменения размера массива строк
void resizeStringArray(struct StringArray* arr) {
    // Увеличение емкости массива в два раза
    arr->capacity *= 2;
    // Перераспределение памяти для массива указателей на строки с новой емкостью
    arr->data = (char**)realloc(arr->data, arr->capacity * sizeof(char*));
}

// Функция для добавления строки в конец массива
void append_string(struct StringArray* arr, const char* str) {
    // Если текущий размер массива равен его емкости, увеличиваем емкость
    if (arr->size == arr->capacity) {
        resizeStringArray(arr);
    }
    // Копирование строки для хранения в массиве
    arr->data[arr->size++] = strdup(str);
    // Вывод сообщения о добавлении строки
    printf("Added '%s' to array\n", str);
}

// Функция для вставки строки в массив по указанному индексу
void insertStringAt(struct StringArray* arr, int index, const char* str) {
    // Проверка на корректность индекса
    if (index < 0 || index > arr->size) {
        printf("Invalid index\n");
        return;
    }
    // Если текущий размер массива равен его емкости, увеличиваем емкость
    if (arr->size == arr->capacity) {
        resizeStringArray(arr);
    }
    // Сдвиг элементов массива вправо для освобождения места для новой строки
    for (int i = arr->size; i > index; i--) {
        arr->data[i] = arr->data[i - 1];
    }
    // Копирование строки для вставки
    arr->data[index] = strdup(str);
    // Увеличение размера массива
    arr->size++;
    // Вывод сообщения о вставке строки
    printf("Inserted '%s' at index %d\n", str, index);
}

// Функция для удаления строки из массива по указанному индексу
void deleteStringAt(struct StringArray* arr, int index) {
    // Проверка на корректность индекса
    if (index < 0 || index >= arr->size) {
        printf("Invalid index\n");
        return;
    }
    // Освобождение памяти для удаляемой строки
    free(arr->data[index]);
    // Сдвиг элементов массива влево для заполнения пробела
    for (int i = index; i < arr->size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    }
    // Уменьшение размера массива
    arr->size--;
    // Вывод сообщения об удалении строки
    printf("Deleted string at index %d\n", index);
}

// Функция для получения строки из массива по указанному индексу
char* getStringAt(struct StringArray* arr, int index) {
    // Проверка на корректность индекса
    if (index < 0 || index >= arr->size) {
        printf("Invalid index\n");
        return NULL;
    }
    // Возвращение строки по указанному индексу
    return arr->data[index];
}

// Функция для замены строки в массиве по указанному индексу
void replaceStringAt(struct StringArray* arr, int index, const char* str) {
    // Проверка на корректность индекса
    if (index < 0 || index >= arr->size) {
        printf("Invalid index\n");
        return;
    }
    // Освобождение памяти для заменяемой строки
    free(arr->data[index]);
    // Копирование новой строки для замены
    arr->data[index] = strdup(str);
    // Вывод сообщения о замене строки
    printf("Replaced string at index %d with '%s'\n", index, str);
}

// Функция для получения текущего размера массива строк
int getStringArraySize(struct StringArray* arr) {
    // Возвращение текущего размера массива
    return arr->size;
}

// Функция для вывода всех строк в массиве
void printStringArray(struct StringArray* arr) {
    // Проверка на пустоту массива
    if (arr->size == 0) {
        printf("Array is empty\n");
        return;
    }
    // Вывод всех строк в массиве
    printf("Array elements: ");
    for (int i = 0; i < arr->size; i++) {
        printf("'%s' ", arr->data[i]);
    }
    printf("\n");
}

// Функция для освобождения памяти, выделенной под массив строк
void freeStringArray(struct StringArray* arr) {
    // Освобождение памяти для каждой строки в массиве
    for (int i = 0; i < arr->size; i++) {
        free(arr->data[i]);
    }
    // Освобождение памяти для массива указателей на строки
    free(arr->data);
    // Освобождение памяти для структуры StringArray
    free(arr);
}
