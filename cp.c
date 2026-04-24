#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SOFTWARE 1000

enum Version {
    ALPHA,
    BETA,
    RELEASE,
    DEVELOPMENT,
    NONINFO
};
enum OperatingSystem {
    WINDOWS,
    LINUX,
    MACOS,
    OTHER
};
struct Date {
    int day;
    int month;
    int year;
};
struct Software {
    char name[100];
    double price;
    char info[200];
    enum Version version;
    struct Date releaseDate;
    int users;
    enum OperatingSystem os;
    double rating;
};

// ПРОТОТИПЫ //
const char* versionToString(enum Version v);
const char* osToString(enum OperatingSystem os);
enum Version stringToVersion(const char* str);
enum OperatingSystem stringToOS(const char* str);

int saveToFile(struct Software* database, int count);
int loadFromFile(struct Software* database, int* count);
int addRecord(struct Software* database, int* count);
int viewAllRecords(struct Software* database, int count);
int searchByPrice(struct Software* database, int count);
int combinedSearch(struct Software* database, int count);
int sortData(struct Software* database, int count);
int showMenu(void);

// MAIN //
int main(void) {
    setlocale(LC_ALL, "RUS");

    struct Software database[MAX_SOFTWARE];
    int count = 0;

    loadFromFile(database, &count);

    int choice;

    do {
        choice = showMenu();

        switch (choice) {
        case 1:
            viewAllRecords(database, count);
            break;
        case 2:
            addRecord(database, &count);
            break;
        case 3:
            saveToFile(database, count);
            break;
        case 4:
            combinedSearch(database, count);
            break;
        case 5:
            searchByPrice(database, count);
            break;
        case 6:
            sortData(database, count);
            break;
        case 7:
            loadFromFile(database, &count);
            break;
        case 8:
            printf("Выход из программы...\n");
            break;
        default:
            printf("Неверный выбор! Попробуйте снова.\n");
        }

        printf("\n");

    } while (choice != 8);

    saveToFile(database, count);

    return 0;
}

// МЕНЮ //
int showMenu(void) {
    int choice;

    printf("\n========== ГЛАВНОЕ МЕНЮ ==========\n");
    printf("1. Просмотреть все записи\n");
    printf("2. Добавить новую запись\n");
    printf("3. Сохранение данных в файл\n");
    printf("4. Комбинированный поиск (версия + рейтинг)\n");
    printf("5. Поиск по цене\n");
    printf("6. Сортировка данных\n");
    printf("7. Загрузка данных из файла\n");
    printf("8. Выход\n");

    printf("Выберите действие: ");
    scanf("%d", &choice);

    return choice;
}

// ПРЕОБРАЗОВАНИЯ //
const char* versionToString(enum Version v) {
    switch (v) {
    case ALPHA: return "Alpha";
    case BETA: return "Beta";
    case RELEASE: return "Release";
    case DEVELOPMENT: return "Development";
    default: return "NonInfo";
    }
}
const char* osToString(enum OperatingSystem os) {
    switch (os) {
    case WINDOWS: return "Windows";
    case LINUX: return "Linux";
    case MACOS: return "MacOS";
    default: return "Other";
    }
}
enum Version stringToVersion(const char* str) {
    if (strcmp(str, "Alpha") == 0) return ALPHA;
    if (strcmp(str, "Beta") == 0) return BETA;
    if (strcmp(str, "Release") == 0) return RELEASE;
    if (strcmp(str, "Development") == 0) return DEVELOPMENT;
    return NONINFO;
}
enum OperatingSystem stringToOS(const char* str) {
    if (strcmp(str, "Windows") == 0) return WINDOWS;
    if (strcmp(str, "Linux") == 0) return LINUX;
    if (strcmp(str, "MacOS") == 0) return MACOS;
    return OTHER;
}

// ФАЙЛ //
int saveToFile(struct Software* database, int count) {
    FILE* file = fopen("database.txt", "w");

    if (!file) {
        printf("Ошибка: не удалось открыть файл для записи!\n");
        return -1;
    }

    fprintf(file, "# База данных программного обеспечения\n\n");

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s|%.2f|%s|%s|%d.%d.%d|%d|%s|%.1f\n",
            database[i].name,
            database[i].price,
            database[i].info,
            versionToString(database[i].version),
            database[i].releaseDate.day,
            database[i].releaseDate.month,
            database[i].releaseDate.year,
            database[i].users,
            osToString(database[i].os),
            database[i].rating);
    }

    fclose(file);

    printf("Данные успешно сохранены в файл.\n");
    return 0;
}
int loadFromFile(struct Software* database, int* count) {
    FILE* file = fopen("database.txt", "r");

    if (!file) {
        printf("Файл не найден. Будет создан при сохранении.\n");
        return -1;
    }

    char line[500];
    *count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;

        struct Software temp;
        char ver[20], date[20], os[20];

        if (sscanf(line, "%99[^|]|%lf|%199[^|]|%19[^|]|%19[^|]|%d|%19[^|]|%lf",
            temp.name, &temp.price, temp.info,
            ver, date, &temp.users, os, &temp.rating) == 8) {

            temp.version = stringToVersion(ver);
            temp.os = stringToOS(os);

            sscanf(date, "%d.%d.%d",
                &temp.releaseDate.day,
                &temp.releaseDate.month,
                &temp.releaseDate.year);

            database[*count] = temp;
            (*count)++;
        }
    }

    fclose(file);

    printf("Загружено записей: %d\n", *count);
    return 0;
}

