#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/***********************
A simple program to setup regular printing.

Possible command line arguments:
    - No argument = Default: Report every 1 second for 5 times
    - 1 argument [X] = Report every 1 second for X times
    - 2 argument [X] [Y] = Report every Y second for X times

Example:
    a.out 10 3  //report every 3 second for 10 times  
    a.out 5     //report every 1 second for 5 times
***********************/

int main(int argc, char** argv)
{
    int reportTime = 1;  //default: Report every 1 second
    int repeatNum = 5;  //default: Report for 5 times
    int userInput, i;

    switch(argc){
        case 3:
            userInput = atoi(argv[2]);
            if (userInput > 0)
                reportTime = userInput;
        case 2:
            userInput = atoi(argv[1]);
            if (userInput > 0)
                repeatNum = userInput;
    }

    printf("Reporting every %i seconds for %i times\n",reportTime, repeatNum);

    for (i = 0; i < repeatNum; i++){
        sleep( reportTime );
        printf("Report No.%i: %i seconds elapsed!\n", i+1, reportTime);
    }

    return 0;
}

