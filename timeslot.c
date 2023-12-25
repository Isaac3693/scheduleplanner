#include "scheduleStructs.h"


//timeslot acts like a node on a linked list, day is the linked list containing all timeslots

timeslot timeslot_create_timeslot(int hour_start, int min_start, int hour_end, int min_end, char* name, size_t name_length) {

    //Check if name_length exceeds MAX_NAME_LENGTH - 1 (MAX_NAME_LENGTH includes the space for the null terminating character)


    if (name_length > MAX_NAME_LENGTH - 1) {
        fprintf(stderr, "ERROR: UNABLE TO MAKE TIMESLOT FOR \"%s\", name is too long\n", name);
        return NULL;
    } 
    //If the user has inputted some weird numbers.
    else if ((hour_end < hour_start) || (hour_end == hour_start && (min_end < min_start))) {printf("Unable to make timeslot, no time interval in between\n"); return NULL;}
    else if ((hour_start < 0 || hour_start > 23) || (min_start < 0 || min_start > 59)) {printf("Unable to make timeslot, time inputs inappropriate\n"); return NULL;}

    timeslot tmp;

    if ((tmp = (timeslot)malloc(sizeof(struct timeslot_struct))) == NULL) {
        fprintf(stderr, "ERROR: UNABLE TO ALLOCATE MEMORY FOR TIMESLOT \"%s\"\n", name);
        return NULL;
    }
    //Assign the variables to the timeslot
    tmp->hour_start = hour_start; tmp->hour_end = hour_end;  tmp->min_start = min_start; tmp->min_end = min_end;
    strcpy(tmp->task_name, name);
    tmp->next_timeslot = NULL;

    return tmp;
}

date date_create_date(int month, int day, int year) {
    date tmp;
    if ((tmp = (date)malloc(sizeof(struct timeslot_list_struct))) == NULL) {
        fprintf(stderr, "ERROR: FAILED TO ALLOCATE %d/%d/%d", day, month, year);
        return NULL;
    }
    tmp->month = month; tmp->year = year; tmp->day = day;
    tmp->head = NULL;
    tmp->next_day = NULL;
    return tmp;
}

int date_append_timeslot_to_date(date existing_date, timeslot slot) {

    /************************************
     * 
     * 
     * December 5th update: I have written the entire function. I will not be surprised if this doesn't work (guaranteed, it will not work).
     * 
     * 
    *************************************/

   //The main reason as to why I get a shit load of segmentation faults: I don't check whether timeslot is NULL.
   if (slot == NULL) return 1;

    //Scenario 1: list is empty
    if (existing_date->head == NULL) {existing_date->head = slot; return 0;}
    //Scenario 2: There exists a head in the list, check if the timeslot goes before, first elif: same hour, if slot's hour is previous, assign it previous
    else if (existing_date->head->hour_start == slot->hour_end) {

        //Check if minutes are same, if slot is greater, (overlap), then return 1, else, assign it to the head and the previous head becomes the next slot
        if (existing_date->head->min_start >= slot->min_end) {
            slot->next_timeslot = existing_date->head;
            existing_date->head = slot;
            return 0;
        } else return 1; //Throws an error to system and won't put in the list.

    } else if (existing_date->head->hour_start > slot->hour_end) {
        slot->next_timeslot = existing_date->head;
        existing_date->head = slot;
        return 0;
    }
    //Head comes before slot, check if it is the only one but make sure its hour end doesn't overlap with slot's hour start
    else if (existing_date->head->next_timeslot == NULL) {

        if (existing_date->head->hour_end < slot->hour_start) {existing_date->head->next_timeslot = slot; return 0;}
        else if (existing_date->head->hour_end == slot->hour_start) {

            if (existing_date->head->min_end <= slot->min_start) {existing_date->head->next_timeslot = slot; return 0;}
            else return 1;

        }
    }

    //General Scenario (3): There exists more slots in the linked list and we need to iterate with two slot variables
    
    //This is a huge ass for loop, I like the way that it looks.
    for (timeslot i = existing_date->head->next_timeslot, p = existing_date->head; i != NULL; i = i->next_timeslot, p = p->next_timeslot) {

        //If the slot's end hour comes before the current index's start hour
        if (i->hour_start > slot->hour_end) {
            //If the slot's start hour comes after the previous index's last hour
            if (p->hour_end < slot->hour_start) {
                p->next_timeslot = slot;
                slot->next_timeslot = i;
                return 0;
            } else if (p->hour_end == slot->hour_start) { //If the previous's end hour is the same as the slot's start hour
                //if the previous index's last minute is the same as or less than the slot's minute start
                if (p->min_end <= slot->min_start) {
                    p->next_timeslot = slot;
                    slot->next_timeslot = i;
                    return 0;
                } else return 1; //Overlap to the minute
            } else return 1; //If the previous end hour is greater than the slot's start hour (overlap)

        } else if (i->hour_start == slot->hour_end) { //If the current index's hour start collides with slot's end hour

            //If current index's minute start is later or at the same time as slot's minute end
            if (i->min_start >= slot->min_end) {

                //copy and paste -----------------------------------------------------------------------------------------------------

                //If the slot's start hour comes after the previous index's last hour
                if (p->hour_end < slot->hour_start) {
                    p->next_timeslot = slot;
                    slot->next_timeslot = i;
                    return 0;
                } else if (p->hour_end == slot->hour_start) { //If the previous's end hour is the same as the slot's start hour
                    //if the previous index's last minute is the same as or less than the slot's minute start
                    if (p->min_end <= slot->min_start) {
                        p->next_timeslot = slot;
                        slot->next_timeslot = i;
                        return 0;
                    } else return 1; //Overlap to the minute
                } else return 1; //If the previous end hour is greater than the slot's start hour (overlap)

                //copy and paste -----------------------------------------------------------------------------------------------------


            } else return 1; //Overlaps

        } else if (i->next_timeslot == NULL) { //If it seems that there is no other timeslot right after

            if (i->hour_end < slot->hour_start) {
                i->next_timeslot = slot;
                return 0;
            } else if (i->hour_end == slot->hour_start) {
                if (i->min_end <= slot->min_start) {
                    i->next_timeslot = slot;
                    return 0;
                } else return 1; //Overlap with minutes 
            } else return 1; //current index's hour end overlaps with slot's hour start.

        }
    }
    //
    fprintf(stderr, "wtf error\n");
    return 1;
}

