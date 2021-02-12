/*************************************
* Lab 2 Exercise 2
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


int main()
{
    char **cmdLineArgs;
    char path[20] = ".";  //default search path
    char userInput[121];
	char *command;

    int tokenNum;

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
        //Figure out which command the user want and implement below
		if (strcmp("showpath", command) == 0) { 
			printf("%s \n", path);
		} else if (strcmp("setpath", command) == 0){
			strcpy(path, cmdLineArgs[1]);
		} else { // specific commands: 

			// determine the execpath: 
			struct stat sb;
			char *execPath = (char *) malloc(sizeof('c') * 20);
			strcpy(execPath, path);
			strcat(execPath, "/");
			strcat(execPath, command);
		
		    // run executable as a child proc if possible:	
			if(stat(execPath, &sb) != 0) { 
				printf("\"%s\" not found \n", execPath);
			} else { // valid exec path, fork and execl:
				int cpid = fork();
				if(cpid == 0) { // child proc, call execl
					execl(execPath, command, (char*) NULL);
					return 0; // this return prevents fork-bombing
				} else { // parent proc, should wait for cleanup purposes
					wait(NULL);	
				}
			}


		}
        //Prepare for next round input

        //Clean up the token array as it is dynamically allocated
        freeTokenArray(cmdLineArgs, tokenNum);



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

/*=============== ex2. TAKEAWAYS: ====================================
1. From example files: 
	A) Allow program to take varargs by doing a case-switch on argc(# of args).
2. Assigning a new value to a string. Since strings are *char / char[], 
	we have to replace the existing value by copying something into it. That's
	why we need to use the strcpy(<dest>, <src>) function.
3. Param list for execl: need to supply the execpath and the command followed by a
   bunch of varargs. If there aren't any flags to be passed in, pass in a NULL, 
   but have to typecast it to char* so pass in (char*) NULL. Had no idea that 
   NULL could be typecasted.	

 
 ====================================================*/



