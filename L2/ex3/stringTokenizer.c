#include <stdio.h>
#include <stdlib.h>
#include <string.h>     //for string comparison etc

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
    char **argsArr;
    char input[120], delimiter[80];
    int i, nToken;


    //Example usage
    printf("Example:\n");
    fgets(input, 120, stdin);

    printf("Delimiter: ");
    fgets(delimiter, 80, stdin);

    argsArr = split(input, delimiter, 5, &nToken);
    printf("%d Tokens read:\n", nToken);

    for (i = 0; i < nToken; i++){
        printf("Arg %d \"%s\"\n", i, argsArr[i]);
    }
    
    //Free the memory space
    freeTokenArray(argsArr, 5);
    argsArr = NULL;     //Just a good habit

    return 0;

}
