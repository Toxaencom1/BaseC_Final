#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "temp_functions.h"


/**
 * @brief Main function of the sensor data processing application.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return The exit status of the program.
 */
int main(int argc, char *argv[])
{
    int res = 0, month = 0, flag = 0;
    char fileName[MAX_FILE_NAME_SIZE] = "temperature_small.csv";
    opterr = 0;

    // Обработка аргументов
    while ((res = getopt(argc, argv, "dhf:sm:")) != -1)
    {
        switch (res)
        {
        case 'd':
            flag = 1;
            break;
        case 'h':
            helpArgInfo();
            return 0;            
        case 'f':
            printf("Found file argument '-f' = \"%s\".\n", optarg);
            strcpy(fileName, optarg);
            break;
        case 's':
            flag = 1;
            printf("Found 'Show Data' argument '-s'\n");
            break;
        case 'm':
            month = atoi(optarg);
            if (month >= 1 && month <= 12)            
                printf("Found month argument '-m' = \"%s\".\n", optarg);            
            else
            {
                printf("Month argument ERROR, exit\n");
                return 1;
            }
            break;        
        case '?':
            printf("Arguments Error found !\n");
            return 1;            
        };
    }    
    if (argc == 1)
    {
        noArgsInfo();
        return 0;
    }    
    printf("\n");

    // Окрытие файла и интеграция данных в память
    FILE *file;
    file = fopen(fileName, "r");
    if (file == NULL)
    {
        perror("File opening error: ");
        return 1;
    }    
    struct sensor *info = (struct sensor *)malloc(MAX_MEMORY_ALLOCATE * sizeof(struct sensor));
    int size = AddInfo(info, file, month);
    fclose(file);

    // Вызов функций программы
    if (flag)
        print(info, size);

    findAverage(info, size);
    findMaxAndMin(info, size);
    if (month == 0)
        showMonthlyStatistic(info, size);

    return 0;
}
