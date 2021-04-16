/*************************************
* Lab 1 Exercise 3
* Name     : Ritesh Kumar
* Matric No: A201829H
* Lab Group: 12
*************************************/

/*************************************
* Lab 1 Exercise 3
* Name: Fang Junwei, Samuel
* Matric No: A0199163U
* Lab Group: 01
*************************************/

#include <stdio.h>
#include <assert.h>

//Datatype Declarations
typedef int (*arithFuncPtr)(int, int);


//Function Prototypes
int add(int x, int y);


int main()
{

   	// set up array of function pointers and function names: 
	// indicate prototypes: 
	int lumos(int x, int y);
	int alohomora(int x, int y);
	int expelliarmus(int x, int y);
	int sonorus(int x, int y);
	char spellNames[4][13] = {"lumos", "alohomora", "expelliarmus", "sonorus"}; // largest spell has 12 chars:
	arithFuncPtr spells[4] = {lumos, alohomora, expelliarmus, sonorus};


	int spellNumber, x, y, res;
	while(scanf("%i %i %i", &spellNumber, &x, &y) == 3 ) {
		spellNumber--;
		res = (*spells[spellNumber])(x, y);
		printf("%s %i\n", spellNames[spellNumber], res);
	}
	return 0;



    //The code below is to show the usage of a function pointer only
    //You can remove/comment them off 

    //arithFuncPtr is a function pointer datatype
    //   - it points to afunction that takes (int, int) as parameter
    //   - and returns int as return result

	/*========================  demo: ==============================
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
	================================================================*/

}


// arithmetic functions:

int add(int x, int y)
{
    return x+y;
}

//Sum of Arithmetic progression b/w two bounds:
int lumos(int x, int y) {
	assert(x <= y);
	int result = 0;
	for(int i = x; i <= y; i++) {
		result += i;
	}
	return result;
}

//GCD function:
int alohomora(int x, int y) {
	if(x == 0 || y == 0) {
		return (x == 0) ? y : x;
	} else if (x == y) {
        return x;
	} else { // actual gcd:
        return (x > y) 
				? alohomora(x - y, y)
				: alohomora(y - x, x);	
	}
}

// remove factor y from x completely:
int expelliarmus(int x, int y) {
	while(x % y == 0) {
		x /= y;
	}
	return x;
}


// Power:
int sonorus(int x, int y) {
	int result = 1;
	while(y > 0) {
		result *= x;
		y--;
	}
	return result;
}


