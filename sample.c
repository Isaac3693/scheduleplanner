#include <stdio.h>
#include <time.h>
#include <stdlib.h>




int main(void) {

    struct tm *time_struct;
    time_t t;
    t = time(NULL);
    time_struct = localtime(&t);
    printf("%s\n", asctime(time_struct));
    printf("day is %d, month is %d, year is %d, hour is %d\n", time_struct->tm_mday, time_struct->tm_mon, time_struct->tm_year, time_struct->tm_hour);
    printf("min is %d\n", time_struct->tm_min);

    

    return 0;
}