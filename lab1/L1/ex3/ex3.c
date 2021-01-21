/*************************************
* Lab 1 Exercise 3
* Name     :
* Matric No:
* Lab Group:
*************************************/

#include <stdio.h>

//Datatype Declarations
typedef int (*arithFuncPtr)(int, int);


//Function Prototypes
int add(int x, int y);


int main()
{
    int a, b, optype, res;

    //The code below is to show the usage of a function pointer only
    //You can remove/comment them off 

    //arithFuncPtr is a function pointer datatype
    //   - it points to afunction that takes (int, int) as parameter
    //   - and returns int as return result

    arithFuncPtr ptr;

    //ptr points to the function add
    ptr = add;

    scanf("%i %i", &a, &b);

    //Defereference a function pointer === call the function
    //if you take (*ptr) as some function f
    // then below is the same as
    //   res = f(a, b);

    res = (*ptr)(a, b);

    printf("%i\n", res);
   
    return 0;
}

int add(int x, int y)
{
    return x+y;
}
