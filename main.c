#include "scheduleStructs.h"
#include "userInterface.h"

/*

Now that I am actually thinking about it, using the keyword tomorrow would mean that I have to do more than just increment the day by one (in case it is the 31st and the user wants to schedule the 1st of whatever month.)

I am not implementing the today keyword. I leave this to the reader as an exercise.

CHECK TO SEE IF THE DATES ARE ALEADY SCHEDULED TO EDIT THEM IN THE TWO FUNCTIONS I HAVE MADE
schedule today
schedule <date>

delete today
delete <date>

print today
print tomorrow
print

*/




int main(int argc, char *argv[]) {
    //We want to check if the user has inputted the proper text format of the file.

    if (argc < 2) printf("Incorrect amount of parameters. Include the path of the file\n");

    else if (!hasTxtExtension(argv[1])) printf("Incorrect file format\n");

    else {
        //MAIN EXECUTION OF THE PROGRAM
        scheduled_days user_list;
        args user_arguments;
        char user_input[BUFFER_SIZE];

        //Declaration & initialization of the time variables
        struct tm *timestruct;
        time_t t;
        int *current_date_array;
        int *user_inputted_date_array;
        bool has_updated = false; //I need to keep the current_date array be continuously update in case for some reason I find myself making a schedule sometime in the midnight 
        //TODO: Implement the updateFile() function in the main program


        if ((user_list = scheduled_days_open_file(argv[1], strlen(argv[1]))) == NULL) {fprintf(stderr, "ERROR: USER LIST RETURNS NULL\n"); exit(1);}
        printf("type 'help' for a comprehensive list of instructions\n");


        while (1) {

            t = time(NULL);
            timestruct = localtime(&t);
            current_date_array = make_date_arr(timestruct);
            if (!has_updated) update_file(argv[1], strlen(argv[1]), current_date_array, user_list, &has_updated);

            printf("(schedule planner)");
            fgets(user_input, BUFFER_SIZE, stdin);
            user_input[strlen(user_input) - 1] = '\0';

            if ((user_arguments = c_arg(user_input, strlen(user_input))) == NULL) {printf("Incorrect arg format\n"); continue;}

            //No issue with the argument, now we have to check the principal argument

            //USER OPTIONS (in any case that the user puts nothing, they previous conditional checks that due to check_arg_format() function in c_arg)
            //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


            //HELP COMMAND
            if (!strcmp(user_arguments->argv[0], "help")) print_help();

            //SCHEDULE COMMAND
            else if (!strcmp(user_arguments->argv[0], "schedule")) {
                //check if they have put the date or the 'today' special keyword in the argument. Prior to checking those values, make sure that argc equals 2
                if (argc != 2) printf("Incorrect amount of arguments for 'schedule' command\n");

                else if ((user_inputted_date_array = parse_input_return_date(user_arguments->argv[1], strlen(user_arguments->argv[1]))) == NULL) printf("Incorrect format for the date. Must be in month/day/year\n");
                else {
                    //Normal execution of the make/edit schedule function. we will not allow for the user to put any day before today, Only today and 
                    //Normally, I would implement a switch checking the value of the enum but I feel like it would be more confusing to follow, so instead I implement another if else loop.
                    //ISSUE FIX: PUTTING AN EQUALS SIGN MEANT THAT ALL DAYS THAT CAME AFTER TODAY WOULD NOT HAVE BEEN ABLE TO BE CREATED
                    enum date_comparison_state compare_today_with_user_inputted_date = current_date_comparison_with_selected_date(current_date_array, user_inputted_date_array);

                    if (compare_today_with_user_inputted_date == AFTER) printf("Date has already passed, enter another date\n");

                    else {
                        //use the peek date to retrieve the date of a list.
                        date selected_date;
                        //if there is no date that is already in the list, then make a new date
                        if ((selected_date = scheduled_days_peek_date(user_list, user_inputted_date_array[1], user_inputted_date_array[0], user_inputted_date_array[2])) == NULL) {make_schedule(user_list, user_inputted_date_array);}
                        else edit_schedule(user_list, selected_date);

                        write_file(argv[1], strlen(argv[1]), user_list);

                    }

                    free(user_inputted_date_array);

                    
                }

                
            }

            //DELETE COMMAND
            else if (!strcmp(user_arguments->argv[0], "delete")) {
                
                if (user_arguments->argc != 2) printf("Incorrect amount of arguments for 'delete' command\n");

                else if ((user_inputted_date_array = parse_input_return_date(user_arguments->argv[1], strlen(user_arguments->argv[1]))) == NULL) printf("Incorrect format for the date. Must be in month/day/year");

                else {scheduled_days_delete_day_from_scheduled_days(user_list, user_inputted_date_array[1], user_inputted_date_array[0], user_inputted_date_array[2]); free(user_inputted_date_array); write_file(argv[1], strlen(argv[1]), user_list);}
            }

            //PRINT COMMAND
            else if (!strcmp(user_arguments->argv[0], "print")) {
                //if the user just puts "print" without any other additional arguments, just print today
                date selected_date;
                if (user_arguments->argc == 1) {
                    if ((selected_date = scheduled_days_peek_date(user_list, current_date_array[1], current_date_array[0], current_date_array[2])) == NULL) printf("No schedule for today\n");
                    else date_print_date(selected_date);
                } else if (user_arguments->argc == 2) {
                    if (!strcmp(user_arguments->argv[1], "all")) scheduled_days_print_entire_list(user_list); //Print the entire list
                    else if ((user_inputted_date_array = parse_input_return_date(user_arguments->argv[1], strlen(user_arguments->argv[1]))) == NULL) printf("Incorrect format for the date. Must be in month/day/year");
                    else {
                        if ((selected_date = scheduled_days_peek_date(user_list, user_inputted_date_array[1], user_inputted_date_array[0], user_inputted_date_array[2])) == NULL) printf("No schedule for selected date\n");
                        else {date_print_date(selected_date); free(user_inputted_date_array);}
                    }
                } else printf("Incorrect amount of arguments for 'print' command\n");
            }

            //User wants to clear the screen
            else if (!strcmp(user_arguments->argv[0], "clear")) system("clear"); //I did not know that I could call commands from the bash lmao

            else if (!strcmp(user_arguments->argv[0], "quit")) {printf("Exiting program\n"); exit(0);}

            else printf("Unknown command\n");



            //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            

            //After the code goes through all of the if else chain, free the date array and the args 
            destroy_args(user_arguments); free(current_date_array);
        }


    }

    return 0;
}


