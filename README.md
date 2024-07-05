# ShellCraft - A Linux BASH like Shell in C

## About

This is an implementation of an interactive user defined bash like shell in C language.

## Run the shell

- Execute the following in the directory Custom-C-shell that contains the source files:
   
  - make
  - ./a.out
   

##  Input Requirements Satisfied by the shell

- The shell supports a ‘;’ or ‘&’ separated list of commands. '; command can be used to give multiple commands at the same time. ‘&’ operator runs the command preceding it in the background after printing the process id of the newly created process.

- It also accounts for random spaces and tabs when taking input.
- If any command is erroneous 'Invalid Input !!' is printed.
---

## Supported commands

### System commands

- System commands such as sleep,vim,echo,gedit,emacs are very well executed upon giving input. They are not implemented seperately and execvp has been used to get the desired results.

### Foreground and Background Processes 

- A foreground process can be executed in the shell if '&' is not used after the command. The shell will wait for that process to complete and regain control afterwards. Control of terminal is handed over to this process for the time being while it is running.

```bash
<JohnDoe@SYS:~> sleep 5
# sleeps for 5 seconds
<JohnDoe@SYS:~ 
```

- Any command invoked with “&” is treated as a background command. The shell will spawn that process but doesn’t hand the control of terminal to it. Shell will keep taking other user commands.

- Whenever a new background process is started, PID of the newly created background process is printed.

- Multiple background processes can be run.

- Process name along with pid is printed when background process ends. If the process ended normally or abnormally is also mentioned.


```bash
<JohnDoe@SYS:~> sleep 10 &
13027
<JohnDoe@SYS:~> sleep 20 &                       # After 10 seconds
Sleep exited normally (13027)
13054
<JohnDoe@SYS:~> echo "Lorem Ipsum"               # After 20 seconds
Sleep exited normally (13054)
Lorem Ipsum

```
Background processes for custom commands are not handled.
### Custom Commands

### 1. warp command

- ‘warp’ command changes the directory that the shell is currently in. It also prints the full path of working directory after changing. The directory path/name can be provided as argument to this command.

- '~' changes directory to the home directory i.e. the directory from which the shell was executed
- '-' changes directory to the previous working directory.
- Any relative path from the home directory is valid.
- Any absolute path (i.e. from the root directory) is valid.
- Passing no arguments changes directory to the home directory.
- Note that these are valid as long as the user has the appropriate permissions for the directory passed.


```bash
<JohnDoe@SYS:~> warp test
/home/johndoe/test

<JohnDoe@SYS:~/test> warp assignment
/home/johndoe/test/assignment

<JohnDoe@SYS:~/test/assignment> warp ~
/home/johndoe

<JohnDoe@SYS:~> warp -
/home/johndoe/test/assignment

<JohnDoe@SYS:~/test/assignment> warp .. tutorial
/home/johndoe/test
/home/johndoe/test/tutorial

<JohnDoe@SYS:~/test/tutorial> warp ~/project
/home/johndoe/project

<JohnDoe@SYS:~/project> 

```

When a path command is given which contains travelling through multiple directories, the directory changes are done sequentially, hence if an invalid path is found in between, the current directory is changed till last valid directory input.

---

### 2. peek command

- ‘peek’ command lists all the files and directories in the specified directories in lexicographic order (default peek does not show hidden files). It supports the -a and -l flags as well.

- -l : displays extra information
- -a : displays all files, including hidden files

- Similar to warp, it supports “.”, “..”, “~”, and “-” symbols.

- It supports both relative and absolute paths.
- If no argument is given, it peeks at the current working directory.

The input is always be in the following format :

```bash
peek <flags> <path/name>
```
The following cases are also handled in case of flags :
```bash
peek -a <path/name>
peek -l <path/name>
peek -a -l <path/name>
peek -l -a <path/name>
peek -la <path/name>
peek -al <path/name>
```
Note that the user is required to have appropriate permissions to read the details/contents of the file/folder

It is assumed that the paths given are not space seperated.
it is assumed Multiple arguments will not be given as input.

---

### 3. pastevents commands

### pastevents
‘pastevents’ command is similar to the actual history command in bash. The implementation stores (and output) the 15 most recent command statements given as input to the shell based on some constraints. The oldest commands are overwritten if more than the set number (15) of commands are entered. pastevents is persistent over different shell runs, i.e., the most recent command statements should be stored when the shell is exited and be retrieved later on when the shell is opened.


Note :

- A command is not stored in pastevents if it is the exactly same as the previously entered command. 
- All statements except commands that include pastevents or pastevents purge are stored.

### pastevents purge

Clears all the pastevents currently. This command in not stored in the pastevents.

### pastevents execute <index>

Executes the command at position in pastevents (ordered from most recent to oldest). 

Note: If the new command entered is same as *previous* command, then it will not be stored.

```bash
<JohnDoe@SYS:~> peek test
# output
<JohnDoe@SYS:~> sleep 5
# output
<JohnDoe@SYS:~> sleep 5
# output
<JohnDoe@SYS:~/test> echo "Lorem ipsum"
# output
<JohnDoe@SYS:~> pastevents
peek test
sleep 5
echo "Lorem ipsum"
<JohnDoe@SYS:~> pastevents execute 1
# output of echo "Lorem ipsum"
<JohnDoe@SYS:~> pastevents
peek test
sleep 5
echo "Lorem ipsum"
<JohnDoe@SYS:~> pastevents execute 3
# output of peek test
<JohnDoe@SYS:~> pastevents
peek test
sleep 5
echo "Lorem ipsum"
peek test
<JohnDoe@SYS:~> pastevents purge
<JohnDoe@SYS:~> pastevents
<JohnDoe@SYS:~>
```

