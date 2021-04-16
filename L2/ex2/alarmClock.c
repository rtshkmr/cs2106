#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/***********************
A simple program to simulate an alarm clock.

Possible command line arguments:
    - No argument = Alarm clock will set off in 3 seconds 
    - 1 argument [X]= Alarm clock will set off in X seconds 

Example:
    a.out       //Alarm set off in 3 seconds
    a.out 5     //Alarm set off in 5 seconds
***********************/


int main(int argc, char** argv)
{
    int sleepTime = 3;  //default
    int userInput;

    if (argc > 1 ){
        userInput = atoi(argv[1]);
        if (userInput > 0){     //make sure no negative time
            sleepTime = userInput;
        }
    }

    sleep( sleepTime );

    printf("Time's Up! %i seconds elapsed\n", sleepTime);

    return 0;
}

