#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include "sha-256.h"

// Programmer: Nathan Hodgson
// Program name: main.c

void readFile(char* inputArray, int args, char** arg);
void writeFile(char* inputArray, int pipeRead[], int args, char** arg);

int main(int args, char** arg) {
	int processes = atoi(arg[4]);
	int howManyBytes = 0, i = 0, checkFail = 0;
	int pipeRead[2];
	char** pipeBuffer;
	char* inputArray;
	int processArray[processes];
	char* argv[5];
	char* hasher = "./hasher";
	int thepipe[2 * processes]; // Creates number of pipes needed for processes

	pipeBuffer = malloc(sizeof(char*) * processes);
	inputArray = (char *) malloc(sizeof(char) * 1000000);

	for(int i=0;i<processes;i++) // Allocate dynamic memory for the pipe buffer
		pipeBuffer[i] = malloc(sizeof(char*)); 

	for (int i = 0; i < processes; i++) { // Open "processes" number of pipes and check if any fail
		checkFail = pipe(&thepipe[2*i]);
		if (checkFail == -1) {
			printf("Pipe failed. Terminating...\n");
			return 1;
		}
		sprintf(pipeBuffer[i], "%d", thepipe[2*i + 1]); // Store each write value into an array to be used for child processes
	}

	if (args != 5) {
		printf("Invalid number of command line arguments... Please try again.\n");
		exit(1);
	}

	argv[0] = hasher;
	argv[1] = arg[1];
	argv[2] = "";
	argv[3] = arg[3];
	argv[4] = NULL;

	pid_t ret;

	for (i=0; i<processes; i++) { // Creates the number of processes specified by the user and assigns each a pipe-write value
		ret = fork();
		processArray[i] = ret;
		if (ret == 0) {
			argv[2] = pipeBuffer[i];
			execvp(hasher, argv);
		} 
	}
	
    int status;
   	pid_t cpid = wait(&status);

   	if (WIFEXITED(status)) {
   		for (i=0; i<processes; i++) {
   			if (processArray[i] != cpid) {
   				kill(processArray[i], SIGINT);
   			}
   			else {
   				howManyBytes = read(thepipe[2*i], pipeRead, 2*sizeof(int));
   				close(thepipe[2*i]);
   			}
   		}
   	} else if (WIFSIGNALED(status)) {
     	printf("Terminated by signal %d\n", WTERMSIG(status));
    }

    readFile(inputArray, args, arg);
    writeFile(inputArray, pipeRead, args, arg);
    free(inputArray);

    for(int i=0;i<processes;i++) // Free dynamically allocated memory
		free(pipeBuffer[i]);
    free(pipeBuffer);

    printf("\nName of output file: %s\n", arg[2]);
	return 0;
}

/* ====================================================================================
readFile(): opens a user-specified command line filename given through arg[1] and appends
the contents of the file to the dynamically allocated array "inputArray", followed by 
a null terminator. 
==================================================================================== */

void readFile(char* inputArray, int args, char** arg) { \
	char fileName[100];
	char c;
	int i = 0;

	strcpy(fileName, arg[1]);

	FILE* infile;
	infile = fopen(fileName, "r");

	if (infile == NULL) {
		printf("Invalid file name... Please try again.\n");
		fclose(infile);
		exit(1);
	}
	else {
		c = fgetc(infile);
		while (c != 0) { // Stores entire file into inputArray, character by character, followed by a null terminator
			if (feof(infile)) {
				break;
			}
			inputArray[i] = c;
			c = fgetc(infile);
			i++;
		}
		inputArray[i] = '\0';

		fclose(infile);
	}
	return;
}

/* ====================================================================================
writeFile(): creates an output file based on the correct value hashed, and then writes
the correct string to the output file followed by the random number and count obtained
from the reading end of the pipe of the hasher program. The output file name will be
what was specified in the command line argument 2. 
==================================================================================== */

void writeFile(char* inputArray, int pipeRead[], int args, char** arg) { 
	int i = 0;
	char outputName[100];
	strcpy(outputName, arg[2]);

	FILE* outfile; 
	outfile = fopen(outputName, "w");

	if (outfile == NULL) {
		printf("Invalid output file name... Please try again.\n");
		fclose(outfile);
		exit(1);
	} 
	else {
		for (i=0; inputArray[i] != '\0'; i++) // Writes the entire inputArray to the file, one character at a time (until null terminator)
			fputc(inputArray[i], outfile);

		fprintf(outfile, "%06d%06d", pipeRead[0], pipeRead[1]);

		fclose(outfile);
	}
	return;
}

