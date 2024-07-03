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



### 4. proclore - Prints the process related info of a shell program


$ proclore [pid]


- Displays the pid, process status, memory consumption and executable path for the specified process
- Passing a valid pid displays the info related to the specified pid
- If no arguments are passed, instead displays the info related to the current *Terminal* process

---

### 5. seek - Looks for files and directory in the specified target directory


$ seek <flags> <search> <target_directory>


- Flags :

  - -d : Only look for directories (ignore files even if name matches)
  - -f : Only look for files (ignore directories even if name matches)
  - -d and -f flag can’t be used at the same time.
  - If No flags are given, then it will look for both files and directory
  - -e : This flag is effective only when a single file or a single directory with the name is found. If only one file (and no directories) is found, then print it’s output. If only one directory (and no files) is found, then change current working directory to it

- search :

  - Write the file or directory that needs to be searched inside the <target_directory> tree

- target_directory :

  - Write the folder in whose the file or folder is to be printed

- “No match found!” will be printed in case no matching files/directories is found.

- It returns a list of relative paths (from target directory) of all matching files/directories *(files in green and directories in blue)* separated with a newline character.

---

### 6. File redirection

- < [FILE] can be used to redirect input from the specified [FILE] for a command
- > [FILE] and >> [FILE] can be used to redirect the output to the specified [FILE] for a command. > Overwrites the output file and >> appends to the output file
- Both input and output redirections can be used on a single command
  - Example:
  
  $ cat < input.txt >> output.txt
  
  - Example:
  
  $ wc < input.txt > output.txt
  

---

### 7. Piping commands


$ <command> | <command>


- <command> | <command> can be used to pipe the output of the command on the left side of the pipe to the command on the right side of the pipe
- Two or more commands can be chained together by piping
- Piping can be used alongside input and output redirections
  - Example: cat < in.txt | wc -l > lines.txt
- Note that in case of a conflict between taking input/output from the pipe or through a redirection, the redirection is given higher preference
  - Example: cat in.txt | sort > out.txt | wc -l
  - This shall output 0 since the sort command redirects the output to out.txt and not the pipe

### 8. Signals

- <ctrl>C or SIGINT does not kill *Terminal*
- <ctrl>Z or SIGTSTP does not kill *Terminal*
- <ctrl>D or an EOF safely logs out of the shell (functionally equivalent to the exit command)

---

### 9. Activities - Displays the processes spawned by the shell which are either running or suspended


$ activities


- *activities* print a list of all the processes currently running that were spawned by shell in *lexicographic* order.
- This list will contain the following information about all processes :

  - Command Name
  - pid
  - state : running, stopped and finished

- If a process is *finished*, then it will be printed only once.

---

### 10. Ping - Sends a specified signal to the process with the specified job number


$ ping [pid] [signal_number]


- The pid can be any of the job numbers listed using the command jobs .
- The signal_number can be any of the signals listed under the manpage signal(7).

---

### 11. fg - Brings the running or stopped process specified by the pid to the foreground


$ fg [pid]


- The pid can be any of the pids listed using the command activities

### 12. bg - Sends the running or stopped process specified by the pid to the background


$ bg [pid]


- The job_number can be any of the pids listed using the command activities

---

### 13. Neonate - Prints a specific system data periodically for a specified period


$ neonate [OPTION] N


- Here, the [OPTION] must include exactly one instance of
  - -n followed by a positive integer [N > 0] which specifies the interval for printing the command output
  - This prints the PID of the most recently created process on the system after every N seconds
- A keypress of x stops execution of the command

### 14. iMan : fetches man pages from the internet using sockets and outputs it to the terminal (stdout) using the website http://man.he.net/ to get the man pages.


$ iMan <command_name>


- <command_name> is the name of the man page that you want to fetch.

- This will fetch the man page (atleast the name, synopsis and description of the command) for the given command from http://man.he.net/ and will Print an error statement if the page does not exist.

### 15. finish : Last but not the least, this will lead to the Exit, out of the shell.


$ finish


- It will display a messese "Oh! You are out of your *ShellCraft* now!" in red colour.

# Assumptions


1. Assuming the input size will be maximum of *1000* bytes.
2. In the pastevents command, if the command contains pastevents, then it wont be stored in history.txt, or else the corresponding command will be checked with the last command and if both of them are not same, then it will be stored in *history.txt* to ensure that 'unique consecutive commands' are only appended.
3. All paths involved are less than *1000 characters*.
4. Each command is less than *1000 characters*.
5. History is stored in txt file in the same folder as where the executable is stored.
6. In IO_redirection , I am appending/ redirecting the output to the mentioned text-file with a "\n" in between the lines for clear presentation.
7. history.txt mush be present in the folder while any command is given to store the pastevents.
8. I am assuming that the pastevents execute command does not give argument greater than 9
9. I am assuming that that the oldest argument is not being given as input when history is of length less than 15
10. A history.txt file has already been created.
11. For specification 6 the proceses run in foreground and background as required but the status of background process is not being printed after the next input is entered.
12. Some of the part of seek command is left which will be completed by the end of project.
13. There might be some printf statements between the code which by mistake might  not be commented.However
it doesen't affect the flow of code.
14. It might be possible that few commands or flags might not work but i will ensure that it would work by
the end of the project.

---

- Owner: Shivam Mittal
  (IIIT-HYDERABAD)
- Made under the valuable guidance of TA's and respected Professor: Mr. Karthik Vaidhyanathan.
