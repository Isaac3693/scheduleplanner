#include "args.h"

//Newer implementation of the argument constructor.
args c_arg(char *input, size_t len) {
    if (check_arg_format(input, len)) return NULL;

    args tmp;

    if ((tmp = (args)malloc(sizeof(struct argument_struct))) == NULL) {fprintf(stderr, "ARG MAKE FAIL\n"); return NULL;}

    //Create 2 integer indices in which they take the start and end of a quote. (THERE MUST BE AN EVEN AMOUNT OF QUOTOATION MARKS)

    int quote_counter = 0;
    int *indices_of_quote_start, *indices_of_quote_end;

    for (int i = 0; i < len; i++) if (input[i] == '\"') quote_counter++;

    if (quote_counter % 2 != 0) return NULL; //There are an inappropriate amount of quotation marks, returning NULL

    //If there are n quotation marks, then these pointers must accomodate (is that how I spell that) n/2 indices. n must be even
    int current_index = 0; //Used to assign the index of the pointers. And also used to assign the tmp->argv pointers
    indices_of_quote_start = (int*)malloc(sizeof(int) * (quote_counter/2)); indices_of_quote_end = (int*)malloc(sizeof(int) * (quote_counter/2));

    //Iterate through the loop and then find the indices of the quote start and end

    bool is_odd = true; //This is used to assign the correct index of the quote mark to the right pointer. Like a gate. (WHEN I MEAN ODD, I MEAN 0, 2, ETC.)

    for (int i = 0; i < len; i++) if (input[i] == '\"') {
        if (is_odd) {indices_of_quote_start[current_index] = i; is_odd = false;}
        else {indices_of_quote_end[current_index] = i; current_index++; is_odd = true;}
    }

    //There could be an instance in which the user has putted "()sample "" pp lmao", in which they have putted a quotation mark but no content in betweeen.
    //This also checks for any fuck ups in the code... if the quote end somehow goes before the quote start for some reason...
    for (int i = 0; i < quote_counter / 2; i++) if (indices_of_quote_end[i] - indices_of_quote_start[i] < 2) {
        //If I need to go back, then I need to free all the variable pointers that I have dynamically created.
        fprintf(stderr, "Error in parsing arg. No content between quotation marks\n");
        free(tmp);
        free(indices_of_quote_end); free(indices_of_quote_start);
        return NULL;
    }

    //Next, implement original arg implementation and check for the quotation marks.
    int whitespace_count = 0; bool dont_count_whitespaces = false;

    for (int i = 0; i < len; i++) {
        //If the iteration reaches the quote start, stop counting the whitespaces and keep continuing. If it is the quote end, continue back counting.
        for (int j = 0; j < quote_counter/2; j++) {
            if (i == indices_of_quote_start[j]) {dont_count_whitespaces = true; break;}
            else if (i == indices_of_quote_end[j]) {dont_count_whitespaces = false; break;}
        }

        if ((isspace(input[i]) && (!dont_count_whitespaces))) whitespace_count++;


    }

    //Now that we have the whitespace_count, we can make an integer pointer that takes into account of the indices.

    //Iterate through the array and check if the index hits the array and then create the words from the spot. 

    //amount of arguments in tmp->argc: (whitespace_count + 1) + (quote_counter/2). this block contains the memory allocation
    tmp->argc = whitespace_count + 1;
    tmp->argv = (char**)malloc(sizeof(char*) * tmp->argc);
    for (int i = 0; i < tmp->argc; i++) tmp->argv[i] = (char*)calloc(BUFFER_SIZE, sizeof(char));

    //Do we account for the indices of the quote start as the first letter of each "word" in this scneario? /Yes...
    int *index_of_first_letter_of_each_word = (int*)malloc(sizeof(int) * tmp->argc);
    index_of_first_letter_of_each_word[0] = 0; //Assuming that the user didn't any spaces in the first word.

    is_odd = false; //Is this line of code necessary?

    current_index = 1; //We have already assigned the first element of the index_of_first_letter_of_each_word to be 0.

    //I am thinking of a variable that stops counting the whitespace if it is in the quote... Oh wait, I have already declared it...
    dont_count_whitespaces = false;

    //This next iteration if going to record the indices of the first letter of each word. TODO: DO THIS SHIT..
    //I should probably check that the user has actually inputted any characters inside the quotes... because if not, then it will run into a segmentation fault.
    for (int i = 0; i < len; i++) {
        if (isspace(input[i]) && (input[i + 1] != '\"') && !dont_count_whitespaces) {index_of_first_letter_of_each_word[current_index] = (i+1); current_index++;}
        else if (input[i] == '\"' && !dont_count_whitespaces) {
            //The code block goes here because it has landed in the index of a quote start.
            index_of_first_letter_of_each_word[current_index] = i; //Going to have the first letter be a quotation mark.
            current_index++;
            dont_count_whitespaces = !dont_count_whitespaces;
        }
        else if (input[i] == '\"' && dont_count_whitespaces) {
            //Code goes here because it has landed in a quotation mark end.
            dont_count_whitespaces = !dont_count_whitespaces;
        }
    }

    //Now that we have located the indices of the first letter of each apparent "word" now we must strcpy all of that shit into the tmp->argv array one by one.
    current_index = 0;

    for (int i = (tmp->argc - 1); i >= 0; i--) {
        //This line of code is the reason that my computer fucking died lmao...
        bool already_parsed_string = false;
        for (int j = 0; j < quote_counter / 2; j++) if (index_of_first_letter_of_each_word[i] == indices_of_quote_start[j]) {strncpy(tmp->argv[i], (input + index_of_first_letter_of_each_word[i] + 1), (indices_of_quote_end[j] - indices_of_quote_start[j] - 1)); already_parsed_string = true; break;}
        if (!already_parsed_string) strcpy(tmp->argv[i], (input + index_of_first_letter_of_each_word[i]));
        if (i != 0) input[index_of_first_letter_of_each_word[i] - 1] = '\0';
    }

    //According to my previous implementation of this function, I thought that there is a space in the last word that the user has inputted... I guess I'll implement it too.
    for (int i = 0; i < strlen(tmp->argv[tmp->argc - 1]); i++) {
        if (isspace(tmp->argv[tmp->argc - 1][i])) tmp->argv[tmp->argc - 1][i] = '\0';
        break;
    }

    //FREE EVERYTHING THAT WE HAVE DECLARED IN THIS FUNCTION FFS.
    free(index_of_first_letter_of_each_word); free(indices_of_quote_start); free(indices_of_quote_end);

    //December 19th update: I have finished implementing this function, now I need to test it. 80 lines of code... nice.
    //December 20th update: This shit is beyond repair.
    //3 hours later... this shit finally works.
    return tmp;


}

