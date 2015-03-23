# UNIX Shell

Compile the code using the makefile. Type **make** on the command line. It will generate two executables. Run **ronakShell** to start the shell.
**TESTING.md** contains some of the text cases and formats for various features.

This implementation contains the following features-

####1. Creating a Child Process 
* The parent process normally waits for the child process to terminate, unless otherwise specified using '&' at the end of the command 
* If the command is followed by '&' the parent process executes concurrently
	
####2. Creating a History Feature
* 'history' command shows 10 most recent commands in the order of most to least recent
* '!!' command executes the most recent commands
* Program handles errors for '!!'. If there are no commands in the history entering '!!' results in a message “No
 commands in history”
* '!N' command executes the Nth command in the history 
-Program handles errors for '!N'. If there is no command corresponding to the number
 entered with the single '!', the program results in a message "No such command in
 history."

####3. Alias
* Format: alias aliasName “CommandName”. E.g., alias dir “ls -al”
* Program handles error if the alias is not defined in the specified format. An error results in the following message- 
“No such command. Format for alias is: alias aliasName “commandName””

####4. Verbose
* Program handles the error if specified format is not followed. An error results in a message “No such command. Only arguments allowed with verbose are on and off”.

* Prorgram handles error if any arguments other than on or off are passed along with set verbose. The program results in a message “

####5. Initialization Script .cs543rc
* Commands specified in the .cs543rc are implemented as we run the program
* Initialization commands are displayed on the console

####6. Saving shell session in 'myshell.log'
* Doesn't print anything on console while saving to file
* 'script myshell.log' command initiates recording the shell session, 'endscript' ends recording

####7. Set path
* Accepts path address
* Searches for executable in the paths given and executes
* Format: set path (. /bin). Works with this format
* Program handels errors if the set path command is not in the correct format. An error results in the following message- "No such command. Format for set path is: set path = (paths separated by spaces)"
* Program handles conditions if the command entered by the user is not in the specified path. An error results in the following message- "No such command in the specified path."

####8. exit
* Exits the shell on the command exit
