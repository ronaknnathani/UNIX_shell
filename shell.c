/**
 * Simple shell interface program.
 *
 * Author = Ronak Nathani
 * CS 543 | Operating Systems
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include "uthash.h"
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_LINE 80 /* 80 chars per line, per command */

char *args[MAX_LINE/2 + 1]; /* Arguments from command line are stored in args */
char *pathUser[41];
char *token;
pid_t pid;  
char temp[81];
char tempComplete[81];
int i=0;
int pathIndex = 0;
int argsCount;  
int verbose = 0;
int pathFlag = 0;
char pathTemp[81]; 
int logFlag = 0;
int outFile;
int saved_stdout;
int waitFlag = 0;
char pathCmd[81];

// Structure for alias
struct alias{
    char aliasName[81];
    char commandName[81];
    UT_hash_handle hh;
};

struct alias *check;

struct alias *aliases = NULL;

// Adding alias to the hash table
void add_alias(char key[], char cmd[]) {
    struct alias *s;
    HASH_FIND_STR(aliases, key, s);  
    if (s==NULL) {
        s = (struct alias*)malloc(sizeof(struct alias));
        strcpy(s->aliasName, key);
        HASH_ADD_STR(aliases, aliasName, s); 
    }
    strcpy(s->commandName, cmd);
}

// Structure for linkedlist to store history
struct node{
    char command[81];
    struct node *next;
};

struct node *head = NULL;

// Entering commands to history at the head of linkedlist
void push(struct node **headref, char cmd[]){
    struct node *newNode = malloc(sizeof(struct node));
    strcpy(newNode->command, cmd);
    newNode->next = *headref;
    *headref = newNode;
}

// Returns length of history
int length(struct node *head){
    struct node *tempk = head;
    int i=0;
    while(tempk!=NULL){
        i++;
        tempk = tempk->next;
    }
    return i;
}

// Removes most recent command from the history
void pop(struct node **headref){
    struct node *tempk = *headref;
    *headref = tempk->next;
    free(tempk);
}

// Function to display history
void display(struct node *head){
    int j = 10;
    struct node *tempk = head;
    int commandLength = length(head);
    if (commandLength > 10){
        while(j!=0){
            printf("%d \t %s \n", commandLength--, tempk->command);
            tempk = tempk->next;
            j--;
        }
    }
    else{
        while(tempk!=NULL){
            printf("%d \t %s \n", commandLength--, tempk->command);
            tempk = tempk->next;
        }
    }
}

// Function to fetch Nth command in the history
char* GetNth(struct node *head, int index)
{
    struct node *current = head;
    int l = length(current);
    while (current != NULL)
    {
        if (l == index)
        {
            return current->command;
        }
        l--;
        current = current->next;
    }
}

// Searching command when path set by the user
int findCommand(char *args[]) {
    int iter = 0;
    int flag = 0;
    int fo;
    for(iter = 0; pathUser[iter]!=NULL; iter++) {
        strcpy(pathCmd, pathUser[iter]);
        strncat(pathCmd, args[0], strlen(args[0]));
        if((fo = open(pathCmd, O_RDONLY)) > 0) {
            flag = 1;
            close(fo);
            return 1;
                }
        }
    return 0;
}

// Create a child process and execute
void executeCommand(char *args[], char *pathUser[]) {
    char * const argv[] = {NULL};
    char * const envp[] = {NULL};
    int iter = 0;
    int flag;
    int ex;
    int fo;
    pid_t pid;
    pid = fork();
    if (pid < 0) { 
        fprintf(stderr, "Fork Failed");
        exit(1);
    }
    else if (pid == 0) { /* child process */
        if (pathFlag == 1) { 
            flag = findCommand(args);
            if (flag == 1) {
                ex = execve(pathCmd, args, envp);
                if(ex < 0) {
                    printf("No such command in the specified path.\n");
                    exit(1);        
                }
            }    
            else if (flag == 0) {
                ex = execve(args[0], args, envp);
                if(ex < 0) {
                    printf("No such command in the specified path.\n");
                    exit(1);        
                }
            }  
        }
        else {
            if (execvp(args[0], args) == -1) {
                perror("osh");
                fflush(stdout);
            }
        }
        exit(EXIT_SUCCESS);
    }
    else { /* parent process */
    // printf("Child running\n");
    if (waitFlag == 0){ /* parent runs concurrently if '&' in command */
        if (verbose == 1){
            printf("Parent process waiting\n");
        }
        wait(NULL); /* parent will wait for the child to complete */
    }
        // printf("Child Complete\n");
    }
    return;
}

