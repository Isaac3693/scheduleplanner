#include "scheduleStructs.h"
#include "args.h"

//for using strncpy, make sure that

//memory allocated strings "char string[32] already have a null terminating character at the end"

int *suck(void);


int main(void) {

    int *other_suck = suck();

    printf("suck is %d %d %d\n", other_suck[0], other_suck[1], other_suck[2]);

    free(other_suck);
    return 0; //Like this is ever going to happen on the first try.
}

int *suck(void) {
    int penis[3] = {1, 2, 3};
    return penis;
}