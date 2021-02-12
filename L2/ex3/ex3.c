/*************************************
* Lab 2 Exercise 3
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
--------------------------------------
Warning: Make sure your code works on
compute cluster node (Linux on x86)
*************************************/

#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
//#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()


char** split( char* input, char* delimiter, int maxTokenNum, int* readTokenNum )
//Assumptions:
//  - the input line is a string (i.e. with NULL character at the end)
//  - the delimiter is a string of possible delimiters, each delimiter is single chracter
//Behavior:
//  - Split up to and include maxTokenNum from the input string
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - Must use the freeTokenArray to free memory after use!
{
    char** tokenStrArr;
    char* tStart;   //start of token
    int i, strSize;      

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum );

    //Nullify all entries
    for (i = 0; i < maxTokenNum; i++){
        tokenStrArr[i] = NULL;
    }

    //Look for next token as separated by the delimiter
    tStart = strtok(input, delimiter); 

    i = 0;
    while (i < maxTokenNum && tStart){

        strSize = strlen(tStart);

        //Allocate space for token string. +1 for null terminator
        tokenStrArr[i] = (char*) malloc(sizeof(char) * (strSize + 1) );

        strcpy(tokenStrArr[i], tStart);    

        i++;
        tStart = strtok(NULL, delimiter); 
    }

    //return number of token read
    *readTokenNum = i;

    return tokenStrArr;
}

void freeTokenArray(char** strArr, int size)
{
    int i;

    //Free each string first
    for (i = 0; i < size; i++){
        if (strArr[i])
            free( strArr[i] );
        strArr[i] = NULL;
    }

    //Free the entire array
    free( strArr );

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}

bool fileExists (char *filename) {
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

void runCommandBackground (char *fullPath, char **cmdLineArgs, int *pid) {
    if (fileExists(fullPath)) {
        int status = fork();
        if (status == 0) {
            execv(fullPath, cmdLineArgs);
        } else {
            *pid = status;
            printf("\"Child %d in background\"\n", status);
        }
    } else {
        printf("\"%s\" not found\n", fullPath);
    }
}

void runCommand (char *fullPath, char **cmdLineArgs, int *childResult) {
    if (fileExists(fullPath)) {
        if (fork() == 0) {
            execv(fullPath, cmdLineArgs);
        } else {
            wait(childResult);
        }
    } else {
        printf("\"%s\" not found\n", fullPath);
    }
}

void getUserInput(char userInput[121]) {
    //read user input
    printf("YWIMC > ");

    //read up to 120 characters from user input
    fgets(userInput, 120, stdin);
}

int main()
{
    char **cmdLineArgs;
    char path[20] = ".";  //default search path
    char userInput[121];
    char fullPath[40];
    char slash[2] = "/";

    int tokenNum;

    int pids[10] = {0}; 
    bool waited[10] = {false};
    int currentPid = 0;

    int childResult = 0;

    getUserInput(userInput);

    //split is a helper function to "tokenize" the input
    //see "stringTokenizer.c" program for more sample usage 
    cmdLineArgs = split( userInput, " \n", 7, &tokenNum );

    //At this point you have the user input split neatly into token in cmdLineArg[]

    while ( strcmp( cmdLineArgs[0], "quit") != 0 ){

        //Figure out which command the user want and implement below
        if ( strcmp( cmdLineArgs[0], "showpath") == 0 ) {
            printf("%s\n", path);
        } else if ( strcmp( cmdLineArgs[0], "setpath") == 0 ) {
            strcpy(path, cmdLineArgs[1]);
        } else if ( strcmp( cmdLineArgs[0], "wait") == 0 ) {
            int pid = atoi(cmdLineArgs[1]);
            bool pidPresent = false;

            for (int i = 0; i < currentPid; i++) {
                if (pid == pids[i]) {
                    pidPresent = true;
                    waited[i] = true;
                    break;
                }
            }

            if (pidPresent) {
                waitpid(pid, &childResult, 0);
            } else {
                printf("%d is not a valid child pid\n", pid);
            }
        } else if ( strcmp( cmdLineArgs[0], "pc") == 0 ) {
            printf("Unwaited Child Processes:\n");
            for (int i = 0; i < currentPid; i++) {
                if (!waited[i]) {
                    printf("%d\n", pids[i]);
                }
            }
        } else if ( strcmp( cmdLineArgs[0], "result") == 0 ) {
            printf("%d\n", childResult >> 8);
        } else {
            strcat(fullPath, path);
            strcat(fullPath, slash);
            strcat(fullPath, cmdLineArgs[0]);

            bool runInBackground = strcmp(cmdLineArgs[tokenNum - 1], "&") == 0;

            if ( runInBackground ) {
                runCommandBackground(fullPath, cmdLineArgs, &pids[currentPid]);

                currentPid++;
            } else {
                runCommand(fullPath, cmdLineArgs, &childResult);
            }
        }

        //Prepare for next round input
        strcpy(fullPath, "");

        //Clean up the token array as it is dynamically allocated
        freeTokenArray(cmdLineArgs, tokenNum);

        getUserInput(userInput);
        cmdLineArgs = split( userInput, " \n", 7, &tokenNum );
    }

    printf("Goodbye!\n");

    //Clean up the token arrays as it is dynamically allocated
    freeTokenArray(cmdLineArgs, tokenNum);

    return 0;

}
