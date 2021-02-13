/*************************************
 * Lab 2 Exercise 3
 * Name: Ritesh Kumar
 * Student Id: A0201829H
 * Lab Group: B12
 *************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
Note2: Exercise 3 is a superset of exercise 2
Please copy over your code to ex3.c and continue
Submit ONLY ex3.c (even if you cannot finish ex3)
--------------------------------------
Warning: Make sure your code works on
compute cluster node (Linux on x86)
 *************************************/

#include <stdbool.h>    // to handle bool values
#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>  
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
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

int findEmptySlot(pid_t*  backgroundJobs) {
    for(int i = 0; i < 10; i++) { // guaranteed array max size is 10
        int currentPid =  *(backgroundJobs + (i * 2) + 1);
        if (currentPid == 0) { // assumption: the pid 0 shall never be assigned since it's init(?)
            return i;
        }
    }
    return -1;
}

int getJobIdx(pid_t targetPid, pid_t* backgroundJobs) {
    for(int i = 0; i < 10; i++) { // guaranteed array max size is 10
        int currentPid =  *(backgroundJobs + (i * 2) + 1);
        if (currentPid == targetPid) { // assumption: the pid 0 shall never be assigned since it's init(?)
            return i;
        }
    }
    return -1;
}

bool isValidExecPath(char* execPath) {
    struct stat sb;
    return (bool) stat(execPath, &sb) == 0;
}
bool isValidToWait(pid_t targetPid, pid_t* backgroundJobs) {
    bool result = false;
    for(int i = 0; i < 10; i++) {
        int currentPid = *(backgroundJobs + (i * 2) + 1);
        int waitTag = 	*(backgroundJobs + (i * 2) + 2);
        result = (currentPid == targetPid) && (waitTag == 0);
        break;
    }	
    return result;
}


void markJobAsWaiting(int jobIdx, pid_t* backgroundJobs) {
    *(backgroundJobs + (jobIdx * 2) + 2) = 1;		
}

int waitForChild(pid_t cpid) {
    int wstatus;
    int exitStatus = WEXITSTATUS(wstatus); // WEXITSTATUS here is an inspection of the wstatus
    return exitStatus;
}

void deregisterBackgroundJob(int jobIdx, pid_t* backgroundJobs) {
    *(backgroundJobs + (jobIdx * 2) + 1) = 0;		
    *(backgroundJobs + (jobIdx * 2) + 2) = 0;		
}

int waitForBackgroundChild(pid_t cpid, pid_t* backgroundJobs) {
    int jobIdx = getJobIdx(cpid, backgroundJobs); 	
    markJobAsWaiting(jobIdx, (pid_t*) backgroundJobs);
    int returnValue = waitForChild(cpid);
    deregisterBackgroundJob(jobIdx, (pid_t*) backgroundJobs);
    return returnValue;
}

void registerBackgroundJob(pid_t pid, pid_t* backgroundJobs) {
    int idx = findEmptySlot(backgroundJobs);
    // update the table with pid set and wait status set to 0:
    *(backgroundJobs + (idx * 2) + 1) = pid;
    *(backgroundJobs + (idx * 2) + 0) = 0;
    printf("Child %i in background\n", pid);
}

void updateExecPath(char* command, char* execPath, char* searchPath) {
    strcpy(execPath, searchPath);
    strcat(execPath, "/");
    strcat(execPath, command);
}

void printChildren(pid_t* backgroundJobs) {
    printf("Unwaited Child Processes:\n");
    for(int i = 0; i < 10; i++) {
        int pid = *(backgroundJobs + (i * 2) + 1);
        int waitTag = *(backgroundJobs + (i * 2) + 2);
        if(pid != 0 && waitTag == 0) { // unwaited bg procs
            printf("%i\n", pid);
        }
    }
}

