#ifndef SCHEDULE_STRUCTS
#define SCHEDULE_STRUCTS

//Will contain all the libraries necessary for the application

#include "bufferlimits.h"
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "args.h" //Make sure to see for any overwritten macros (check header guards to make sure nothing gets changed.)

#define PRINT_DASHES printf("---------------------------------------------------------------\n")

/*************************************************************************
 * 
 * 
 * We don't know how many timeslots the user is going to put, and we can't ask a number,
 *   so a dynamic array will be of no use to us. Fuck, linked lists are really carrying me.
 * 
 * So it is a linked list on top of a linked list, or on top of a queue?
 * 
 * 
**************************************************************************/

enum string_parse_states {DATE, TIMESLOT, END};


typedef struct timeslot_struct {
    int hour_start;
    int hour_end;
    int min_start;
    int min_end;
    char task_name[MAX_NAME_LENGTH];
    struct timeslot_struct *next_timeslot;
} *timeslot;

typedef struct timeslot_list_struct {
    timeslot head;
    int month;
    int day;
    int year;
    struct timeslot_list_struct *next_day;
} *date;

//The universal set in the context of this application
typedef struct scheduled_days_struct {
    date main_head;
} *scheduled_days;

//Forward declaration of functions
timeslot timeslot_create_timeslot(int, int, int, int, char*, size_t);
date date_create_date(int, int, int);
int date_append_timeslot_to_date(date, timeslot);
void print_schedule(date);
int date_delete_timeslot_from_date(int, int, date);
int timeslot_edit_timeslot_name(char*, char*, size_t, date);
void timeslot_destroy_timeslot(timeslot);
void date_destroy_date(date);
void date_print_date(date);

scheduled_days scheduled_days_create_scheduled_days(void);
void scheduled_days_destroy_scheduled_days(scheduled_days);
int scheduled_days_append_date_to_scheduled_days(scheduled_days, date);
date scheduled_days_peek_date(scheduled_days, int, int, int);
void scheduled_days_pop_date_from_scheduled_days(scheduled_days);
int scheduled_days_delete_day_from_scheduled_days(scheduled_days, int, int, int);
void scheduled_days_print_entire_list(scheduled_days);

//File operations for making the schedule planner console application.
scheduled_days scheduled_days_open_file(char*, size_t);
timeslot timeslot_parse_string_return_timeslot(char*, size_t);
date date_parse_string_return_date(char*, size_t);
int write_file(char*, size_t, scheduled_days);
void update_file(char*, size_t, int*, scheduled_days);
int hasTxtExtension(char*);






#endif