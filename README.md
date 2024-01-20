# ShellCraft - A Linux BASH like Shell in C

## About

This is an implementation of a Linux Bash like shell in C language.

## Run the shell

1. Execute the following in the directory that contains the source files:
   
   $ make
   $ ./a.out
   

## Display

### ; seperated commands

- Concatenate multiple commands in a single line by separating them with a semicolon ;

---

### & terminated background processes

- Run processes in background by appending an ampersand & at the end of the command
- *Terminal* displays the pid of such processes before executing them in the background
- *Child handling: **Terminal* displays the exit status of such processes when they are terminated, suspended or continued.

---

- On executing the code, a shell prompt of the following form appears along with it.
  <Username@system_name:curr_dir>
  where curr_dir from which the shell is being invoked is represented by a '~'.

## Source Files Organisation

- Modular
- The commands are implemented in separate .c files.
- The shell is called from the main function in [main.c](main.c).
- Common header files are included, functions and global variables are declared in the [headers.h](headers.h) file.

## Supported commands

### Built-in commands

- Builtin commands are contained within the shell itself. When the name of a builtin command is used as the
  first word of a simple command, the shell executes the command directly without invoking another program.
- Builtin commands are necessary to implement functionality impossible or inconvenient to obtain with separate utilities.
- Built-in commands have been defined and implemented.

### 1. warp- Changes the current working directory


$ warp [DIR]
$ warp ..
$ warp -
$ warp ~


- Here, [DIR] can take on any of the following values
  - ~ Changes directory to the home directory i.e. the directory from which the shell was executed
  - - Changes directory to the previous working directory
  - Any relative path from the home directory is valid
  - Any absolute path (i.e. from the root directory) is valid
  - Passing no arguments changes directory to the home directory
  - Note that these are valid as long as the user has the appropriate permissions for the directory passed

---

### 2. peek - List files/folders and their information from one or multiple directories


$ peek [OPTIONS]... [FILE]...`


- Here, [OPTIONS] can take any of the following values
  - Specifying no options prints only the names of the non-hidden files/folders from the FILEs passed
  - -a (all) Behaves similar to no options but additionally prints hidden files
  - -l (long format) Prints additional info such as permissions, date modified, size for each non-hidden file
  - Note that multiple options can be used at the same time
- The [FILE] argument can be
  - A relative or absolute path to a file or a folder
  - Note that the user is required to have appropriate permissions to read the details/contents of the file/folder

---

### 3. pastevents - Displays the previous 15 commands executed on the Terminal


$ pastevents
$ pastevents execute [0<=index<=15]
$ pastevents purge


- Here, specifying only pastevents, will print previous 15 commands and are stored in *past.txt*
- If the new command entered is same as *previous* command, then it will not be stored in past.txt
- *pastevents purge* clears past.txt file
- *pastevents execute* index will Execute the command at position in pastevents

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