// Check if the command entered is an internal shell keyword
void checkCommand(char *args[]){
    int k=1;
    char getInt[81];
    int num;
    int flag = 0;
    char temp1[81];
    HASH_FIND_STR(aliases, args[0], check);

    // Exit the shell prompt
    if (strcmp(args[0], "exit") == 0){
        if (verbose==1){
            printf("Exit command line shell interpreter \n");
        }
        exit(1);
    }

    // Change directory
    else if (strcmp(args[0], "cd") == 0){
        if (verbose==1){
            printf("Changing directory \n");
        }
        if (args[1]==NULL){
            chdir(getenv("HOME"));
        }
        else{
            if (chdir(args[1]) == -1){
                printf("No such directory: %s\n", args[1]);
            }
        }
    }

    // Display history - max 10 most recent commands
    else if (strcmp(args[0], "history") == 0){
        if (verbose==1){
            printf("History of commands (max 10 past commands shown) \n");
        }
        if (length(head) > 0){
            display(head);
        }
        else {
            printf("No commands in the history\n");
        }
    }

    // Execute most recent command
    else if (strcmp(args[0], "!!") == 0){
        if (length(head) <= 1){
            printf("No commands in the history\n");
        }
        else{
            if (verbose==1){
                printf("Executing the most recent command in the history \n");
            }
            pop(&head);
            strcpy(temp1, head->command);
            push(&head, temp1);
            printf("Executing command: %s\n", temp1);
            while (i!=0){
                args[i] = NULL;
                i--;
            }
            token = strtok(temp1, " ");
            while (token != NULL && i < 40){
                args[i] = token;
                token = strtok(NULL," ");
                i++;
            }
            checkCommand(args);
        }
    }

// !Nth command execution
    else if (temp[0]=='!' && temp[1]!='!'){
        if (isdigit(temp[k])) {
            strcpy(getInt, &temp[k]);
            num = atoi(getInt);
            pop(&head);
            if (num > length(head) || num < (length(head) - 9) || num <=0) {
                printf("No such command in history\n");
                return;
            }
            strcpy(temp1, GetNth(head, num));
            strcpy(temp, temp1);
            strcpy(tempComplete, temp1);
            push(&head, temp1);
            while (i!=0){
                args[i] = NULL;
                i--;
            }
            token = strtok(temp1, " ");
            while (token != NULL && i < 40){
                args[i] = token;
                token = strtok(NULL," ");
                i++;
            }
            if (verbose==1){
                printf("Executing the Nth command in the history \n");
            }
            printf("Executing command: %s\n", tempComplete);
            checkCommand(args);
        }   
        else{
            printf("Error\n");  
        }
    }

    // Creating alias
    else if (strcmp(args[0], "alias") == 0) {
        if (args[1] == NULL || args[2] == NULL || tempComplete[strlen(tempComplete)-1] != '"') {
            printf("No such command. Format for alias is: alias aliasName \"commandName\" \n");
            return;
        }
        char *aliasToken;
        char *aliasArgs[5];
        int a;
        char aliasTemp[81];
        struct alias *s;
        aliasTemp[0] = '\0';
        strcpy(temp1, &tempComplete[7+strlen(args[1])]);
        temp1[strlen(temp1)-1] = '\0';
        strcpy(temp1, &temp1[1]);
        add_alias(args[1], temp1);
        HASH_FIND_STR(aliases, args[1], s);
        if (verbose==1){
            printf("Creating alias '%s' for command '%s'\n", args[1], temp1);
        }
    }
     
    // Executing a previously created alias    
    else if (check != NULL) {
        struct alias *s;
        HASH_FIND_STR(aliases, args[0], s);
        strcpy(temp1, s->commandName);
        if (verbose==1){
            printf("Executing an alias to command -> %s\n", temp1);
        }
        while (i!=0){
            args[i] = NULL;
            i--;
        }
        token = strtok(temp1, " ");
        while (token != NULL && i < 40){
            args[i] = token;
            token = strtok(NULL," ");
            i++;
        }
        checkCommand(args);
    }   

    // Test for commands beginning with set
    else if (strcmp(args[0], "set") == 0 && args[1]==NULL) {
         printf("No such command. Format for set path is: set path = (paths separated by spaces) \n");
    }

    // Verbose feature
    else if (strcmp(args[0], "set") == 0 && strcmp(args[1], "verbose") == 0) {
        if (strcmp(args[2],"on") == 0) {
            verbose = 1;
            printf("Verbose on\n");
        }
        else if (strcmp(args[2],"off") == 0){
            verbose = 0;
        }
        else {
            printf("No such command. Only arguments allowed with verbose are on and off \n");

        }
    }

    // Set path feature
    else if (strcmp(args[0], "set") == 0 && strcmp(args[1], "path") == 0) {
        if (args[3] == NULL || tempComplete[strlen(tempComplete)-1] != ')') {
            printf("No such command. Format for set path is: set path = (paths separated by spaces) \n");
            return;
        }
        if (args[2]!=NULL && strcmp(args[2], "=") == 0){
            pathFlag = 1;
            strcpy(temp1, &tempComplete[4 + strlen(args[0]) + strlen(args[1]) + strlen(args[2])]);
            temp1[strlen(temp1)-1] ='\0';
            printf("Path(s) provided: %s\n", temp1);
            while (pathIndex!=0){
                pathUser[pathIndex] = NULL;
                pathIndex--;
            }
            token = strtok(temp1, " "); 
            while (token != NULL){
                strcpy(pathTemp, token);
                strncat(pathTemp, "/", 1);
                // printf("%s\n",  pathTemp);
                pathUser[pathIndex] = pathTemp;
                // printf("%d : %s\n", pathIndex, pathUser[pathIndex]);
                token = strtok(NULL," ");
                pathIndex++;
            }
        }
        else{
            printf("No such command. Format for set path is: set path = (paths separated by spaces) \n");
        }
    }

    // Scripting log file
    else if (strcmp(args[0], "script") == 0 && strcmp(args[1], "myshell.log") == 0) {
        logFlag = 1;
        outFile = open(args[1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        saved_stdout = dup(1);
        dup2(outFile, 1);
        close(outFile);
    }

    else if (strcmp(args[0], "endscript") == 0) {
        logFlag = 0;
        dup2(saved_stdout, 1);
    }

    // If the command entered isn't an internal shell keyword, then implement system program
    else{
        executeCommand(args, pathUser);
    }
    // return;
}

int main(int argc, char **argv, char **envp)
{
    // Initialization
    int should_run = 1;
    int start;
    char input;   
    char *inname = "cs543rc.txt";
    FILE *infile;

    //
    char *lline = NULL;
    ssize_t read;
    size_t llen = 0;
    char *end;
    //
    char line_buffer[81]; /* BUFSIZ is defined if you include stdio.h */
    int line_number = 0;
    infile = fopen(".cs543rc", "r");
    if (!infile) {
        printf("Couldn't run initialization script.\n");
        return 0;
    }
    printf("Initializing...\n");

    // Reading and executing the .cs543rc script for initialization
    while ((read = getline(&lline, &llen, infile)) != -1) {
            end = lline + strlen(lline) - 1;
              while(end > lline && isspace(*end)) end--;
            *(end+1) = 0;
            strncpy(temp, lline, (int)read);
            printf("\n%s\n", temp);
            while (i!=0){
                args[i]=NULL;
                i--;
            }
            strcpy(tempComplete, temp);
            token = strtok(temp, " ");
            while (token != NULL && i < 40){
                args[i] = token;
                token = strtok(NULL," ");
                i++;
                argsCount++;
            }
            checkCommand(args);
       }

    // Reading commands from user, parsing and processing for execution
    while (should_run){  
        printf("osh>");
        fflush(stdout);
        start = 0;
        argsCount = 0;
        waitFlag = 0;
        input = getchar();
        
            switch(input){
                case '\n': 
                    break;
                case EOF:
                    should_run = 0;
                    break;
                default: 
                    while (i!=0){
                        args[i]=NULL;
                        i--;
                    }
                    while (input != '\n' && start < MAX_LINE){
                        temp[start] = input;
                        start++;
                        input = getchar();
                    }
                    temp[start] = 0x00;
                    push(&head, temp);
                    if (temp[(int)strlen(temp) - 1] == '&') {
                        waitFlag = 1;
                         temp[(int)strlen(temp) - 1] = '\0';
                    }
                    strcpy(tempComplete, temp);
                    if (temp[(int)strlen(temp) - 1] == '&') {
                        waitFlag = 1;
                    }
                    token = strtok(temp, " ");
                    while (token != NULL && i < 40){
                        args[i] = token;
                        token = strtok(NULL," ");
                        i++;
                        argsCount++;
                    }
                    if (logFlag == 1){
                        printf("%s\n", tempComplete);
                    }
                    checkCommand(args);
                    break;
            }
    }
    return 0;
}