//old implementation of the argument constructor that doesn't take any quotation mark sentences
args create_arg(char *input, size_t len) {

    //if create_arg throws an error, then return NULL and use it to signify changes to execution
    //whenever something wrong happens

    if (check_arg_format(input, len)) return NULL;
    args tmp = (args)malloc(sizeof(struct argument_struct));

    //create a dynamic int array that contains the indexes of the spaces
    //we have to take care of any whitespaces present
    int *index_of_first_letter_of_each_word;
    int whitespace_count = 0;

    for (int i = 0; i < strlen(input); i++) if (isspace(input[i])) whitespace_count++;

    tmp->argc = whitespace_count + 1;
    // there are whitespace_count + 1 words in an input
    index_of_first_letter_of_each_word = (int*)malloc(sizeof(int) * tmp->argc);

    //index_of_first_letter_of_each_word[0] should be 0, that's the first word
    index_of_first_letter_of_each_word[0] = 0;

    //index variable j that puts the elements in the specified order.
    int j = 1; 

    //Allocate (whitespace_count + 1) elements for argVariables
    tmp->argv = (char**)malloc(sizeof(char*) * tmp->argc);

    //Go through each char* in the list and allocate it to the max length of a string.
    //All of these words should be less than 32 characters long
    for (int i = 0; i < tmp->argc; i++) tmp->argv[i] = (char*)calloc(BUFFER_SIZE, sizeof(char));
    

    //Iterate through the string again and record the indices of whitespace in the array.
    /*i is the starting index for the string being read, j is the indexes of whitespace index*/
    for (int i = 0; i < len; i++) if (isspace(input[i])) {
        index_of_first_letter_of_each_word[j] = (i + 1);
        j++;
    }
    

    //Now assign each string element to the words
    //(taskList)this is a sample
    //"this is a sample" => ["this", "is", "a", "sample"]
    for (int i = (tmp->argc - 1); i >= 0; i--) {
        strcpy(tmp->argv[i], (input + index_of_first_letter_of_each_word[i]));
        if (i != 0) input[index_of_first_letter_of_each_word[i] - 1] = '\0';
    } 

    //last argv row most likely contains spaces, take them out
    //index through each letter and then if there is a space in the ahead index, set it to null terminating char

    for (int i = 0; i < strlen(tmp->argv[tmp->argc - 1]) - 1; i++) if (isspace(tmp->argv[tmp->argc - 1][i + 1])) {
        tmp->argv[tmp->argc - 1][i + 1] = '\0';
        break;
    }
    

    

    //Args is now set. free index_of_first_letter... you get my point.
    free(index_of_first_letter_of_each_word);

    return tmp;

}

void destroy_args(args tmp) {
    //free each index of the argVariables list, then free the argVariable double pointer, then free the struct pointer
    for (int i = 0; i < tmp->argc; i++) free(tmp->argv[i]);
    free(tmp->argv);
    free(tmp);
}

int check_arg_format(char *input, size_t len) {
    //Issue 1: first index is space 
    //Issue 2: user puts nothing
    //Issue 3: multiple spaces in between spaces
    //Issue of any space after the last word is eliminated by the newline character

    if (len == 0 || len > MAX_LENGTH) return 1; //User puts nothing or exceeds the maximum size
    if (isspace(input[0])) return 1; //Space is null
    if (isspace(input[len - 1])) input[len - 1] = '\0';
    for (int i = 0; i < len; i++) if (isspace(input[i]) && isspace(input[i + 1])) return 1; //multiple spaces between
    return 0; //Passes all tests
}
