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
// Program name: hasher.c

void readFile(char* inputArray, int args, char** arg);
int randomNum();
void sigIntHandler(int sig);
void hasher(char hash[], char* inputArray, int args, char** arg);

int writePipe = 0; // Declare writing end of pipe as global for signal handler

int main(int args, char** arg) { // Driver function

	char hash[65];
	writePipe = atoi(arg[2]);
	signal(SIGINT, sigIntHandler);

	char* inputArray;
	inputArray = (char *) malloc(sizeof(char) * 1000000);
	
	readFile(inputArray, args, arg);
	hasher(hash, inputArray, args, arg);

	free(inputArray);

	printf("Hasher has found a solution, terminating...\n");

	return 0;
}

/* ====================================================================================
readFile(): opens a user-specified command line filename given through arg[1] and appends
the contents of the file to the dynamically allocated array "inputArray", followed by 
a null terminator. Must have exactly 4 command line arguments for this function to
 execute properly. 
==================================================================================== */

void readFile(char* inputArray, int args, char** arg) { 
	char fileName[100], buffer[100];
	int c;
	int i = 0;
	
	if (args != 4) {
		printf("Incorrect number of command line arguments... Please try again.\n");
		exit(1);
	}

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
		while (c != 0) { // Stores entire file into inputArray, character by character
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
randomNum(): generates a random integer between 0 and 999999 seeded by using the time
of day and returns the number.
==================================================================================== */

int randomNum() { 
	int digits = 6;
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	srand(((((int)current_time.tv_sec * 1000000))+((int)current_time.tv_usec)));
	int num = rand() % 999999;

	return num;
}

/* ====================================================================================
hasher(): Finds a hash value in hex given a specified number of leading zeroes. The 
file contents followed by the random number and count which gave the correct hash value
are appended to the "inputArray".
==================================================================================== */

void hasher(char hash[], char* inputArray, int args, char** arg) { 
	char* buffer;
	buffer = (char *) malloc(sizeof(char) * 1000000);
	int numArray[2];
	int i = 0;
	int count = 0;
	int zeroes = 0;
	int num = 0;
	int zeroesNeeded = atoi(arg[3]);

	num = randomNum();

	while (zeroes < zeroesNeeded) { // Continuously hashes the buffer with an incremented count until zeroesNeeded is found

		if (count > 999999) {
			num = randomNum();
			count = 0;
		}
		zeroes = 0;
		i = 0;
		sprintf(buffer, "%s%06d%06d", inputArray, num, count);
		sha_256_string(hash, buffer, strlen(buffer));

		while (hash[i] == '0') { // Increments "zeroes" by 4 for each leading hex value equal to 0
			zeroes += 4;
			i++;
		}
		if (hash[i] == '1')
			zeroes += 3;
		else if (hash[i] == '2' || hash[i] == '3')
			zeroes += 2;
		else if (hash[i] == '4' || hash[i] == '5' || hash[i] == '6' || hash[i] == '7')
			zeroes += 1;
		else
			zeroes += 0;

		count++;
	}

	numArray[0] = num;
	numArray[1] = count-1; // Decrement count before appending to numArray

	printf("\nHash: %s\n", hash);

	write(writePipe, numArray, 2 * sizeof(int));
	close(writePipe);

	sprintf(inputArray, "%s", buffer);

	free(buffer);

	return;
}

/* ====================================================================================
sigIntHandler(): function is triggered when the program is sent a SIGINT signal. Once
this signal is received, the function closes the writing end of the pipe and exits.
==================================================================================== */

void sigIntHandler(int sig) {
	close(writePipe);
	exit(0);
}


