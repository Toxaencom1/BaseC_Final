#include <stdint.h>
#define MAX_FILE_NAME_SIZE 30
#define MAX_DATA_LINE_LENGTH 25
#define MAX_MEMORY_ALLOCATE 600000
#define N 6
struct sensor {
uint8_t day;
uint8_t month;
uint16_t year;
uint8_t hour;
uint8_t minute;
int8_t temperature;
};

void AddRecord(struct sensor *info, int number,
    uint16_t year, uint8_t month, uint8_t day,uint8_t hour, uint8_t minute,  int8_t temperature);
int AddInfo(struct sensor *info, FILE *file, int calendarMonth);
void showMonthlyStatistic(struct sensor *info, int size);
double findAverage(struct sensor *info, int size);
int findMin(struct sensor *info, int size);
int findMax(struct sensor *info, int size);
void print(struct sensor* info,int size);
void noArgsInfo();
void helpArgInfo();