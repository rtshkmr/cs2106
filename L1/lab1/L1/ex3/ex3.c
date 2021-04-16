/*************************************
* Lab 1 Exercise 3
* Name: Fang Junwei, Samuel
* Matric No: A0199163U
* Lab Group: B01
*************************************/

#include <stdio.h>

//Datatype Declarations
typedef int (*arithFuncPtr)(int, int);

//Function Prototypes
int lumos(int x, int y);
int alohomora(int x, int y);
int expelliarmus(int x, int y);
int sonorus(int x, int y);

arithFuncPtr funcs[4] = {lumos, alohomora, expelliarmus, sonorus};
char func_names[4][15] = {"lumos", "alohomora", "expelliarmus", "sonorus"};

int main()
{
    int a, b, optype, res;
        
    while (scanf("%i %i %i", &optype, &a, &b) > 0)
    {
        res = (*funcs[optype - 1])(a, b);
        printf("%s %i\n", func_names[optype - 1], res);
    } 
   
    return 0;
}

int lumos(int x, int y)
{
    return (y - x + 1) * (x + y)/ 2;
}

int alohomora(int x, int y)
{
    if (x == 0) {
       return y;
    }

    if (y == 0) {
       return x;
    }

    if (x == y) {
        return x;
    }
  
    if (x > y) {
        return alohomora(x - y, y);
    } else {
        return alohomora(x, y - x);
    }
}

int expelliarmus(int x, int y)
{
    while (x / y * y == x) {
        x /= y;
    }

    return x;
}

int sonorus(int x, int y)
{
    int z = 1;

    while (y--) {
        z *= x;
    }
    return z;
}