#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include "sha-256.h"

// Programmer: Nathan Hodgson
// Program name: main.c

int main(int args, char** arg) {
	int processes = atoi(arg[3]);
	char buffer[50];
	int i = 0;
	int processArray[processes];
	char* argv[5];
	char* hasher = "./hasher";

	if (args != 4) {
		printf("Invalid number of command line arguments... Please try again.\n");
		exit(1);
	}

	argv[0] = hasher;
	argv[1] = arg[1];
	argv[2] = "out0.txt";
	argv[3] = arg[2];
	argv[4] = NULL;

	pid_t ret;

	for (i=0; i<processes; i++) {
		ret = fork();
		processArray[i] = ret;
		if (ret == 0) {
			sprintf(buffer, "%s%d%s", "out", i, ".txt");
			argv[2] = buffer;
			execvp(hasher, argv);
		} 
	}
	
    int status;
   	pid_t cpid = wait(&status);

   	if (WIFEXITED(status)) {
   		for (i=0; i<processes; i++) {
   			if (processArray[i] != cpid) {
   				kill(processArray[i], SIGKILL);
   			}
   			else {
   				printf("Name of output file: out%d.txt\n", i);
   			}
   		}
   	} else if (WIFSIGNALED(status)) {
     	printf("Terminated by signal %d\n", WTERMSIG(status));
    }

	return 0;
}