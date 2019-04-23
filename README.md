Wheatcoin: Multiprocessing and Pipes
---
Spring of 2019

Programmer:
Nathan Hodgson

### Program Description

This program calculates a hash given a user-specified number of leading zeroes and desired number of processes. To calculate a unique hash each time, the program generates a random number and appends it to the input array, followed by a counter which is also appended to the array for every iteration. Once a child process calculates the correct hash, it writes the random number and counter to a pipe and terminates. Main.c reads the pipe, terminates all of the other child processes, and creates the output file. 

### Compilation Specifications

Compile main.c with the following command line arguments:
- gcc main.c sha-256.c -o main -g

(May also need to compile hasher.c, if so):
- gcc hasher.c sha-256.c -o hasher -g

### Execution Specifications

Run main.c with the following command line arguments:
1. Name of input file (default sampleInput.txt is given)
2. Name of output file
3. Number of leading zeroes that should be present in the hash
4. Number of processes to start

### Sample Command Line Argument

./main sampleInput.txt output.txt 15 4

### Output

- The correct hash value calculated by one of the child processes
- The name of the output file specified
- The contents of the input .txt file concatenated with the random number and counter are written to the output file. In order to verify that the program found the correct number of leading zeroes, one can copy and paste the contents of the output file and hash it using an online hash tool. Note that this program uses SHA-256. 
