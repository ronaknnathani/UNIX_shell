##Test cases for all functionalities

####1. Creating a child process
* Test case 1: ls
* Test case 2: cal
* Test case 3: gedit test.txt &

####2. history feature
* Test case 1: history
* Test case 2: !!
* Test case 3: !2

####3. Alias
* Test case 1: alias dir “ls al”
* Test case 2: alias cls “printf “\033c””
* Test case 3: alias p “pwd”

####4. Verbose
Implementing the following commands in sequence shows the affect of verbose
* Test case 1: set verbose on
* Test case 2: !!
* Test case 3: set verbose off
* Test case 4: !!
* Test case 5: set verbose on
* Test case 6: !N
* Test case 7: alias dir “ls”

####5.Initialization script (The last line of the script should be kept empty)
* Test case 1: alias cls "printf "\033c""
* Test case 2: pwd
* Test case 3: cal
* Test case 4: date

####6. Saving shell session in 'myshell.log'
* First enter 'script myshell.log' on the command line- this will start saving the session
  * Test case 1: ls -l
  * Test case 2: pwd
  * Test case 3: cal
* Enter 'endscript' on the command line to stop recording the session  

####7. Set path
* Test case 1: set path = (/bin)
* Test case 2: ls
* Test case 3: set path = (.)
* Test case 4: test (it is an executable generated from the program test.c. test.c is compiled along with shell.c)
* Test case 5: set path = (. /bin)
* Test case 6: test
* Test case 7: ls

8. Exit
* Exits the shell