---



### 4. proclore command

- proclore is used to obtain information regarding a process. If an argument is missing, information of the shell is printed.

  Information printed :

  - pid
  - Process Status (R/R+/S/S+/Z)
  - Process group
  - Virtual Memory
  - Executable path of process

- Process states :

  - R/R+ : Running
  - S/S+ : Sleeping in an interruptible wait
  - Z : Zombie
  - The “+” signifies whether it is a foreground or background process ('+' if it is a foreground process).

```bash
<JohnDoe@SYS:~> proclore
pid : 210
process status : R+
Process Group : 210
Virtual memory : 167142
executable path : ~/a.out

```
---

### 5. seek command

- ‘seek’ command looks for a file/directory in the specified target directory (or current if no directory is specified). 
- It returns a list of relative paths (from target directory) of all matching files/directories (files in green and directories in blue) separated with a newline character. Note that by files, the text here refers to non-directory files.


- Flags :

  - -d : Only look for directories (ignores files even if name matches)
  - -f : Only look for files (ignore directories even if name matches)
  - -d and -f flag can’t be used at the same time.
  - If No flags are given, then it will look for both files and directory
  - -e : This flag is effective only when a single file or a single directory with the name is found. If only one file (and no directories) is found, then print it’s output. If only one directory (and no files) is found, then it changes current working directory to it. Otherwise, the flag has no effect. This flag works with -d and -f flags. If -e flag is enabled but the directory does not have access permission (execute) or file does not have read permission, then “Missing permissions for task!” is printed.

- Argument 1 : The target that the user is looking for. A name without whitespace characters must be given here.

- Argument 2 : The path to target directory where the search will be performed (this path can have symbols like . and ~ similar to the peek command). If this argument is missing, target directory is the current working directory. The target directory’s tree is searched (and not just the directory).

---

### 6. IO redirection

-I/O Redirection is when you the default input/output (which is the terminal) is changed to another file. This file can be used to read input into a program or to capture the output of a program. The shell supports >, <, » (< works with both > and »).

```bash
> : Outputs to the filename following “>”.
>> : Similar to “>” but appends instead of overwriting if the file already exists.
< : Reads input from the filename following “<”.
```
The output file is be created (with permissions 0644) if it does not already exist in both > and ».

In case the output file already exists, it is overwritten in case of > and appended to in case of ».

```bash
<JohnDoe@SYS:~> echo "Hello world" > newfile.txt
<JohnDoe@SYS:~> cat newfile.txt
Hello world
```

---

### 7. Pipes

Pipes are used to pass information between commands. It takes the output from command on left and passes it as standard input to the command on right. The shell supports any number of pipes.

```bash
<JohnDoe@SYS:~> echo "Lorem Ipsum" | wc
1 2 12      # extra spaces can be present

```
NOTE: Piping can be used alongside input and output redirections
```bash
<JohnDoe@SYS:~> cat a.txt
Lorem Ipsum
<JohnDoe@SYS:~> cat < a.txt | wc | cat > b.txt
<JohnDoe@SYS:~> cat b.txt
1212
```
---
### 8. Ping command

ping command is used to send signals to processes. The pid of a process and a signal number which corresponds the signal to be sent to the process is expected in input. 

- The pid can be any of the job numbers listed using the command jobs .
- The signal_number can be any of the signals listed under the manpage signal(7).

#### Keyboard Signals

Ctrl - C :Interrupt any currently running foreground process by sending it the SIGINT signal. It has no effect if no foreground process is currently running.

Ctrl - D: Log out of your shell (after killing all processes) while having no effect on the actual terminal.

Ctrl - Z: Push the (if any) running foreground process to the background and change it’s state from “Running” to “Stopped”. It has no effect on the shell if no foreground process is running.

---

### 9. activities - command

Prints a list of all the processes currently running that were spawned by the shell in lexicographic order. This list contains the following information about all the processes :

- Command Name
- pid
- state : running or stopped
```bash
<JohnDoe@SYS:~> activities
221 : emacs new.txt - Running
430 : vim - Stopped
620 : gedit - Stopped
```
---


### 11. fg and bg command

```fg <pid>```

Brings the running or stopped background process with corresponding pid to foreground, handing it the control of terminal.

- The pid can be any of the pids listed using the command activities

```bg <pid>```

Changes the state of a stopped background process to running (in the background).

---

### 13. Neonate command

Command : ```neonate -n [time_arg]```

The command prints the Process-ID of the most recently created process on the system and this pid will be printed every [time_arg] seconds until the key ‘x’ is pressed

--- 

### 14. iMan Command 

```iMan <command_name>```

<command_name> is the name of the man page that you want to fetch.

This fetches the man page for the given command from http://man.he.net/ and prints an error statement if the page does not exist.

## Additional feature

- The user can execute other files from within the shell (including the shell).

Author: [Shivam Mittal](https://github.com/mittalshivam2709)

Made under the valuable guidance of TA's and respected Professor: [Mr. Karthik Vaidhyanathan](https://karthikvaidhyanathan.com/).
