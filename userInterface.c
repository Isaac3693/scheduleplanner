#include "userInterface.h"

/*

WHAT IS THIS HEADER FILE FOR

employing the user commands and the user choices for the program.

print <date>
print today
print tomorrow (take the time input and then access the date)
show days_planned (that can be done with a loop of the date_print_date function)

/Can they make a schedule for today?

schedule today
schedule tomorrow
schedule <date>

delete today
delete <date>

*/



/*
COMMANDS FOR THE MAKE_SCHEDULE FUNCTION

This is used to make functions and edit dates that we are working with. create a new date for a call through 

print
make_timeslot <time_start> <time_end> <name_of_task>
del <time_start>
quit //Doesn't save any changes
save //Appends the date to the list and then goes back to the main menu.
/I made a delete function that takes the timestart as the index because some may have the same task at different times (I guess I was smart about this.)


*/


int make_schedule(scheduled_days list, int *day) {
    //How would I pass the date that the user has putted? 
    //ASSUME THAT DAY HAS 3 ELEMENTS IN THE ARRAY, NOTHING MORE, NOTHING LESS.
    args timeslot_args;
    char user_input[BUFFER_SIZE];

    date current_date;
    if ((current_date = date_create_date(day[0], day[1], day[2])) == NULL) {
        fprintf(stderr, "ERROR IN CREATING DATE\n");
        return 1;
    }

    //Now we have a day that we are working on and we are ready to add it to the list.

    while (1) {
        printf("(Create Schedule for %d/%d/%d)", day[0], day[1], day[2]);
        fgets(user_input, BUFFER_SIZE, stdin);
        user_input[strlen(user_input) - 1] - '\0';
        if ((timeslot_args = c_arg(user_input, strlen(user_input))) == NULL) {printf("Incorrect arg format\n"); destroy_args(timeslot_args); continue;}

        //Now do a continuous if else loop that takes into account of the many things the user wants to do

        //User wants to quit without making any changes
        if (!strcmp(timeslot_args->argv[0], "quit")) {
            date_destroy_date(current_date);
            destroy_args(timeslot_args);
            return 0;
        }
        //User wants to save the changes to the schedule and append it to the list
        else if (!strcmp(timeslot_args->argv[0], "save")) {
            scheduled_days_append_date_to_scheduled_days(list, current_date);
            destroy_args(timeslot_args);
            return 0;
        }
        //User wants to add a timeslot to the date
        else if(!strcmp(timeslot_args->argv[0], "make_timeslot")) {
            //()make_timeslot <time_start> <time_end> <name> 
            //argc must be 4, the middle two strings must be parsed into times.

            int *time_start, *time_end;

            //We must check that there exists 4 arguments because we need to check them in the other tests, or else we are reading who the fuck knows what
            if (timeslot_args->argc != 4) {printf("insufficient arguments\n");}

            //Super duper long statements make my day
            else if (((time_start = parse_input_return_time(timeslot_args->argv[1], strlen(timeslot_args->argv[1]))) == NULL) || ((time_end = parse_input_return_time(timeslot_args->argv[2], strlen(timeslot_args->argv[2]))) == NULL)) printf("Unable to make times from given input\n");

            //Normal execution, everything is normal and we can append the timeslot to the list.
            else if (!date_append_timeslot_to_date(current_date, timeslot_create_timeslot(time_start[0], time_start[1], time_end[0], time_end[1], timeslot_args->argv[3], strlen(timeslot_args->argv[3])))) printf("Timeslot added\n");

        }

        //User wants to delete a timeslot from the date
        else if(!strcmp(timeslot_args->argv[0], "delete")) {

            int *time_index_for_deletion;
            if (timeslot_args->argc < 2) printf("insufficient arguments\n");

            else if ((time_index_for_deletion = parse_input_return_time(timeslot_args->argv[1], strlen(timeslot_args->argv[1]))) == NULL) printf("Inappropriate time format\n");

            else if (!date_delete_timeslot_from_date(time_index_for_deletion[0], time_index_for_deletion[1], current_date)) {printf("Timeslot successfully deleted.\n"); free(time_index_for_deletion);}


        }

        else if (!strcmp(timeslot_args->argv[0], "print")) date_print_date(current_date);


        else printf("Unknown command\n");

        destroy_args(timeslot_args);

    }

    //in case that I didn't look into the control flow, I should do a print statement here and put a return 0 over here

    printf("MAKESCHEDULE FUNCTION FLOW WENT HERE\n");
    return 0;

}

