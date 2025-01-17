#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int i = 0; //index for user_str 
    int j = 0; //index for buff 
    int was_space = 0; //used to check if the last character was a space     
    
    while (user_str[i] != '\0') {
        
        //check if the user_str is too long 
        if (i >= len) {
            return -1;
        }

        //check if character is not white spaces 
        if (user_str[i] != ' ' && user_str[i] != '\t') {
            buff[j++] = user_str[i];
            was_space = 0;   
        } else {
            if (!was_space) {
                buff[j++] = ' ';
                was_space = 1; //set to true since just put space into buffer 
            }
        }
        i++; //move to next character in user_str
    }
        
    while (j < len) {
        buff[j++] = '.'; //fill with '.'
    }    

    return j; //return length of buffer 
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    int word_count = 0; 
    int at_start = 1; //starting at true for start of word 
    int i = 0; //index for buffer 

    if (str_len > len) {
        printf("Error: String length did not match buffer length.\n");
        return 3;
    }
    if (str_len == 0) {
        return 0; //buff is empty so no words 
    }
    
    //loop until reach padding 
    while (buff[i] != '.') {
        //make sure you are at start and that start does not start with ' '
        if (at_start == 1 && buff[i] != ' ') {
            word_count++;  //add word_count since at start 
            at_start = 0; //reset at_start flag 
        }
        //check for space, if found space then that means new word 
        if (buff[i] == ' ') {
            at_start = 1;
        }
        i++;  
    }
    return word_count;
}

int print_words(char *buff, int buff_len, int str_len) {
    int word_count = 0; 
    int char_ctr = 0; //len of current word 
    int at_start = 1; //flag for start of word 
    int i = 0; //buff index 

    if (str_len > buff_len) {
        printf("Error: String length is longer than buffer length.\n");
        return 3;
    }
    if (str_len == 0) {
       return 0;
    }
    //print heading 
    printf("Word Print\n");
    printf("----------\n");

    while (buff[i] != '.') {
        if (at_start == 1) {
            word_count++; 
            at_start = 0; //set to false
            printf ("%d. ", word_count); 
        } if (buff[i] == ' ') {
            printf(" (%d)", char_ctr);
            char_ctr = 0; //reset character counter 
            at_start = 1; //reset new word 
        } else {
            printf("%c", buff[i]);
            char_ctr++; 
        }
        i++; 
    }
    printf(" (%d)", char_ctr); //prints character coutn of last word 
    return 0; 
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
    buff = malloc(BUFFER_SZ); //allocating space 
    
    //check if malloc failed 
    if (buff == NULL) { 
        printf("Memory allocation failed.\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
                        
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
