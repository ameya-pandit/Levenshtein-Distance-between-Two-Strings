# Levenshtein-Distance-between-Two-Srings

# Summary # 
This C++ program calculates the Levenshtein Distance between two strings (for more information about what this is check out: https://en.wikipedia.org/wiki/Levenshtein_distance), either user inputted on the command line or through a text file. The program will output each change in converting string A to string B. I used Dynamic Programming to accomplish building the functionality of this project. 

# Technologies Used #
As aforementioned, I used Dynamic Programming to accomplish building the functionality of this project. I used a 2D Array to host the characters and determine the switching process per my algorithm (more information about the algorithm is in the commemnts in the file). If the user input was through a file, I used a FILE scannner. 

# Files #
The code is all on edit_distance.cc.

the print_head() function prints the head row of the "table" that is outputted (Operation, Z (the change), Cost, and Total Cost).

outputSequence() handles the conversion from String A to String B and outputs the converted string.

editDistance() determines the cost related to each operation and builds a matrix of the values associated with each operation (which can then be interpreted to determine what to do with each conversion). 

