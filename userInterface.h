#include "scheduleStructs.h"
#include "args.h"

int make_schedule(scheduled_days, int*);
int delete_schedule(scheduled_days, args);
int *parse_input_return_time(char*, size_t);
int *parse_input_return_date(char*, size_t);
void print_time(struct tm*);
int *make_date_arr(struct tm*);
