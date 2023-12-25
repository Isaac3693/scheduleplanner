#include "scheduleStructs.h"

/*

Expected file input of the program: 

------------------------------------------------------------------------------------------------------------------
DATE:9/23/2023
9:30-12:20:enter name
12:20-14:30:another name
...
::END::
DATE
9:30-12:30:enter_name
12:20-20:40:andaoiniodsaniodnsajndsia
::END::
/eof


------------------------------------------------------------------------------------------------------------------

openFile(pathname): opens the file, if it doesn't exist, create a new one and return an empty scheduled_days

import has txtExtension function from previous project.

*/

//Enumeration for changing between the states of the fgets function (UPDATE: I have moved the enum declaration at the scheduleStructs.h header file)


//This function is supposed to update the list to make sure that it is up to date. We are modifying the list, and then we write to the file.
//INT *TODAY IS GOING TO BE IN THE FORMAT {MONTH, DATE, YEAR}
void update_file(char *path_name, size_t len, int *today, scheduled_days list, bool *is_up_to_date) {

    //Iterate and delete each item from the list until you have reached today (if i->date >= today)

    if (list->main_head == NULL) return; //The list is achieved from scheduled_days_open_file, , and if the head is null, then there is nothing on the linked list.

    //we are popping all items from the list such that these items came before today. We are going to implement a while loop by iterating the head

    while (list->main_head != NULL) {
        if (list->main_head->year < today[2]) {
            scheduled_days_pop_date_from_scheduled_days(list);
        } else if (list->main_head->year == today[2]) {

            if (list->main_head->month < today[0]) scheduled_days_pop_date_from_scheduled_days(list);
            else if (list->main_head->month == today[0]) {

                if (list->main_head->day < today[1]) scheduled_days_pop_date_from_scheduled_days(list);
                else break;
            
            } else break;
        } else break; //The year comes after today, so we don't pop it off the list.  
    }

    //After we have popped every single preceding day before today, we then write to the file for some changes
    //We are not freeing the list because we are only passing it from the previous declaration (which should have been made in the openFile function). This is a reason as to why this is void
    if (write_file(path_name, len, list)) printf("Check file for correct format\n");
    *is_up_to_date = true;
    return;

}

//We write the file always, no need to append the function.
int write_file(char *path_name, size_t path_len, scheduled_days list) {
    FILE *fp; 
    if ((fp = fopen(path_name, "w")) == NULL) {
        fprintf(stderr, "FOPEN ERROR\n");
        return 1;
    }

    //Iterate through the list and then write each consequent timeslot per day

    //list is empty, so we simply just write blank on the file and it doesn't go through the for loop expression.

    for (date i = list->main_head; i!= NULL; i = i->next_day) {

        fprintf(fp, "DATE:%d/%d/%d\n", i->month, i->day, i->year);
        for (timeslot j = i->head; j != NULL; j = j->next_timeslot) {
            //check if both the min start and min end are less than 10 (i.e, they need a leading zero to still take two characters.)
            if ((j->min_start < 10) && (j->min_end < 10)) fprintf(fp, "%d:0%d-%d:0%d:%s\n", j->hour_start, j->min_start, j->hour_end, j->min_end, j->task_name);
            else if (j->min_start < 10) fprintf(fp, "%d:0%d-%d:%d:%s\n", j->hour_start, j->min_start, j->hour_end, j->min_end, j->task_name);
            else if (j->min_end < 10) fprintf(fp, "%d:%d-%d:0%d:%s\n", j->hour_start, j->min_start, j->hour_end, j->min_end, j->task_name);
            else fprintf(fp, "%d:%d-%d:%d:%s\n", j->hour_start, j->min_start, j->hour_end, j->min_end, j->task_name);
        }

        if (i->next_day == NULL) fprintf(fp, "::END:: "); //It was creating an issue such that the final ::END:: closer had a whitespace and it was making a timeslot using an empty string after the newline.
        else fprintf(fp, "::END::\n");
    }

    fclose(fp);
    return 0;


}   

