#include "scheduleStructs.h"

/*

CONTAINS ALL THE METHODS AND PROPERTIES OF THE SCHEDULELIST STRUCT CONTAINING THE DATES AND THE TIMESLOTS IN EACH DATE IN THE LIST.

EMPLOYING A QUEUE-LIKE STRUCTURE

*/

/*

CONSIDERATIONS: Check the user's date to see how many times we need to pop off the list

*/

int scheduled_days_delete_day_from_scheduled_days(scheduled_days list, int day, int month, int year) {
    if (list->main_head == NULL) {
        printf("list is empty\n");
        return 1;
    }

    //Check if the head is equal to the search
    if (year == list->main_head->year && month == list->main_head->month && day == list->main_head->day) {
        if (list->main_head->next_day == NULL) {date_destroy_date(list->main_head); list->main_head == NULL; return 0;}
    }
}

//returns the date that has the contents of the specified month, day, and year. Once again, CHECK THE CONTENTS OF THE RETURN VALUE  
date scheduled_days_peek_date(scheduled_days list, int day, int month, int year) {

    if (list->main_head == NULL) return NULL;

    for (date tmp = list->main_head; tmp != NULL; tmp = tmp->next_day) if (year == tmp->year && month == tmp->month && day == tmp->day) return tmp; 

    return NULL;
}


void scheduled_days_pop_date_from_scheduled_days(scheduled_days list) {
    //IF CALLING THIS FUNCTION, MAKE SURE TO CHECK ITS RETURN VALUE
    //ALSO CHECK THE VALUE OF THE SCHEDULE LIST MAIN HEAD AFTER CALLING THIS FUNCTION.
    if (list->main_head == NULL) return;
    date tmp = list->main_head;
    list->main_head = list->main_head->next_day;
    return;
}

scheduled_days scheduled_days_create_scheduled_days(void) {
    scheduled_days tmp;
    if ((tmp = (scheduled_days)malloc(sizeof(struct scheduled_days_struct))) == NULL) {
        fprintf(stderr, "ERROR: FAILED TO ALLOCATE TO SCHEDULED_DAYS STRUCT\n");
        free(tmp);
        return NULL;
    }

    tmp->main_head = NULL;

    return tmp;
}

void scheduled_days_destroy_scheduled_days(scheduled_days list) {
    if (list->main_head == NULL) {free(list); return;}

    for (date tmp = list->main_head; list->main_head != NULL; tmp = list->main_head) {
        list->main_head = list->main_head->next_day;
        date_destroy_date(tmp);
    }

    free(list);
}

int scheduled_days_append_date_to_scheduled_days(scheduled_days list, date schedule) {
    //fuck, we have to apply the entire algorithm again to make sure that it is ordered. Luckily, it isn't that complicated like the times
    //12-8-2023 note, It's complicated

    //Case 1: the list is empty
    if (list->main_head == NULL) {list->main_head = schedule; return 0;}
    //Scenario 2: There is a head in the list, but check if the appended date comes before the head
    if (list->main_head->year == schedule->year) {
        //The list_head and the appendee have the same year, but check to see if they have the same month and day. This section under this else if is the month checker

        if (list->main_head->month == schedule->month) {
            //This is the small section to check if the days are the same. There cannot be two days
            if (list->main_head->day > schedule->day) {
                schedule->next_day = list->main_head;
                list->main_head = schedule;
                return 0;
            } else if (list->main_head->day == schedule->day) {
                printf("Unable to add date to schedule list, a schedule already exists in that slot\n");
                return 1;
            } //else, go to the next iteration statements

        } else if (list->main_head->month > schedule->month) {
            schedule->next_day = list->main_head;
            list->main_head = schedule;
            return 0;
        }

    } else if (list->main_head->year > schedule->year) {
            schedule->next_day = list->main_head;
            list->main_head = schedule;
            return 0;
    }

    //Flow goes here because the schedule comes after the date

    //Check if the head has a next pointer pointing to... you know, something at least
    if (list->main_head->next_day == NULL) {
        list->main_head->next_day = schedule;
        return 0;
    }

    //There are multiple items within the set, so we have to iterate through the list to add the schedule to the list. If it doesn't fit, then return 1.

    //For each item in the list, check to see if there is an open spot available before that slot

    //ASSUME THAT J COMES BEFORE SCHEDULE

    for (date i = list->main_head->next_day, j = list->main_head; i != NULL; i = i->next_day, j = j->next_day) {

        if (i->year > schedule->year) {
            j->next_day = schedule;
            schedule->next_day = i;
            return 0;
        } else if (i->year == schedule->year) {

            if (i->month > schedule->month) {
                j->next_day = schedule;
                schedule->next_day = i;
                return 0;
            } else if (i->month == schedule->month) {
                if (i->day > schedule->day) {
                    j->next_day = schedule;
                    schedule->next_day = i;
                    return 0;
                } else if (i->day == schedule->day) {
                    printf("Unable to add date to schedule list, a schedule already exists in that slot\n");
                    return 1;
                } //Else, continue on the loop
            } //Else, i->month comes before schedule
        } //Else, i->year comes before schedule->year, go to next item

        //i comes before schedule, but check if i's next is NULL

        if (i->next_day == NULL) {i->next_day = schedule; return 0;}

    }
    
}

void scheduled_days_print_entire_list(scheduled_days list) {
    if (list->main_head == NULL) printf("List is empty\n");
    else for (date i = list->main_head; i != NULL; i  = i->next_day) date_print_date(i);
}