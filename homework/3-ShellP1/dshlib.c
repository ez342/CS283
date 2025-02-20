#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */

void printList(command_list_t *clist) {
    for (int i = 0; i < clist->num; i++) {
        printf("Command[%d]: exe = \"%s\", args = \"%s\"\n",
               i, clist->commands[i].exe, clist->commands[i].args);
    }
}

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}


int build_cmd_list(char *cmd_line, command_list_t *clist)
{
	char *cmd_saveptr; 
    char *sub_saveptr;
    
    cmd_line = trimwhitespace(cmd_line);
	char* cmd_token = strtok_r(cmd_line, PIPE_STRING, &cmd_saveptr);

	//loops through based on pipes
	while (cmd_token != NULL) {
		if (clist->num >= CMD_MAX) {
			return ERR_TOO_MANY_COMMANDS;
		}

		char *sub_token = strtok_r(cmd_token, " ", &sub_saveptr); 

		command_t *cmd = &clist->commands[clist->num]; 
		
		//initialize by setting all bytes to null bytes (0)
		memset(cmd->exe, 0, EXE_MAX); 
        memset(cmd->args, 0, ARG_MAX); 

		int first_arg = 1; //flag to keep track of exe

		//loop through based on space
		while(sub_token != NULL) {
			if (first_arg == 1) {
				// Check if exe name is too long
                if (strlen(sub_token) >= EXE_MAX) {
                    return ERR_CMD_OR_ARGS_TOO_BIG;
                }	
			
				strncpy(cmd->exe, sub_token, EXE_MAX - 1); 
				first_arg = 0;

				//exit when "exit" command found
				if (strcmp(clist->commands[clist->num].exe, EXIT_CMD) == 0) {
					exit(OK);
				}
			} else {				
				if (strlen(cmd->args) + strlen(sub_token) + 1 >= ARG_MAX) {
                    return ERR_CMD_OR_ARGS_TOO_BIG;
                }
                
                //space is added to nonempty args to separate args and appended to end
                if (strlen(cmd->args) > 0) {
                    strncat(cmd->args, " ", ARG_MAX - strlen(cmd->args) - 1);
                }
                strncat(cmd->args, sub_token, ARG_MAX - strlen(cmd->args) - 1);
			}
			sub_token = strtok_r(NULL, " ", &sub_saveptr);
		}
		clist->num++;
		cmd_token = strtok_r(NULL, PIPE_STRING, &cmd_saveptr);
	}
    return OK;
}
