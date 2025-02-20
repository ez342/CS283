1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  The reasons that we would use forl/execvp instead of just calling 'execvp' directly are because it allows for better process control, error handling, and multitasking. For example, fork() can create multiple processes that will handle their tasks in their own space. If an error were to occur, then it would not affect all the other spaces.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If the fork() system call fails, it would return a negative number meaning that the creation of a child process was not successful. Usually, you would print an error message using perror and then exit so that undefined behavior does not occur in the program. However, my implementation handled this with a perror message of "fork error."

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  The execvp() function finds the command to execute by searching through the directories that are in the PATH environment variable. There are two approaches, where the given command includes a "/" which means that execvp() will execute the path without searching in the PATH environment variable. However, if it does not include "/", then it will search in the PATh environment variable to find an executable file that matches the command that was provided through the parameters. 

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The purpose of calling wait() in the parent process after forking is because it is telling the parent process to wait for the child process to complete before executing the command in the parent process. Once the child process is complete and terminates, then it waits until the parent process gets to exit, in order to free everything. If we did not call wait(), then the child process will continue to stay in memory, and it may cause a build up of processes. In addition to that, the parent process also would not wait for the child process's termination signal, so the parent process may run even if the child process is not completed yet.  
    
5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  In the demo code, the WEXTSTATUS() is used to check the exit status of the child process. What this information provides is how the child process was terminated, and this is important because the return value will show us if the process was successful or not. An exit status of 0 would indicate a success, while non-zero numbers will mean that there was an error or something else went wrong during the processing. 

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  My implementation of exec_local_cmd_loop() handles quoted arguments by treating the white spaces inside the quotes as part of the argument, and then it saves everything inside the quotations as an argument. This is necessary because we need to ensure that the echo command works as intended. If we did not save the quoted argument with correctly, it would get rid of the spaces when using strtok_r with the delimiter being space. This means it would separate "hello world" as two different arguments, causing a behavioral error when echo is executed. 

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  In my current parsing logic, I only use strtok once to separate the command lines with spaces, unlike the past where I used strtok_r twice to separate by pipes and then spaces. However, I also updated my function for getting rid of duplicate spaces as I should be not be getting rid of duplicate spaces in quotes. Lastly, I took into account of how the the quoted argument should be saved while using strtok_r as I did not want strtok_r to get rid of the spaces within my quoted arguments, which was the most challenging part, as I did not handle this case in the last assignement. 
8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  The purpose of signals in a Linux system is to allow processes to interact with one another and to coordinate their actions based on the type of signals recieved. Signals differ from other forms of interprocess communication because they are asynchronous, unlike most other ICPs which are synchronous. What this means is that a signal can be sent at any time and the reciever can handle it at any time as well. Another difference is that signals only carry a signal number, unlike other ICPs which can transfer data. 

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  The signal, SIGKILL, cannot be handled, blocked, or ignored, and what it does is it forces a process to terminate immediately. Usually, you would only use SIGKILL as a last resort to terminate, such as when you have an unresponsive process. In addition to that, it also kills the child processes. On the other hand, the signal, SIGTERM, can be handled, ignored, and blocked, and it does not kill the child processes. What it does do, is it asks the process to terminate gracefully, allowing it to clean up. You would usually prefer to use SIGTERM rather than SIGKILL, to shut down servers or applications. Lastly, the signal, SIGINT, interrupts a process from the terminal when Ctrl+C is pressed, and this can be ignored. What it does is that it allows the users to stop a process from running by manually using the keyboard. Usually, you would use this in command-line applications so that you can exit gracefully allowing for clean up. 

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process recieves SIGSTOP, it is stopped imediately by the kernel and it cannot continue its execution until it recieves a SIGCONT signal, which tells it to continue. Unlike SIGINT, this process cannot be caught, ignored, or handled. This is because SIGSTOP was designed so that processes cannot control it, and what actually controls it is the OS, stopping all execution until specified.  