//assumes that the path_name has a txt extension (checked in main program).
//IF THIS RETURNS NULL, THEN THAT MEANS THAT THERE IS SOMETHING WRONG WITH THE TEXT FILE AND IT NEEDS TO BE OVERWRITTEN OR TELL THE USER THAT
//THEY NEED TO PUT A DIFFERENT FILE NAME OR DELETE THE CURRENT FILE THAT THEY HAVE INPUTTED PREVIOUSLY.
scheduled_days scheduled_days_open_file(char *path_name, size_t path_len) {
    FILE *fp;

    scheduled_days list = scheduled_days_create_scheduled_days();


    //Scenario 1: File doesn't exist and the user can decide to make a new one or not.

    if (!(fp = fopen(path_name, "r"))) {
        char input;
        printf("Inputted path does not exist, create new file (Y / N)?\n");
        scanf(" %c", &input);

        if (toupper(input) == 'Y') {

            if (!(fp = fopen(path_name, "w"))) {
                fprintf(stderr, "ERROR: UNABLE TO MAKE FILE\n");
                exit(1);
            } 

            return list; //Returns an empty list.
        }

        else exit(0); //User doesn't want to make a file, exit the program as intended

    }

    //Scenario 2: the filename does exist and so we can read the file (CONSIDER SITUATION IN WHICH THE FILE IS EMPTY)
    

    char content[BUFFER_SIZE];
    char other_content[32]; //This is use the strnpy function to an anonymous string (IMPORTANT NOTE: does the pre-allocated array have null terminators in every element? It doesn't seem like the case in Linux)
    int anticipated_dates = 0;
    //Probably make a pre-read in which the file pointer checks if there are any dates within the list. 
    //This applies to a scenario in which the user creates a new file, quits the program, then enters again with the empty file

    while(fgets(content, BUFFER_SIZE, fp)) {
        content[strlen(content) - 1] = '\0';
        strncpy(other_content, content, 4); //We know that the string length of the string "date" (and uppercase version of it) is just 4.. Keep it like that.
        other_content[4] = '\0';
        if (!strcmp(other_content, "DATE")) anticipated_dates++;
    }

    //If its zero, then return an empty schedule_list
    if (anticipated_dates == 0) return list;

    //Now the file pointer points (get it?) to the end line, so we need to reopen it again.

    if ((fp = freopen(path_name, "r", fp)) == NULL) {
        puts("FREOPEN FAILURE");
        exit(1); 
    }

    //The question: how can I make sure that ggets doesn't skip a single line in the file?
    enum string_parse_states state = DATE;
    date current_date_being_parsed;

    while (!feof(fp)) {
        fgets(content, BUFFER_SIZE, fp);
        content[strlen(content) - 1] = '\0';
        //Check the actual string
        if (!strcmp(strncpy(other_content, content, strlen("DATE")), "DATE")) state = DATE;
        else if (!strcmp(content, "::END::")) state = END;
        else state = TIMESLOT;

        //Do a switch statement, if its a date, then create a new one, if its end, append the date to the list, 
        //and if its a timeslot, parse the string and append the timeslot to the current date.

        switch (state) {
            case (DATE): {
                if ((current_date_being_parsed = date_parse_string_return_date((content + 5), strlen((content + 5)))) == NULL) {
                    fprintf(stderr, "ERROR IN RETURNING DATE, RETURNING NULL LIST\n"); 
                    scheduled_days_destroy_scheduled_days(list);
                    return NULL;
                    }
                break;
            }
            case (TIMESLOT):{
                if (date_append_timeslot_to_date(current_date_being_parsed, timeslot_parse_string_return_timeslot(content, strlen(content)))) {
                    fprintf(stderr, "ERROR IN APPENDING DATE, RETURNING NULL\n"); 
                    scheduled_days_destroy_scheduled_days(list);
                    return NULL;
                }
                break;
            }
            case (END): {
                if (scheduled_days_append_date_to_scheduled_days(list, current_date_being_parsed)) {
                    fprintf(stderr, "ERROR IN APPENDING DATE, RETURNING NULL\n"); 
                    scheduled_days_destroy_scheduled_days(list);
                    return NULL;               
                }
                break; //Not really necessary to add a break here? Or maybe add a default statement?
            }
        }

    }

    //Finishes reading, return list and close file
    fclose(fp);
    return list;



}

