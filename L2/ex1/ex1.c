/*************************************
* Lab 2 Exercise 1
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
--------------------------------------
Warning: Make sure your code works on
compute cluster node (Linux on x86)
*************************************/

#include <stdio.h>
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>     //for fork()
#include <sys/wait.h>   //for wait()

int main()
{
    int nChild;

    //Read the number of child
    scanf("%d", &nChild);

    //Array to store child PIDs
    int childPids[nChild];

    //Spawn child processes
    for (int i = 0; i < nChild; i++) {
        int status = fork();
        if (status == 0) {
            printf("Child %d[%d]: Hello!\n", i + 1, getpid());
            return 0;
        } else {
            childPids[i] = status;
        }
    }
    
    //Wait on child processes in order
    for (int i = 0; i < nChild; i++) {
        waitpid(childPids[i], NULL, 0);
        printf("Parent: Child %d[%d] done.\n", i + 1, childPids[i]);
    }
    
    printf("Parent: Exiting.\n");

    return 0;
}
