I have implemented the following:

-From the book
1. Creating a Child Process 
-Works as described in the book
-The parent process normally waits for the child process to terminate, unless otherwise specified using '&' at the end of the command 
-If the command is followed by '&' the parent process executes concurrently
	
2. Creating a History Feature
-Works as described in the book
-'history' command shows 10 most recent commands in the order of most to least recent

-'!!' command executes the most recent commands
-Program handles errors for '!!'. If there are no commands in the history entering '!!' results in a message “No
 commands in history”

-'!N' command executes the Nth command in the history 
-Program handles errors for '!N'. If there is no command corresponding to the number
 entered with the single '!', the program results in a message "No such command in
 history."


-Additional functionality

3. Alias
-Works with the format specified in the homework
-Format: alias aliasName “CommandName”. E.g., alias dir “ls -al”

-Program handles error if the alias is not defined in the specified format. An error results in the following message- 
“No such command. Format for alias is: alias aliasName “commandName””



4. Verbose
-Works with the format specified in the homework

-Program handles the error if specified format is not followed. An error results in a message “No such command. Only arguments allowed with verbose are on and off”.

-Prorgram handles error if any arguments other than on or off are passed along with set verbose. The program results in a message “



5. Initialization Script .cs543rc
-Commands specified in the .cs543rc are implemented as we run the program

-Initialization commands are displayed on the console


6. Saving shell session in myshell.log
-Works with the format specified in the homework

-Doesn't print anything on console while saving to file



7. Set path

-Accepts path address

-Searches for executable and executes

-Format: set path (. /bin). Works with this format

-Program handels errors if the set path command is not in the correct format. An error results in the following message-
"No such command. Format for set path is: set path = (paths separated by spaces)"

-Program handles conditions if the command entered by the user is not in the specified path. An error results in the following message-
"No such command in the specified path."



8. exit
-Exits the shell on the command exit