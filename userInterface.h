#include "scheduleStructs.h"
#include "args.h"

enum date_comparison_state {BEFORE, SAME, AFTER};

int make_schedule(scheduled_days, int*);
int edit_schedule(scheduled_days, date);
int delete_schedule(scheduled_days, args);
int *parse_input_return_time(char*, size_t);
int *parse_input_return_date(char*, size_t);
void print_time(struct tm*);
int *make_date_arr(struct tm*);
enum date_comparison_state current_date_comparison_with_selected_date(int *, int*);
void print_help(void); //help menu on the main menu of the program (the part in which the user sees after they execute it with the appropriate arguments)
void print_edit_or_make_schedule_help(bool);
bool has_correct_date_format(int*);
bool has_correct_time_format(int*);
