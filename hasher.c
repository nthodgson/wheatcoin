#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include "sha-256.h"

// Programmer: Nathan Hodgson
// Program name: hasher.c

void readFile(char inputArray[], int args, char** arg);
void writeFile(char hash[], int args, char** arg);
int randomNum();
void hasher(char hash[], char inputArray[], int args, char** arg);

int main(int args, char** arg) {
	char inputArray[1000000], hash[65];

	readFile(inputArray, args, arg);
	hasher(hash, inputArray, args, arg);
	writeFile(inputArray, args, arg);

	return 0;
}

void readFile(char inputArray[], int args, char** arg) { // Read in the input file and append it to the array
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
		while (c != 0) {
			if (feof(infile)) {
				break;
			}
			inputArray[i] = c;
			c = fgetc(infile);
			i++;
		}

		fclose(infile);
	}
	return;
}

int randomNum() { // Generate random number and append it to the end of the array
	int digits = 6;
	int i;
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	srand(((((int)current_time.tv_sec * 1000000))+((int)current_time.tv_usec)));
	int num = rand() % 999999;

	return num;
}

void hasher(char hash[], char inputArray[], int args, char** arg) {
	char buffer[1000000];
	int i, count, zeroes, num = 0;
	int zeroesNeeded = atoi(arg[3]);

	num = randomNum();

	while (zeroes < zeroesNeeded) {

		if (count > 999999) {
			num = randomNum();
			count = 0;
		}
		zeroes = 0;
		i = 0;
		sprintf(buffer, "%s%06d%06d", inputArray, num, count);
		sha_256_string(hash, buffer, strlen(buffer));

		while (hash[i] == '0') {
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

	sprintf(inputArray, "%s", buffer);

	return;
}

void writeFile(char inputArray[], int args, char** arg) {
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
		for (i=0; inputArray[i] != '\0'; i++) 
			fputc(inputArray[i], outfile);
		fclose(outfile);
	}

	return;
}