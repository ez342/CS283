#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
char *trimwhitespace(char *str)
{ 
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

char *eliminateDupSpace(char *str)
{
    int was_space = 0; //flag for checking if last character was space 
    int in_quotes = 0; //flag if within quotes  
    int i = 0;
    int j = 0;

    int len = strlen(str);
    char *saved_result = malloc(len + 1);

    //if there are no extra spaces, returm string as is 
    while (str[i] != '\0') {
        if (str[i] == '"') {
            in_quotes = !in_quotes; //swap quotations flag as needed 
            saved_result[j++] = str[i++];
            was_space = 0;
        } else if (str[i] == SPACE_CHAR) {
            if (in_quotes || !was_space) {
                saved_result[j++] = str[i];
            }
            was_space = 1;
            i++;
        } else {
            saved_result[j++] = str[i++];
            was_space = 0;
        }
    }
    saved_result[j] = '\0';
    return saved_result;
}

int getQuote(char* str, cmd_buff_t *cmd) {
    char *quote = malloc(sizeof(char)*strlen(str)+1);
    char *quoteStart = quote;
    int quoteLen = 0;

    str++;

    while (*str != '"') {
        *quote = *str;
        quote++;
        str++;
        quoteLen++;
    }
    *quote = '\0'; //null terminator

    cmd->argv[cmd->argc] = malloc(sizeof(char)*(strlen(quoteStart)));
    strcpy(cmd->argv[cmd->argc], quoteStart);
    return quoteLen + 2;
}

int parse_cmd(cmd_buff_t *cmd, char* cmd_token, int charCount) {
    char *sub_saveptr;
    cmd->argc = 0;
    char *sub_token = strtok_r(cmd_token, " ", &sub_saveptr);

    while (sub_token != NULL) {
        if (cmd->argc > CMD_ARGV_MAX - 1) {
            break;
        }
        if (*sub_token == '\"') {
                //implement strtok while loop 
                int addCount = getQuote(&cmd->_cmd_buffer[charCount], cmd);
                charCount += addCount;
                charCount++;
                cmd->argc++;
                sub_token = strtok_r(&cmd->_cmd_buffer[charCount], " ", &sub_saveptr);
                continue;
        }
        if (strlen(sub_token) > 0) {
            cmd->argv[cmd->argc] = malloc(sizeof(char)*(strlen(sub_token)));
            strcpy(cmd->argv[cmd->argc], sub_token);
            cmd->argc++;
            charCount += strlen(sub_token) + 1;
        }
        sub_token = strtok_r(NULL, " ", &sub_saveptr);
    }
    return 0;
}

int build_cmd_list(char *cmd_buff, command_list_t *clist)
{
    char *cmd_saveptr;
    int rc;
    int charCount = 0;

    char* cmd_token = strtok_r(cmd_buff, PIPE_STRING, &cmd_saveptr);

    //loops through based on pipes
    while (cmd_token != NULL) {
        if (clist->num >= CMD_MAX) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            return ERR_TOO_MANY_COMMANDS;
        }
        cmd_buff_t *cmd = &clist->commands[clist->num];

        char *trimmed = trimwhitespace(cmd_token);
        char *noDupSpace = eliminateDupSpace(trimmed);

        cmd->_cmd_buffer = malloc(sizeof(char)*(strlen(noDupSpace)));

        strcpy(cmd->_cmd_buffer, noDupSpace);

        rc = parse_cmd(cmd, cmd_token, charCount);
        if (rc != OK) {
            return rc;
        }

        clist->num++;
        cmd_token = strtok_r(NULL, PIPE_STRING, &cmd_saveptr);
    }
    return OK;
}

void print_cmd_list(command_list_t *clist) {
    printf("Total Commands: %d\n", clist->num);

    for (int i = 0; i < clist->num; i++) {
        cmd_buff_t *cmd = &clist->commands[i];

        printf("Command %d:\n", i + 1);
        printf("Raw Command: %s\n", cmd->_cmd_buffer);
        printf("Argument Count: %d\n", cmd->argc);

        for (int j = 0; j < cmd->argc; j++) {
            printf("Arg[%d]: %s\n", j, cmd->argv[j]);
        }
        printf("\n");
    }
}

int exec_one_cmd(command_list_t *clist) {
    int rc = 0;

    cmd_buff_t *cmd = &clist->commands[0];
    if (strcmp(cmd->argv[0], EXIT_CMD) == 0) {
        exit(OK);
    }
    //check for cd 
    cmd->argv[cmd->argc] = 0;

    if (strcmp(cmd->argv[0], "cd") == 0) {
        chdir(cmd->argv[1]);
    } else {
        int f_result, c_result;

        f_result = fork();
        if (f_result < 0) {
            perror("fork error\n");
        }

        if (f_result == 0) {
            //child will execute 
            rc = execvp(cmd->argv[0], cmd->argv);
            if (rc < 0) {
                perror("fork error in execvp\n");
                exit(42);
            }

        } else {
            //parent process 
            wait(&c_result);
        }
    }
    return OK;
}

int execute_pipeline(command_list_t *clist) {
    int pipes[clist->num - 1][2]; //array of pipes
    pid_t pids[clist->num]; //array to stire process IDs 
    //create all necessary pipes 
    for (int i = 0; i < clist->num - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    //create processes for each command 
    for (int i = 0; i < clist->num; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pids[i] == 0) { //child process
            cmd_buff_t *cmd = &clist->commands[i];
            //set up input pipe for all except first process 
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            //set up output pipes for all except last process 
            if (i < clist->num - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            //close all pipe ends in child
            for (int j = 0; j < clist->num - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            //execute command
            execvp(cmd->argv[0], cmd->argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    //parent process: close all pipe ends 
    for (int i = 0; i < clist->num - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    //wait for all children 
    for (int i = 0; i < clist->num; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return OK;
}

int free_cmd_list(command_list_t *cmd_lst) {
    if (cmd_lst == NULL) {
        return OK;
    }

    for (int i = 0; i < cmd_lst->num; i++) {
        if (cmd_lst->commands[i]._cmd_buffer) {
            free(cmd_lst->commands[i]._cmd_buffer);
            cmd_lst->commands[i]._cmd_buffer = NULL;
        }
    }
    return OK;
}

int exec_local_cmd_loop()
{

    // THIS CODE SHOULD BE THE SAME AS PRIOR ASSIGNMENTS
   	char *cmd_buff = malloc(SH_CMD_MAX);
    command_list_t *clist = malloc(sizeof(command_list_t));
    int rc;

    //while loop to read input from stdin 
    while(1){
        printf("%s", SH_PROMPT);

        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }
        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';

        //IMPLEMENT THE REST OF THE REQUIREMENTS
        clist->num = 0;
        rc = build_cmd_list(cmd_buff, clist);

        if (rc == OK) {
            if (clist->num == 0) {
                printf(CMD_WARN_NO_CMD);
                return (WARN_NO_CMDS);
            } else if (clist->num == 1) {
                rc = exec_one_cmd(clist);
                if (rc != OK) {
                    return rc;
                }
            }
            else {
                rc = execute_pipeline(clist);
                if (rc != OK) {
                    return rc;
                }
            }
        } else {
            return rc;
        }
    }
    rc = free_cmd_list(clist);
    return OK;
}
