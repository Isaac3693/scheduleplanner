#ifndef ARGS 
#define ARGS

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "bufferlimits.h"

typedef struct argument_struct {
    int argc;
    char **argv;
} *args;

args create_arg(char *, size_t);
args c_arg(char *, size_t);
int check_arg_format(char *, size_t); //RETURNS 0 IF NO ISSUES
void destroy_args(args);


#endif