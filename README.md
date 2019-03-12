Wheatcoin
---
Spring of 2019

Programmer:
Nathan Hodgson

### Program Description

This program calculates a hash given a user-specified number of leading zeroes and desired number of processes. To calculate a unique hash each time, the program generates a random number and appends it to the input array, followed by a counter which is also appended to the array for every iteration. 

### Execution Specifications

Run main.c with three command line arguments:
1. Name of input file (default sampleInput.txt is given)
2. Number of leading zeroes that should be present in the hash
3. Number of processes to start

### Sample Command Line Argument

./main sampleInput.txt 15 4

### Output

- A .txt file containing the hashed input which corresponds with the correct hash. 
- Example: "out3.txt" means that the third process finished first, so all other processes were terminated.