int main()
{
    char **cmdLineArgs;
    char searchPath[20] = ".";  //default search path
    char userInput[121];
    char *command;
    char execPath[20];

    int tokenNum;

    pid_t backgroundJobs[10][2] = { }; // tuples of (pid, boolean(0 for not waiting, 1 for waiting ))
    int previousResult = 0;

    //read user input
    printf("YWIMC > ");

    //read up to 120 characters from user input
    fgets(userInput, 120, stdin);

    //split is a helper function to "tokenize" the input
    //see "stringTokenizer.c" program for more sample usage 
    cmdLineArgs = split( userInput, " \n", 7, &tokenNum );

    //At this point you have the user input split neatly into token in cmdLineArg[]

    while ( strcmp( cmdLineArgs[0], "quit") != 0 ){
        command = cmdLineArgs[0];
        //Figure out which command the user wants and implement below
        if (strcmp("showpath", command) == 0 && tokenNum == 1) { 
            printf("%s \n", searchPath);
        } else if (strcmp("setpath", command) == 0 && tokenNum == 2){
            strcpy(searchPath, cmdLineArgs[1]);
        } else if (strcmp("wait", command) == 0 && tokenNum == 2 ){
            int targetPid = atoi(cmdLineArgs[1]);
            bool isValidChildPid = isValidToWait(targetPid, (pid_t*) backgroundJobs);
            if(isValidChildPid) {
                previousResult = waitForBackgroundChild(targetPid, (pid_t*) backgroundJobs);
            } else {
                printf("%i not a valid child pid\n", targetPid);	
            }
        } else if (strcmp("result", command) == 0){
            printf("%i\n", previousResult);
        } else if (strcmp("pc", command) == 0){
            printChildren((pid_t*) backgroundJobs);	
        } else { // specific commands: 
            updateExecPath(command, execPath, searchPath);

            if(isValidExecPath(execPath)) { 
                bool isBackgroundJob = (strcmp(cmdLineArgs[tokenNum - 1], "&") == 0);
                pid_t cpid = fork();

                if(cpid == 0) { // child proc, call execl
                    execv(execPath, cmdLineArgs);
                    return 0; // this return prevents fork-bombing
                } else { // parent proc, should wait for child
                    if(isBackgroundJob) {
                        registerBackgroundJob(cpid, (pid_t *)backgroundJobs);
                    } else {
                        previousResult = waitForChild(cpid);  
                    }
                }
            } else { // valid exec path, fork and execl:
                printf("\"%s\" not found \n", execPath);
            }

        }
        //Prepare for next round input

        //Clean up the token array as it is dynamically allocated
        freeTokenArray(cmdLineArgs, tokenNum);
        strcpy(execPath, ""); // resets execpath



        // next command:
        printf("YWIMC > ");
        fgets(userInput, 120, stdin);
        cmdLineArgs = split( userInput, " \n", 7, &tokenNum );
    }


    printf("Goodbye!\n");

    //Clean up the token array as it is dynamically allocated
    freeTokenArray(cmdLineArgs, tokenNum);


    return 0;

}

/*=============== ex3. TAKEAWAYS: ====================================

  1. From example files: 
  A) Allow program to take varargs by doing a case-switch on argc(# of args).
  2. Assigning a new value to a string. Since strings are *char / char[], 
  we have to replace the existing value by copying something into it. That's
  why we need to use the strcpy(<dest>, <src>) function.
  3. Param list for execl: need to supply the execpath and the command followed by a
  bunch of varargs. If there aren't any flags to be passed in, pass in a NULL, 
  but have to typecast it to char* so pass in (char*) NULL. Had no idea that 
  NULL could be typecasted.	

  4. we can inspect the return of the child proc as (e.g. after doing a waitpid()) 
  using options like WEXITSTATUS.

  5. learnt how to pass multidimensional array into a function. Initially I put the 
  array as backgroundJobs[10][2] then misunderstood and thought that would make it 
  pass by copy, so ended up using pointer arithmetic. Realised afterwards that it 
  would have been correct in the original way anyway. With pointer arithmetic, 
  just have to typecast your n-d array as (pid_t*) when passing it to the function,
  and supply the dimensions appropriately (if they are variables).




  ASSUMPTIONS IN CODE: 
  ---------------------
  1) PID of 0 will never be assigned to anyone (as per the tracking of procs via 
  the backgroundJobs arr)


  ====================================================================*/




