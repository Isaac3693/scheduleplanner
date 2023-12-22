#ifndef BUFFER_LIMITS
#define BUFFER_LIMITS

/*
    {s, a, m, p, l, e, :, 2, 0, \0}
    128 characters in total, 2 are dedicated to the \0 char and the : char
    63 characters dedicated to the name of the task, add 1 for \0
    10 characters to represent max for UINT max, add 1 for \0
    So in total: 73 characters to read the name and the hours, add 2 for \0 and
        : char and that would be 75
    
    Make the buffer reader twice the length of the 'max' limits i've imposed to check
        if the string length of that exceeds MAX_FILE_LINE_NAME
*/

//when defining pre-allocated space strings "char string[20]", 
//do "char string[MAX_TASK_NAME_STRLEN + 1]"
//an entire user input should be less than 200 characters for a cli command

#define BUFFER_SIZE 500
#define MAX_LENGTH 250
#define MAX_NAME_LENGTH 32 //In reality, the actual strlen of the char* is actually 31 characters, not counting the null terminating character.


#endif