//Not me copying and pasting the exact control flow of the function into this new one... Because making a function do many things (at times) can get quite tedious.
//Gone are the days in which we have to write as little code as possible... Write functions that may do similar things, but are different in some ways... (is that my coding philosophy?)
int edit_schedule(scheduled_days list, date current_date) {

    args timeslot_args;
    char user_input[BUFFER_SIZE];

    //CURRENT_DATE IS IN THE LIST


    //Now we have a day that we are working on and we are ready to add it to the list.

    while (1) {
        printf("(Edit Schedule for %d/%d/%d)", current_date->month, current_date->day, current_date->year);
        fgets(user_input, BUFFER_SIZE, stdin);
        user_input[strlen(user_input) - 1] - '\0';
        if ((timeslot_args = c_arg(user_input, strlen(user_input))) == NULL) {printf("Incorrect arg format\n"); destroy_args(timeslot_args); continue;}

        //Now do a continuous if else loop that takes into account of the many things the user wants to do

        //User wants to quit (In this scenario, all of the changes that the user made cannot be undone unless we make another heuristic that keeps the old date in check.)
        if (!strcmp(timeslot_args->argv[0], "save")) {
            destroy_args(timeslot_args);
            return 0;
        }
        //User wants to add a timeslot to the date
        else if(!strcmp(timeslot_args->argv[0], "make_timeslot")) {
            //()make_timeslot <time_start> <time_end> <name> 
            //argc must be 4, the middle two strings must be parsed into times.

            int *time_start, *time_end;

            //We must check that there exists 4 arguments because we need to check them in the other tests, or else we are reading who the fuck knows what
            if (timeslot_args->argc != 4) {printf("insufficient arguments\n");}

            //Super duper long statements make my day
            else if (((time_start = parse_input_return_time(timeslot_args->argv[1], strlen(timeslot_args->argv[1]))) == NULL) || ((time_end = parse_input_return_time(timeslot_args->argv[2], strlen(timeslot_args->argv[2]))) == NULL)) printf("Unable to make times from given input\n");

            //Normal execution, everything is normal and we can append the timeslot to the list.
            else if (!date_append_timeslot_to_date(current_date, timeslot_create_timeslot(time_start[0], time_start[1], time_end[0], time_end[1], timeslot_args->argv[3], strlen(timeslot_args->argv[3])))) printf("Timeslot added\n");

        }

        //User wants to delete a timeslot from the date
        else if(!strcmp(timeslot_args->argv[0], "delete")) {

            int *time_index_for_deletion;
            if (timeslot_args->argc < 2) printf("insufficient arguments\n");

            else if ((time_index_for_deletion = parse_input_return_time(timeslot_args->argv[1], strlen(timeslot_args->argv[1]))) == NULL) printf("Inappropriate time format\n");

            else if (!date_delete_timeslot_from_date(time_index_for_deletion[0], time_index_for_deletion[1], current_date)) {printf("Timeslot successfully deleted.\n"); free(time_index_for_deletion);}


        }

        else if (!strcmp(timeslot_args->argv[0], "print")) date_print_date(current_date);

        else printf("Unknown command\n");

        destroy_args(timeslot_args);

    }

    //in case that I didn't look into the control flow, I should do a print statement here and put a return 0 over here

    printf("EDITSCHEDULE FUNCTION FLOW WENT HERE\n");
    return 0;



}

//Issue, there is a problem with checking the content of strncpy, we have to assume that the user is not stupid enough to put the right info
//TODO: TEST THIS PROGRAM AND OBSERVE BEHAVIOUR IN CASE THE USER PUTS THE WRONG FORMAT FOR THE TIME.
int *parse_input_return_time(char *input, size_t len) {
    //Most often has the input --:-- (wait, it's that easy?)

    int *time_arr;
    if ((time_arr = (int*)malloc(sizeof(int) * 2)) == NULL) return NULL;
    char copier[MAX_NAME_LENGTH];
    for (int i = 0; i < len; i++) if (input[i] == ':') {

        time_arr[0] = atoi(strncpy(copier, input, i));
        time_arr[1] = atoi(strcpy(copier, (input + i + 1)));

        if (!has_correct_time_format(time_arr)) {free(time_arr); return NULL;}

        return time_arr;
    }

    return NULL;
}

bool has_correct_time_format(int *time_arr) {
    return ((time_arr[0] > 0) && (time_arr[0] < 24) && (time_arr[1] > 0) && (time_arr[1] < 60));
}

//Fortunately for the user, nothing is stopping them from scheduling a day that is in the year INT_MAX, meaning that if they live for an extremely long time (assuming we can now put our brains in immortal mechanical machines, which is highly unprobabilistic), they can make a time capsule in the form of a schedule...
bool has_correct_date_format(int *date_arr) {
    return((date_arr[0] > 0) && (date_arr[0] < 13) && (date_arr[1] > 0) && (date_arr[1] < 32) && (date_arr[2] > 0));
}


