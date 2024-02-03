#include <stdio.h>
#include "temp_functions.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/**
 * @brief Adds a record to the sensor data.
 *
 * @param info Pointer to the sensor data array.
 * @param number Index to add the record.
 * @param year Year of the record.
 * @param month Month of the record.
 * @param day Day of the record.
 * @param hour Hour of the record.
 * @param minute Minute of the record.
 * @param temperature Temperature value of the record.
 */
void AddRecord(struct sensor *info, int number,
               uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, int8_t temperature)
{
    info[number].year = year;
    info[number].month = month;
    info[number].day = day;
    info[number].hour = hour;
    info[number].minute = minute;
    info[number].temperature = temperature;
}

/**
 * @brief Displays monthly statistics of sensor data.
 *
 * @param info Pointer to the sensor data array.
 * @param size Size of the sensor data array.
 */
void showMonthlyStatistic(struct sensor *info, int size)
{
    printf("Monthly statistic: \n\n");
    int count = 0;
    int empty = 0;
    struct sensor *temp = (struct sensor *)malloc(MAX_MEMORY_ALLOCATE / 12 * sizeof(struct sensor));
    printf("+-------+------------+-----+-----+\n");
    printf("| Month |   Average  | Min | Max |\n");
    printf("+-------+------------+-----+-----+\n");
    for (uint8_t m = 1; m <= 12; m++)
    {
        for (size_t j = 0; j < size; j++)
        {
            if (info[j].month == m)
            {
                temp[count++] = info[j];
                if (empty < 1)
                    empty++;
            }
        }
        if (empty)
        {
            printf("|   %2d  |   %8.2lf | %3d | %3d |\n",
                   m, findAverage(temp, count), findMin(temp, count), findMax(temp, count));
        }
        else
            printf("| Empty |            |     |     |\n");

        // обнуляю временныe данные
        memset(temp, 0, sizeof(temp));
        count = 0;
        empty = 0;
    }
    printf("+-------+------------+-----+-----+\n");
}

/**
 * @brief Reads sensor data from a file and adds it to the sensor data array.
 *
 * @param info Pointer to the sensor data array.
 * @param file Pointer to the input file stream.
 * @param calendarMonth Month for filtering the data (0 to add all data).
 * @return Number of records added.
 */
int AddInfo(struct sensor *info, FILE *file, int calendarMonth)
{
    int counter = 0;
    int fileErrorCounter = 0;
    int year, month, day, hour, minute, temperature;
    char line[MAX_DATA_LINE_LENGTH];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if ((sscanf(line, "%d;%d;%d;%d;%d;%d", &year, &month, &day, &hour, &minute, &temperature) == 6) &&
            year > 2000 &&
            month > 0 && month <= 12 &&
            day > 0 && day <= 31 &&
            hour >= 0 && hour <= 23 &&
            minute >= 0 && minute < 60 &&
            temperature >= (-99) && temperature <= 99) // проверка данных на валидность
        {
            fileErrorCounter++;
            if (calendarMonth == 0)
            {
                AddRecord(info, counter++, year, month, day, hour, minute, temperature);
            }
            else if (calendarMonth == month)
            {
                AddRecord(info, counter++, year, month, day, hour, minute, temperature);
            }
            else
                continue;
        }
        else
        {
            printf("DATA error in File, line: %d\n", ++fileErrorCounter);
        }
    }
    return counter;
}

/**
 * @brief Calculates the average temperature from sensor data.
 *
 * @param info Pointer to the sensor data array.
 * @param size Size of the sensor data array.
 */
double findAverage(struct sensor *info, int size)
{
    double sum = 0.0;
    for (int i = 0; i < size; ++i)
    {
        sum += info[i].temperature;
    }
    return sum / size;
    // printf("\nAverage of period: %.2lf°\n", res);
}

/**
 * @brief Finds the maximum and minimum temperatures from sensor data.
 *
 * @param info Pointer to the sensor data array.
 * @param size Size of the sensor data array.
 */
int findMin(struct sensor *info, int size)
{
    int8_t min = INT8_MAX;

    for (int i = 0; i < size; ++i)
    {
        if (info[i].temperature < min)
        {
            min = info[i].temperature;
        }
    }
    return min;
    // printf("Minimal temperature: %d°\nMaximal temperature: %d°\n\n", min, max);
}
int findMax(struct sensor *info, int size)
{
    int8_t max = INT8_MIN;

    for (int i = 0; i < size; ++i)
    {
        if (info[i].temperature > max)
        {
            max = info[i].temperature;
        }
    }
    return max;
    // printf("Minimal temperature: %d°\nMaximal temperature: %d°\n\n", min, max);
}

/**
 * @brief Prints sensor data.
 *
 * @param info Pointer to the sensor data array.
 * @param size Number of records to print.
 */
void print(struct sensor *info, int size)
{
    printf("\n=-Index-=====-Date-=====-Time-===-Temp-===\n");
    for (int i = 0; i < size; i++)
    {
        printf("| %06d | %04d-%02d-%02d | %2d:%2d | t = %3d° |\n", i,
               info[i].year,
               info[i].month,
               info[i].day,
               info[i].hour,
               info[i].minute,
               info[i].temperature);
    }
    printf("==========================================\n");
}

/**
 * @brief Displays information about using the application.
 */
void noArgsInfo()
{
    printf("This is console app to show your montly/year statistic of sensor collected data.\n\
            How to use this app please enter '-h' argument key");
}

/**
 * @brief Displays information about available command-line arguments.
 */
void helpArgInfo()
{
    printf("\n    -d fast execution app to demonstrate statistic output with default file \"temperature_small.csv\".\n\n");
    printf("\n    -h obtaining information about possible launch keys with a brief description of their purpose.\n\n");
    printf("    -f This arg allows you to select your sensor data file, in the format \
YEAR;MONTH;DAY;HOUR;MINUTE;TEMPERATURE.\n \
(with data format - '2021;05;16;01;01;10')\n \
Default file is \"temperature_small.csv\"\n Value is required.\n\
 Example: .\\sensor_app.exe -f \"temperature_big.csv\" \n \n\n");
    printf("    -s to show/print correct period data from your file.\n\n");
    printf("    -m This arg allows you to select the month for which you want to view information. Value is required.\n \
Example \".\\sensor_app.exe -m 1\".\n\n");
}