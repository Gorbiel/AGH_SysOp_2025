/* 
   proces rodzica tworzy wiele procesow potomnych
   i wykrywa ich za konczenie za pomoca sygnalu
   Kazdy proces potomny wyswietla komunikat "I'm the child" i "exits" po uplywie n sekund,
   gdzie n "is the sequence in which it was forked" 
   
   Nalezy uzypelnic kod programu, a nastepnie uruchomic program. 
   Komunikaty wyswietlone w termninalu znajduja sie w pliku out.txt
*/

#include <stdlib.h>	
#include <unistd.h>	
#include <signal.h>	
#include <stdio.h>	
#include <sys/types.h>
#include <sys/wait.h>

#define NUMPROCS 6		     /* number of processes to fork */
int nprocs = NUMPROCS;	     /* number of child processes   */

void catch(int);	         /* signal handler */
void child(int n);	         /* the child calls this */

int main(int argc, char **argv) {  
	int pid;
	int i;

	signal(SIGCHLD, catch);	/* detect child termination */

	for (i = 0; i < NUMPROCS; i++) {
		switch (pid = fork()) {
		case 0:			/* a fork returns 0 to the child */
			child(i + 1);    /* numeracja od 1 */
			break;          /* niepotrzebne, exit w child */
		case -1:		/* something went wrong */
			perror("fork");
			exit(1);
		default:		/* parent just loops to create more kids */
			printf("parent: forked child %d\n", pid);
			break;
		}
	}
	
	printf("parent: going to sleep\n");

	while (nprocs != 0) {
		printf("parent: sleeping\n");
		sleep(60);	/* do nothing for a minute */
	}

	printf("parent: exiting\n");
	exit(0);
}

void child(int n) {
	printf("\tchild[%d]: child pid=%d, sleeping for %d seconds\n", n, getpid(), n);
	sleep(n);							
	printf("\tchild[%d]: I'm exiting\n", n);
	exit(100 + n);						
}

void catch(int snum) {
	int pid;
	int status;

	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
		printf("parent: child process pid=%d exited with value %d\n", pid, WEXITSTATUS(status));
		nprocs--;
	}
	
	signal(SIGCHLD, catch);  // re-arm signal handler
}
