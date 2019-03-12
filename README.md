Wheatcoin
---
Spring of 2019

Programmer:
Nathan Hodgson

### Execution Specifications

- Run main.c with three command line arguments:
1. Name of input file (default input.txt is given)
2. Number of zeroes that should be present in the hash
3. Number of processes to start

### Sample Command Line Argument

./main input.txt 15 4

### Output

- A .txt file containing the hashed input which corresponds with the correct hash. 
- Example: "out3.txt" means that the third process finished first, so all other processes were terminated.
