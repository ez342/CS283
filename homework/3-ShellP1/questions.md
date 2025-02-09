1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  The function, fgets(), is a good choice to get input in the main while loop because it is "lines of input" based. What this means is that it reads line by line as it only stops after finding an EOF or new line. Then it continues to save what line it has read, into a buffer, which allows us to parse through the line and validate the commands.  

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**: I needed to allocate memory for 'cmd_buff' in dsh_cli.c because heap allocated memory remains in memory until it is freed. This allows us to continue to use what is saved in cmd_buff and pass it around through other functions such as build_cmd_list, until we are finished, which is after the while loop that gets input.  


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  This is necessary so that parsing does not run into issues. For example, someone may enter the "   cmd1" command, but when waiting for "cmd1" to be parsed, it may never be found because the command "   cmd1" may be invalid due to the spaces. 

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Using the symbol '>', you can redirect the output on the left side of the arrow to a file that will be on the right side of the arrow. This is known as output redirection. When using two like so '>>', you can append the output of what is on the left of the symbol, to the right of the symbol. Lastly, there is input redirection, which takes inputs from a file and then passes it. When implementing these, it will be important to parse each command correctly and their outputs, we will also have to be very careful when handling files and throwing the correct errors, and handling multiple redirections.  

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection is used to redirect the stdout, stdin, and stderr, which essentially means that it will need file descriptors, so it only works with files. However, pipe passes one command as input to another command, so it does directly work with files unlike redirection. 

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Although STDERR and STDOUT both output, we need to keep these separate because we would not want to output an error message into a file. If we did not separate STDERR and STDOUT, then files could contain both error messages and actually important data. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**: When handling errors from commands that fail, we should take into consideration of the exit codes as STDOUT and STDERR each have their respective numbers. When we find an error in the command, we should either output it to an error file, or throw an error to the user using the program so that they can readjust. However, if we were to provide a way to merge them, we can use 2>&1, where 2 refers to STDERR, so 2>&1 will send STDERR to the same location as 1 (STDOUT). 