// ДОБАВЛЕНИЕ //
int addRecord(struct Software* database, int* count) {
    if (*count >= MAX_SOFTWARE) {
        printf("Ошибка: достигнут лимит записей (%d)!\n", MAX_SOFTWARE);
        return -1;
    }

    printf("\n=== ДОБАВЛЕНИЕ НОВОЙ ЗАПИСИ ===\n");

    printf("Введите название программы: ");
    scanf(" %[^\n]", database[*count].name);

    printf("Введите цену: ");
    scanf("%lf", &database[*count].price);

    printf("Введите дополнительную информацию: ");
    scanf(" %[^\n]", database[*count].info);

    /* --- ВЫБОР ВЕРСИИ --- */
    printf("Выберите версию:\n");
    printf("0 - Alpha\n");
    printf("1 - Beta\n");
    printf("2 - Release\n");
    printf("3 - Development\n");
    printf("4 - NonInfo\n");

    int v;
    do {
        printf("Ваш выбор: ");
        scanf("%d", &v);

        if (v < 0 || v > 4) {
            printf("Ошибка! Введите число от 0 до 4.\n");
        }
    } while (v < 0 || v > 4);

    database[*count].version = (enum Version)v;

    /* --- ДАТА --- */
    printf("Введите дату выпуска (день месяц год через пробел): ");
    scanf("%d %d %d",
        &database[*count].releaseDate.day,
        &database[*count].releaseDate.month,
        &database[*count].releaseDate.year);

    printf("Введите количество пользователей: ");
    scanf("%d", &database[*count].users);

    /* --- ВЫБОР ОС --- */
    printf("Выберите операционную систему:\n");
    printf("0 - Windows\n");
    printf("1 - Linux\n");
    printf("2 - MacOS\n");
    printf("3 - Other\n");

    int o;
    do {
        printf("Ваш выбор: ");
        scanf("%d", &o);

        if (o < 0 || o > 3) {
            printf("Ошибка! Введите число от 0 до 3.\n");
        }
    } while (o < 0 || o > 3);

    database[*count].os = (enum OperatingSystem)o;

    /* --- РЕЙТИНГ --- */
    printf("Введите рейтинг (от 0.0 до 5.0): ");
    scanf("%lf", &database[*count].rating);

    if (database[*count].rating < 0.0) database[*count].rating = 0.0;
    if (database[*count].rating > 5.0) database[*count].rating = 5.0;

    (*count)++;

    printf("Запись успешно добавлена!\n");

    return 0;
}

// ПРОСМОТР //
int viewAllRecords(struct Software* database, int count) {
    if (count == 0) {
        printf("База данных пуста! Нет записей для отображения.\n");
        return -1;
    }

    printf("\n=== ВСЕ ЗАПИСИ В БАЗЕ ДАННЫХ (%d) ===\n", count);

    for (int i = 0; i < count; i++) {
        printf("\n[Запись %d]\n", i + 1);
        printf("  Название: %s\n", database[i].name);
        printf("  Цена: %.2f\n", database[i].price);
        printf("  Информация: %s\n", database[i].info);
        printf("  Версия: %s\n", versionToString(database[i].version));
        printf("  Дата выпуска: %d.%d.%d\n",
            database[i].releaseDate.day,
            database[i].releaseDate.month,
            database[i].releaseDate.year);
        printf("  Количество пользователей: %d\n", database[i].users);
        printf("  Операционная система: %s\n", osToString(database[i].os));
        printf("  Рейтинг: %.1f/5.0\n", database[i].rating);
        printf("----------------------------------------\n");
    }

    return 0;
}