/*

SUPPOSED LOGICAL ERROR FOR THIS FUNCTION:
  It is hard to implement an algorithm that checks if a specific day exists for that year, meaning that the 
  user can put any date in between 0 & 31 for any month (for example, February 31st, which is not possible). I also leave this to the reader as an exercise.

*/
int *parse_input_return_date(char *input, size_t len) {
    //Must be of string "--/--/----" (month/day/year)

    int *date_arr;
    if ((date_arr = (int*)malloc(sizeof(int) * 3)) == NULL) return NULL;
    char copier[MAX_NAME_LENGTH];
    for (int i = 0, slash_counter = 0, previous_slash_index; i < len; i++) {
        if (input[i] == '/') {
            slash_counter++;
            switch (slash_counter) {
                case(1):
                    date_arr[0] = atoi(strncpy(copier, input, i));
                    break;
                case(2):
                    date_arr[1] = atoi(strncpy(copier, (input + previous_slash_index + 1), i - (previous_slash_index - 1)));
                    previous_slash_index = i;
                    date_arr[2] = atoi(strcpy(copier, (input + i + 1)));
                    if (!has_correct_date_format(date_arr)) {free(date_arr); return NULL;}
                    return date_arr;
            }

            previous_slash_index = i; //In case that the iteration has reached the second slash to make the second switch case work
        } 
    }

    //In case that there is a failure to append the string, return NULL
    return NULL;
}

void print_time(struct tm *time_struct) {
    printf("%d/%d/%d, ", time_struct->tm_mon + 1,  time_struct->tm_mday, time_struct->tm_year + 1900);

    //Go through a conditional to check if the current minute is a single digit; in that case, print it as '0n' n is a natural number less than 10
    if (time_struct->tm_min < 10) printf("%d:0%d\n", time_struct->tm_hour, time_struct->tm_min);
    else printf("%d:%d\n", time_struct->tm_hour, time_struct->tm_min);
}

int *make_date_arr(struct tm *time_struct) {
    int *tmp;
    if ((tmp = (int*)malloc(sizeof(int) * 3)) == NULL) return NULL;
    tmp[0] = time_struct->tm_mon + 1;
    tmp[1] = time_struct->tm_mday;
    tmp[2] = time_struct->tm_year + 1900; //counts the amount of years that have passed since 1900

    return tmp;

}

//I need a helper function that compares the dates and sees which one comes before, after, or the same date
//For the implementation with the main program, today will be current_date and the user_inputted date will be selected_date.
enum date_comparison_state current_date_comparison_with_selected_date(int *current_date, int *selected_date) {
    //remember that the format of the arrays are {month, day, year}
    //We are comparing whether current_date comes before, after or is the same as selected date
    if (selected_date[2] < current_date[2]) return AFTER;
    else if (selected_date[2] > current_date[2]) return BEFORE;
    else {
        //The condition such that the years are the same and now we have to compare the months of the two arrays
        if (selected_date[0] < current_date[0]) return AFTER;
        else if (selected_date[0] > current_date[0]) return BEFORE;
        else {
            //The months are the same and we have to check the comparison using the days
            if (selected_date[1] < current_date[1]) return AFTER;
            else if (selected_date[1] > current_date[1]) return BEFORE;
            else return SAME;
        }
    }
}

void print_help(void) {
    PRINT_DASHES;
    puts("schedule <date>");
    printf("\tCreates or modifies a schedule for the user date.\n");
    puts("delete <date>");
    printf("\tDeletes a date that is on the list\n");
    puts("print <date (optional)>");
    printf("\tPrints a selected date to see the contents. If <date> is omitted, by default prints the schedule for today if it exists\n");
    puts("quit");
    printf("exits the program\n");
}

void print_edit_schedule_help(void) {
    PRINT_DASHES;
    puts("print");
    printf("\tprints the schedule for the date\n");
    puts("make_timeslot <time_start> <time_end> <name_of_task>");
    printf("\tCreates a timeslot for the date. time_start and time_end must be in military format\n");
    puts("delete <time_start>");
    printf("\tdeletes a timeslot from the given start time\n");
    puts("save");
    printf("\tSaves changes to the timeslot and returns to the main menu\n");
}

void print_make_schedule_help(void) {
    print_edit_schedule_help();
    puts("quit");
    printf("\tReturns to the main menu without adding the currently worked date into the list of scheduled days\n");
}