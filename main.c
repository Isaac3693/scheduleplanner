#include "scheduleStructs.h"



int main(int argc, char *argv[]) {

    char user_input[128];
    args arg;
    struct tm *time_struct;
    time_t t;

    date thing;

    //I am not sure if I want to pass a double pointer to the update_time function;


    //Start the thread to keep track of the time


    while (1) {

        //While the program runs, keep track of the time

        t = time(NULL);
        time_struct = localtime(&t);

        printf("()");
        fgets(user_input, 128, stdin);
        user_input[strlen(user_input) - 1] = '\0';

        if ((arg = create_arg(user_input, strlen(user_input))) == NULL) {
            printf("incorrect arg format\n");
            continue;
        }

        for (int i = 0; i < arg->argc; i++) printf("argv[%d] is \"%s\"\n", i, arg->argv[i]);

        //Going through a long conditional loop to check the user inputs 
        if (!strcmp(arg->argv[0], "quit")) break;
        else if (!strcmp(arg->argv[0], "show_time")) print_time(time_struct);
        else if (!strcmp(arg->argv[0], "make_schedule")) {
            thing = date_create_date(time_struct->tm_mon + 1, time_struct->tm_mday, time_struct->tm_year + 1900);
            printf("Day is %d, month is %d, year is %d\n", thing->day, thing->month, thing->year);

            args schedule_arg;

            
        }
        
        destroy_args(arg);
    }


    puts("Return success");
    return 0;
}


void print_time(struct tm *time_struct) {
    printf("%d/%d/%d, ", time_struct->tm_mon + 1,  time_struct->tm_mday, time_struct->tm_year + 1900);

    //Go through a conditional to check if the current minute is a single digit; in that case, print it as '0n' n is a natural number less than 10
    if (time_struct->tm_min < 10) printf("%d:0%d\n", time_struct->tm_hour, time_struct->tm_min);
    else printf("%d:%d\n", time_struct->tm_hour, time_struct->tm_min);
}