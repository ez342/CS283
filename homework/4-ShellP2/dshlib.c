#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

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

void print_cmd(cmd_buff_t cmd) {
	printf("command: %s\n", cmd._cmd_buffer);
        for(int i = 0; i < cmd.argc; i++){
            printf("arg[%d]: %s\n", i, cmd.argv[i]);
        }
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

int exec_local_cmd_loop()
{
    char *cmd_buff;
    int rc = 0;
    cmd_buff_t cmd;

	cmd_buff = malloc(sizeof(char)*SH_CMD_MAX);
	
    // TODO IMPLEMENT MAIN LOOP
	while(1){
		printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
        	printf("\n");
            break;
        }
        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
  
        //IMPLEMENT THE REST OF THE REQUIREMENTS
       	// TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

		char *trimmed = trimwhitespace(cmd_buff); 
		char *noDupSpace = eliminateDupSpace(trimmed);
		cmd._cmd_buffer = malloc(sizeof(char)*(strlen(cmd_buff)));

		strcpy(cmd._cmd_buffer, noDupSpace);

		char *sub_saveptr;
		char *sub_token = strtok_r(cmd_buff, " ", &sub_saveptr);

		cmd.argc = 0;
		int charCount = 0;
		while (sub_token != NULL) {
			if (cmd.argc > CMD_ARGV_MAX - 1) {
				printf("too much argument\n");
				break;
			}

			if (*sub_token == '\"') {
				//implement strtok while loop 
				int addCount = getQuote(&cmd._cmd_buffer[charCount], &cmd);	
				charCount += addCount;
				charCount++;
				cmd.argc++; 
				sub_token = strtok_r(&cmd._cmd_buffer[charCount], " ", &sub_saveptr);
				continue;
			}
			if (strlen(sub_token) > 0) {
                cmd.argv[cmd.argc] = malloc(sizeof(char)*(strlen(sub_token)));
                strcpy(cmd.argv[cmd.argc], sub_token);
                cmd.argc++;
                charCount += strlen(sub_token) + 1;
            }
			sub_token = strtok_r(NULL, " ", &sub_saveptr);
		}
		if (cmd.argc == 0) {
			printf(CMD_WARN_NO_CMD);
			return (WARN_NO_CMDS);
		}
		if (strcmp(cmd.argv[0], EXIT_CMD) == 0) { //check for exit
            exit(OK);
        }

		cmd.argv[cmd.argc] = 0;

        if (strcmp(cmd.argv[0], "cd") == 0) { //check for cd
        	if (cmd.argc == 2) {
    			chdir(cmd.argv[1]);
    		}
        } else {
			int f_result, c_result; 

			f_result = fork(); 
			if (f_result < 0) {
				perror("fork error\n");
			}
		
			if (f_result == 0) {
				//child will execute 
				rc = execvp(cmd.argv[0], cmd.argv);
				if (rc < 0) {
					perror("fork error in execvp\n");
					exit(42);
				}

			} else {
				//parent process 
				wait(&c_result);
			}

    	// TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    	// the cd command should chdir to the provided directory; if no directory is provided, do nothing

    	// TODO IMPLEMENT if not built-in command, fork/exec as an external command
    	// for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
    	}
    }
    return OK;
}
