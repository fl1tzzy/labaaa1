#include "../include/dbms.h"

int main(int argc, char* argv[]) {
    // Инициализация указателей на имя файла и строку запроса
    char* filename = NULL;
    char* query = NULL;

    // Проход по аргументам командной строки
    for (int i = 1; i < argc; i++) {
        // Проверка на наличие аргумента "--file"
        if (strcmp(argv[i], "--file") == 0) {
            // Установка указателя на имя файла
            filename = argv[++i];
        // Проверка на наличие аргумента "--query"
        } else if (strcmp(argv[i], "--query") == 0) {
            // Установка указателя на строку запроса
            query = argv[++i];
        }
    }

    // Проверка на наличие обязательных аргументов
    if (filename == NULL || query == NULL) {
        // Вывод сообщения об ошибке, если аргументы отсутствуют
        fprintf(stderr, "Missing --file or --query argument\n");
        return 1;
    }

    // Инициализация структуры DBMS
    DBMS dbms;
    // Парсинг строки запроса и заполнение структуры QueryData
    QueryData q_data = parse_query(query);
    // Обработка команды с использованием структуры DBMS и данных запроса
    comand_handler(&dbms, q_data, filename);
    
    // Возвращение кода успешного завершения программы
    return 0;
}
