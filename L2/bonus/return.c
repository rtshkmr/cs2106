#include <stdio.h>
#include <stdlib.h>

/***********************
A simple program to return a user specified number to the shell.

Possible command line arguments:
    - No argument = this program returns "0" to the shell
    - 1 argument [X]= this program returns "X" to the shell

Note: There is no check to ensure "X" is a number.

Example:
    a.out       //this program returns 0
    a.out 5     //this program returns 5
***********************/


int main(int argc, char** argv)
{
	int userInput = 0;	

    if (argc > 1 ){
        userInput = atoi(argv[1]);
    }


	//Remember that return from main is restricted to 8-bit, i.e. 0-255
	return userInput;
}