//Will most likely need to implement a helper function to transmit the user input to a correct format in terms of the time
int date_delete_timeslot_from_date(int starting_hour, int starting_minute, date schedule) {

    //Case 1: check if the head is NULL, then return 0;
    if (schedule->head == NULL) {printf("No timeslots in schedule\n"); return 0;}

    if ((schedule->head->hour_start == starting_hour) && (schedule->head->min_start == starting_minute)) {
        //free the memory, then assign the head to NULL just in case
        timeslot_destroy_timeslot(schedule->head);
        schedule->head = NULL;
        return 0;
    }

    //Case 2: General scenario, have a previous pointer and a next pointer. 

    if (schedule->head->next_timeslot != NULL) {
        //i is the current timeslot index and j is the one preceding the current timeslot
        for (timeslot i = schedule->head->next_timeslot, j = schedule->head; i != NULL; i = i->next_timeslot, j = j->next_timeslot) {

            if ((i->hour_start == starting_hour) && (i->min_start == starting_minute)) {

                //Check if there is a next pointer to the current index
                if (i->next_timeslot != NULL) {
                    timeslot k = i->next_timeslot;
                    j->next_timeslot = k;
                    timeslot_destroy_timeslot(i);
                } else { //i is the last timeslot within the list.
                    j->next_timeslot = NULL;
                    timeslot_destroy_timeslot(i);
                }

                return 0;
            }
        }
    }  
    
    return 1; //We have already checked that the head's timeslot next is NULL, so there is no need to check or iterate.

}

int timeslot_edit_timeslot_name(char* old_name, char* new_name, size_t len, date schedule) {
    //Check if the new name that the user has inputted is greater than the intended length of any task_name limit
    if (len >= MAX_NAME_LENGTH) {printf("Name is too long\n"); return 0;}

    //Iterate through each timeslot in the list and see if the name matches with the old name and then if so, change it to the new_name variable
    for (timeslot i = schedule->head; i != NULL; i = i->next_timeslot) if (!strcmp(i->task_name, old_name)) {strcpy(i->task_name, new_name); return 0;}

    //Failed to find the task within the list.
    return 1;
}

//Yes, of course I have to write a super long name for a function that only does a free call on a pointer.
void timeslot_destroy_timeslot(timeslot slot) {
    free(slot);
}

void date_destroy_date(date schedule) {

    timeslot i = schedule->head;

    for (timeslot i = schedule->head; schedule->head != NULL; i = schedule->head) {
        schedule->head = schedule->head->next_timeslot;
        timeslot_destroy_timeslot(i);
    }

    free(schedule);
}

void date_print_date(date schedule) {

    if (schedule->head == NULL) {printf("Date is empty\n"); return;}

    PRINT_DASHES;
    printf("Selected date: %d/%d/%d\n", schedule->month, schedule->day, schedule->year);
    puts(" ");
    for (timeslot i = schedule->head; i != NULL; i = i->next_timeslot) {
        //Print the contents of each individual timeslot like (9:20 - 14:23: eat ass)
        printf("%d:", i->hour_start);
        if (i->min_start < 10) printf("0%d - ", i->min_start); else printf("%d - ", i->min_start);
        printf("%d:", i->hour_end);
        if (i->min_end < 10) printf("0%d: ", i->min_end); else printf("%d: ", i->min_end);
        printf("%s\n", i->task_name);
    }
    PRINT_DASHES;
    puts(" ");
}