// ПОИСК //
int searchByPrice(struct Software* database, int count) {
    if (count == 0) {
        printf("База данных пуста! Нечего искать.\n");
        return -1;
    }

    printf("\n=== ПОИСК ПО ЦЕНЕ ===\n");

    int choice;
    printf("Выберите тип поиска:\n");
    printf("1 - По максимальной цене (дешевле чем...)\n");
    printf("2 - По минимальной цене (дороже чем...)\n");
    printf("3 - В диапазоне цен\n");
    printf("Ваш выбор: ");
    scanf("%d", &choice);

    int found = 0;

    switch (choice) {
    case 1: {
        double maxPrice;
        printf("Введите максимальную цену: ");
        scanf("%lf", &maxPrice);

        printf("\nПрограммы дешевле %.2f:\n", maxPrice);
        for (int i = 0; i < count; i++) {
            if (database[i].price <= maxPrice) {
                printf("[%d] %s - %.2f руб.\n", i + 1, database[i].name, database[i].price);
                found++;
            }
        }
        break;
    }

    case 2: {
        double minPrice;
        printf("Введите минимальную цену: ");
        scanf("%lf", &minPrice);

        printf("\nПрограммы дороже %.2f:\n", minPrice);
        for (int i = 0; i < count; i++) {
            if (database[i].price >= minPrice) {
                printf("[%d] %s - %.2f руб.\n", i + 1, database[i].name, database[i].price);
                found++;
            }
        }
        break;
    }

    case 3: {
        double minPrice, maxPrice;
        printf("Введите минимальную цену: ");
        scanf("%lf", &minPrice);
        printf("Введите максимальную цену: ");
        scanf("%lf", &maxPrice);

        printf("\nПрограммы в диапазоне от %.2f до %.2f:\n", minPrice, maxPrice);
        for (int i = 0; i < count; i++) {
            if (database[i].price >= minPrice && database[i].price <= maxPrice) {
                printf("[%d] %s - %.2f руб.\n", i + 1, database[i].name, database[i].price);
                found++;
            }
        }
        break;
    }

    default:
        printf("Неверный выбор!\n");
        return -1;
    }

    if (found == 0)
        printf("По вашему запросу ничего не найдено.\n");
    else
        printf("Найдено записей: %d\n", found);

    return 0;
}
int combinedSearch(struct Software* database, int count) {
    if (count == 0) {
        printf("База данных пуста! Нечего искать.\n");
        return -1;
    }

    printf("\n=== КОМБИНИРОВАННЫЙ ПОИСК (Версия + Рейтинг) ===\n");

    int ver;
    printf("Выберите версию для поиска:\n");
    printf("0 - Alpha\n1 - Beta\n2 - Release\n3 - Development\n4 - NonInfo\n");
    printf("Ваш выбор: ");
    scanf("%d", &ver);

    double minRating;
    printf("Введите минимальный рейтинг (от 0.0 до 5.0): ");
    scanf("%lf", &minRating);

    if (minRating < 0.0) minRating = 0.0;
    if (minRating > 5.0) minRating = 5.0;

    int found = 0;

    printf("\nПрограммы с версией '%s' и рейтингом не ниже %.1f:\n",
        versionToString((enum Version)ver), minRating);

    for (int i = 0; i < count; i++) {
        if (database[i].version == ver && database[i].rating >= minRating) {
            printf("[%d] %s\n", i + 1, database[i].name);
            printf("    Версия: %s\n", versionToString(database[i].version));
            printf("    Рейтинг: %.1f/5.0\n", database[i].rating);
            printf("    Цена: %.2f руб.\n", database[i].price);
            printf("    --------------------\n");
            found++;
        }
    }

    if (found == 0)
        printf("По вашему запросу ничего не найдено.\n");
    else
        printf("Найдено записей: %d\n", found);

    return 0;
}

// СОРТИРОВКА //
int sortData(struct Software* database, int count) {
    if (count == 0) {
        printf("База данных пуста! Нечего сортировать.\n");
        return -1;
    }

    printf("\n=== СОРТИРОВКА ДАННЫХ ===\n");
    printf("Выберите тип сортировки:\n");
    printf("1 - По рейтингу (убывание)\n");
    printf("2 - По цене (возрастание)\n");
    printf("3 - По рейтингу (убывание) и по цене (возрастание)\n");
    printf("Ваш выбор: ");

    int choice;
    scanf("%d", &choice);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {

            int swap = 0;

            switch (choice) {
            case 1:
                if (database[j].rating < database[j + 1].rating) swap = 1;
                break;

            case 2:
                if (database[j].price > database[j + 1].price) swap = 1;
                break;

            case 3:
                if (database[j].rating < database[j + 1].rating)
                    swap = 1;
                else if (database[j].rating == database[j + 1].rating &&
                    database[j].price > database[j + 1].price)
                    swap = 1;
                break;

            default:
                printf("Неверный выбор!\n");
                return -1;
            }

            if (swap) {
                struct Software temp = database[j];
                database[j] = database[j + 1];
                database[j + 1] = temp;
            }
        }
    }

    printf("Данные успешно отсортированы!\n");
    printf("Для просмотра результата выберите 'Просмотреть все записи' в меню.\n");

    return 0;
}