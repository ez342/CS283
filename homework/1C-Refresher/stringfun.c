#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *);
int  print_words(char *);
void reverse_string(char *, int );
//add additional prototypes here

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    //internal buffer buff as the first argument
    //the user supplied string from the command line as the second argument
    //size of buff as the third argument (aka 50 char) 

    char *buff_ptr = buff; 
    const char *user_str_ptr = user_str; 
    int was_space = 0; //used to check for consecutive spaces 
    int user_str_len = 0;     

    while (*user_str_ptr == ' ' || *user_str_ptr == '\t' || *user_str_ptr == '\n') {
        user_str_ptr++; // Skip leading spaces
    }
    
    while (*user_str_ptr != '\0') {
        if (*user_str_ptr == ' ' || *user_str_ptr == '\t' || *user_str_ptr == '\n') {
            if (was_space == 0) {
                *buff_ptr = ' '; 
                was_space = 1; //set flag to 1 (last was space) 
                buff_ptr++; //increment pointer to next char to allow for next copy 
                user_str_len++;
            }
        } else {
            *buff_ptr = *user_str_ptr; //copy non-white space char into buff_ptr 
            was_space = 0; //set flag to 0 (last is no longer space) 
            buff_ptr++; 
            user_str_len++; 
        }
    
        if (user_str_len > len) {
            return -1;
        }
        //printf("%c", *user_str_ptr);
        user_str_ptr++;
    }
    
    if (*(buff_ptr - 1) == ' ') {
        buff_ptr--; 
    }
        
    int buff_len = user_str_len; //buffer length of user input
    while (user_str_len <= len) {
        *buff_ptr = '.'; 
        buff_ptr++; 
        user_str_len++; 
    }
    /*
    buff_ptr++;
    *buff_ptr = '.';   
    */

    //print_buff(buff, 50);
    return buff_len; 
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    printf("[");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]");
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff){
    //YOU MUST IMPLEMENT
    int at_start = 0; //indicate if at start of a word  
    char *buff_ptr = buff;
    int word_count = 0; //wc initially starts at 0
     
    while (*buff_ptr != '.') {
        // Skip over whitespaces if we're not at the start of a word
        if (*buff_ptr == ' ' || *buff_ptr == '\t' || *buff_ptr == '\n') {
            at_start = 0;  // We are not at the start of a word anymore
        } else {
            // We're at the start of a word and found a non-whitespace character
            if (at_start == 0) {
                word_count++;  // Increment the word count
                at_start = 1;  // Now we're in a word
            }
        }
        buff_ptr++;  // Move to the next character
    }
    return word_count;
}

void reverse_string(char *buff, int user_str_len ) {
    int start_idx = 0;
    int end_idx = user_str_len;
    char temp_char; 
    char *sbuff_ptr = buff; 
    char *ebuff_ptr = buff; 
    
    //get end pointer to the end 
    while (*ebuff_ptr != '.') {
        ebuff_ptr++;
    }
    ebuff_ptr--;  

    while (end_idx > start_idx) {
        //swapping using temp char holder 
        temp_char = *sbuff_ptr;
        *sbuff_ptr = *ebuff_ptr; 
        *ebuff_ptr = temp_char; 

        sbuff_ptr++; 
        ebuff_ptr--; 
        start_idx++; 
        end_idx--;  
    }    
}

int print_words(char *buff) {
    int word_count = 0; 
    int char_ctr = 0; 
    int at_start = 0; 
    char *buff_ptr = buff;

    while (*buff_ptr != '.') {
        if (*buff_ptr == ' ' || *buff_ptr == '\t' || *buff_ptr == '\n') {
            at_start = 0;
            printf("(%d)\n", char_ctr); 
        } else {  
            if (at_start == 0) {
                word_count++;
                at_start = 1;
                char_ctr = 1;
                printf("%d. ", word_count); 
            } else {
            char_ctr++; 
            }
        printf("%c", *buff_ptr);    
        }
    buff_ptr++;
    }
    
    if (at_start == 1) {
        printf("(%d)\n", char_ctr); // Print length of the last word
    }
    
    return word_count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //       #1A. This is safe because the first portion of the if statement checks that there will at least be two arguments, which would be the program name (argv[0]) and the operation (argv[1]). If there are less than two arguments, then it will exit because the program would not be able to run smoothly. Then the second part of the if statement ensures that an operation is provided as all the operations begin with '-'. If argv[1] did not exist, it would be caught by the argc < 2 portion as there would only be 1 argument which would still result in an exit due to error. 
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //       #2A. Since we ensured above that at least two arguments were provided and that the operation began with '-' we are now checking to make sure that a third argument, or the user-provided string, was given as well. If the argument count is less than 3 then it means a string was not provided, so the program would exit.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = malloc(BUFFER_SZ*sizeof(char));
    if (buff == NULL) {
        printf("Error allocating memory.\n"); 
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options

        case 'r': 
            reverse_string(buff, user_str_len); 
        
            break; 
        case 'w': 
            rc = count_words(buff);
            printf("Word Print\n----------\n");        
            print_words(buff);        
            printf("\nNumber of words returned: %d\n", rc);

            break;
        case 'x': 
            if (argc < 4) {
                printf("Not Implemented!");
                return -2; 
            }
            
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          Passing both the pointer and the length into the helper functions allow us to give more clarity, especially when testing for errors. Not only that, but since we are using pointers to navigate, it would be useful to have the length of both to use as end pointers if necessary. 