//I have to consider 3 types of strings to check: the date string, the timeslot string, and the ::END:: string

//ASSUME CONTENT HAS A NULL TERMINATOR AT THE END
//ALSO ASSUMES THAT THE NAME INSIDE OF THE STRING IS LESS THAN OR EQUAL TO 31 CHARACTERS
//WE ONLY NEED ONE END STRING
//TODO:there is something wrong with the implementation of this function. For example, one of the hours is putting out 82 for some bizarre reason. I ought to investigate this further because shit, I need to sleep.
timeslot timeslot_parse_string_return_timeslot(char *content, size_t len) {

    //nn:oo-pp:qq:NAMEOFSOMETHING
    timeslot tmp;
    char time_numbers[4][3];
    //For everytime that I initialize a string, I am literally going to declare all of its characters as null terminating characters 

    for (int i = 0; i < 4; i++) for (int j = 0; j < 3; j++) time_numbers[i][j] = '\0';
    
    char task_name[MAX_NAME_LENGTH];
    int index_of_dash = 0;
    int colon_counter = 0; //Counts amount of times the iteration passed through a colon
    int dash_counter = 0; //Assume that it will only be one, user might put dashes on the name
    for (int i = 0; i < len; i++) {

        if (content[i] == ':') {
            colon_counter++;

            //Should we add a default statement? 
            switch (colon_counter) {
                case (1): {
                    strncpy(time_numbers[0], content, i);
                    strncpy(time_numbers[1], (content + (i + 1)), 2);
                    break;
                }
                case (2): {
                    strncpy(time_numbers[2], (content + (index_of_dash + 1)), i - (index_of_dash + 1));
                    strncpy(time_numbers[3], (content + (i + 1)), 2);
                    break;
                }
                case (3): {
                    strcpy(task_name, (content + (i + 1)));
                    break;
                }
            }
        }

        else if (content[i] == '-' && dash_counter == 0) {
            dash_counter++;
            index_of_dash = i;
        }

    }

    tmp = timeslot_create_timeslot(atoi(time_numbers[0]), atoi(time_numbers[1]), atoi(time_numbers[2]), atoi(time_numbers[3]), task_name, strlen(task_name));
    return tmp;



}

//I can pass the content at index five "10/20/2023" (date_parse_string_return_date((content + 5), strlen((content + 5))))
date date_parse_string_return_date(char *content, size_t len) {

    date tmp;

    int slash_counter = 0;
    int date_arr[3];
    char copier[32];
    int index_of_previous_dash = 0;
    for (int i = 0; i < len && slash_counter < 2; i++) {

        if (content[i] == '/') {
            slash_counter++;

            switch (slash_counter) {
                case (1): {
                    date_arr[0] = atoi(strncpy(copier, content, i));
                    index_of_previous_dash = i;
                    break;
                }
                case (2): {
                    date_arr[1] = atoi(strncpy(copier, (content + index_of_previous_dash + 1), i - (index_of_previous_dash + 1)));
                    index_of_previous_dash = i;
                    date_arr[2] = atoi(strcpy(copier, (content + (i + 1))));
                    break;
                }
            }
        }

    }

    //Check if the integers have been properly allocated and there is no failure. Probably use GDB to debug any sort of features.
    
    if ((tmp = date_create_date(date_arr[0], date_arr[1], date_arr[2])) == NULL) {
        printf("Date returned NULL\n");
        return NULL;
    }

    return tmp;
}


int hasTxtExtension(char *path) {

    if (!strcmp(path, ".txt")) return 0; //if the user input is ".txt"
    if (!strcmp((path + (strlen(path) - 4)), ".txt")) return 1; return 0; //checks extension

}
