/*************************************
* Lab 2 Exercise 1
* Name: Ritesh Kumar
* Student Id: A0201829H
* Lab Group: B12
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
--------------------------------------
Warning: Make sure your code works on
compute cluster node (Linux on x86)
*************************************/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>     //for fork()
#include <sys/wait.h>   //for wait()

void showChildMessage(int idx, int pid) {
	printf("Child %i [%i]: Hello! \n", idx, pid); 
}

void showParentMessage(int idx, int pid) {
	(idx >= 0 && pid >= 0) 
			? printf("Parent: Child %i [%i] done. \n", idx, pid)
			: printf("Parent: Exiting.\n");
}


int main()
{
    int nChild;
	int wstatus;

    //Read the number of child
    scanf("%d", &nChild);
	int childPids[9]  = {};

    //Spawn child processes
	for(int i = 0; i < nChild; i++) {
	    pid_t cpid = fork();
		if (cpid == 0) { // child: 
			pid_t current = getpid();
			// sleep(i); // for testing purposes.
			showChildMessage(i + 1, current);	
			exit(0); // forces children to terminate after showing message.
		} else { // parent: 
			childPids[i] = cpid;
			// nb: if children write to array, won't be reflected in parent's mem space.
		}
	}

    //Wait on child processes in order
	for(int i = 0; i < nChild; i++) {
	    waitpid(childPids[i], NULL, 0); 
		// ^status set to null since it's irrelevant, no options need to be set, hence 0


	    // int status = waitpid(childPids[i],&wstatus, 0); 
		// printf("status: %i \n", waitpid(childPids[i],&wstatus, 0)); 

		showParentMessage(i + 1, childPids[i]);
	}
	
	showParentMessage(-1, -1);
    return 0;
}


/* ============ TAKEAWAYS: ================
   1. After forking, children will execute whatever instr that follow, including the 
      subsequent for-loop iterations. 
   2. Children can't write onto array/mem space of the parent since forking will create
      copies of the parent's mem pages (if there's modification). 
   3. We can play with waitpid() 's options arguments for more control. See documentation.
 
 ========================================= */
