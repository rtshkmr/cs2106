#include <stdlib.h>
#include <stdio.h>


int main(int argc, char** argv)
{
    int N, M;
    int ** aPPtr; //A dynamic array of integer pointers
    //Ensure there are N and M value
    if (argc < 3){
        printf("Usage: %s N M\n", argv[0]);
        printf("M expected to be <= N\n");
    }

    //Convert command line arguments to integers
    N = atoi(argv[1]);
    M = atoi(argv[2]);


    //Alllocate an array of N$ of int*, i.e. each element is an integer
    //pointer
    aPPtr = (int**) malloc(sizeof(int*) * N);

    //Allocate the integers
    for (int i = 0; i < N; i++){
        aPPtr[i] = (int*) malloc(sizeof(int)); 
    }

    //Only free M of them
    for (int i = 0; i < M; i++){
        free(aPPtr[i]);
    }


    //Free the array itself
    free(aPPtr);



    return 0;
}
