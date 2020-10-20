#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

using namespace std;

void print_head()
{
  cout.setf(ios::left);
  cout << setw(16) << "Operation" 
       << setw(32) << "z" 
       << setw(8) << "Cost" 
       << setw(8) << "Total" << endl;

  for (int i=0; i<64; ++i) cout << "-";
  cout << endl;
}

void outputSequence(char *stringX, char *stringY, int **operation, int m, int n)   {
    if (m < 100 && n < 100) {   //only output string inputs, not the file inputs (hence the length limit)
        int i = 0;
        int j = 0;
        int k = 0;
        int curr = 0;
        
        int cost = 0;
        int aggCost = 0;
        
        char *z = (char*)malloc(sizeof(char)*(m+n));
        strcpy(z, stringX);
        
        char operationDone[15];
        
        print_head();
        cout << setw(16) << "initial string"
             << setw(32) << stringX
             << setw(8)  << "0"
             << setw(8)  << "0" << endl;
        
        while ((i < m) || (j < n))  {
            if((operation[i][j] == 3) && (j != n))    { //insert - move down in operation array and insert stringY[j] in the current position in the z array
                z[curr] = stringY[j];
                int temp = j;
                j++;
                cost = 3;
                string insert = "insert ";
                insert += stringY[temp];
                strcpy(operationDone, insert.c_str());
                curr++;
            } else if((operation[i][j] == 2) && i != m){    //delete - move right in operation array and delete the current character of z
                i++;
                cost = 2;
                strcpy(operationDone, "delete");
            } else if((operation[i][j] == 0) && (i != m) && (j != n))  {    //right - move diagonally down in operation array and go to the next character in z array
                z[curr] = stringX[i];
                i++;
                j++;
                cost = 0;
                strcpy(operationDone, "right");
                curr++;
            } else if((operation[i][j] == 4) && (i != m) && (j != n)) { //replace - move diagonally down in operation array and replace with the current character in z array
                z[curr] = stringY[j];
                int temp = j;
                i++;
                j++;
                cost = 4;
                string replace = "replace by ";
                replace += stringY[temp];
                strcpy(operationDone, replace.c_str());
                curr++;
            }
            aggCost += cost;
            
            //outputting
            string output = "";
            
            for(k = 0; k < curr; k++){
                output += z[k];
            }
            output += "_";
            
            for(k = i; k < m; k++){
                output += stringX[k];
            }
            
            cout << setw(16) << operationDone
                << setw(32) << output.c_str()
                << setw(8)  << cost
                << setw(8)  << aggCost << endl;
        }
        free(z);
    }
}

int editDistance(char *stringX, char *stringY, int m, int n)    {
    int matrix[m+1][n+1];
    int **operation = (int**)malloc(sizeof(int*) * (m+1));
    
    int mininumCost = 0;
    
    //cost associated to each operation
    int insertCost = 0;
    int deleteCost = 0;
    int replaceCost = 0;
    int rightCost = 0;
    
    for (int i = 0; i < m+1; i++)  {
        operation[i] = (int*)malloc(sizeof(int) * (n+1));
        assert(operation[i]);
    }
    
    matrix[m][n] = 0;
    operation[m][n] = 'f';
    
    for (int i = 0; i < m+1; i++)   {
        matrix[i][n] = (m-i) * 2;
        operation[i][n] = 2;
    }
        
    for (int j = 0; j < n+1; j++)   {
        matrix[m][j] = (n-j) * 3;
        operation[m][j] = 3;
    }
    
    for (int i = m-1; i >= 0; i--)  {
        for (int j = n-1; j >= 0; j--)  {
            //total costs associated with each operation
            insertCost = matrix[i][j+1] + 3;
            deleteCost = matrix[i+1][j] + 2;
            replaceCost = matrix[i+1][j+1] + ((stringX[i] != stringY[j]) * 4);
            rightCost = matrix[i+1][j+1];
            
            mininumCost = replaceCost;  //default highest cost is replace operation
            
            if (stringX[i] == stringY[j])   {   //if (x == y), right operation
                mininumCost = rightCost;
                operation[i][j] = 0;
            } else {
                mininumCost = replaceCost;  //if (x != y), replace operation
                operation[i][j] = 4;
            }
            
            if (mininumCost > insertCost)   {   //if minimumCost > insertCost, minimumCost is the cost to perform insert operation
                mininumCost = insertCost;
                operation[i][j] = 3;
            }
            
            if (mininumCost > deleteCost)   {   //if minimumCost > deleteCost, minimumCost is the cost to perform delete operation
                mininumCost = deleteCost;
                operation[i][j] = 2;
            }
            matrix[i][j] = mininumCost;
        }
    }
    
    outputSequence(stringX, stringY, operation, m, n);  //output the sequence
    
    for (int i = 0; i < m+1; i++)   {
        free(operation[i]);
    }
    free(operation);
    
    return matrix[0][0];
}
                    
int main(int argc, char* argv[])
{
    int indexOf = 0;
    
    char dummyArray[100];
    char dummyArrayTwo[100];
    
    char *stringX = dummyArray;
    char *stringY = dummyArrayTwo;
    
    string testOne = "";
    string testTwo = "";
    
    int xLen = 0;
    int yLen = 0;
    
    FILE *inputOne, *inputTwo, *inputThree; //file input pointers
    
    if (argc < 3)   {   //if dealing with files
        if ((strcmp(argv[1], "input1")) == 0)  {
            inputOne = fopen("input1.txt", "r");
            fscanf(inputOne, "%d\n", &xLen);
            stringX = (char *)malloc(sizeof(char) * xLen + 1);
            
            for (int i = 0; i < xLen+1; i++)    {
                stringX[i] = fgetc(inputOne);
            }
            
            fscanf(inputOne, "%d\n", &yLen);
            stringY = (char *)malloc(sizeof(char) * yLen + 1);
            
            for (int i = 0; i < yLen+1; i++)    {
                stringY[i] = fgetc(inputOne);
            }
            
            int inputOneCost = editDistance(stringX, stringY, xLen, yLen);
            
            printf("Input 1 Cost: %d\n", inputOneCost);
            fclose(inputOne);
            free(stringX);
            free(stringY);
        } else if ((strcmp(argv[1], "input2")) == 0) {
            inputTwo = fopen("input2.txt", "r");
            fscanf(inputTwo, "%d\n", &xLen);
            stringX = (char *)malloc(sizeof(char) * xLen + 1);
            
            for (int i = 0; i < xLen+1; i++)    {
                stringX[i] = fgetc(inputTwo);
            }
            
            fscanf(inputTwo, "%d\n", &yLen);
            stringY = (char *)malloc(sizeof(char) * yLen + 1);
            
            for (int i = 0; i < yLen+1; i++)    {
                stringY[i] = fgetc(inputTwo);
            }
            
            int inputTwoCost = editDistance(stringX, stringY, xLen, yLen);
            
            printf("Input 2 Cost: %d\n", inputTwoCost);
            fclose(inputTwo);
            free(stringX);
            free(stringY);
        } else if ((strcmp(argv[1], "input3")) == 0) {
            inputThree = fopen("input3.txt", "r");
            fscanf(inputThree, "%d\n", &xLen);
            stringX = (char *)malloc(sizeof(char) * xLen + 1);
            
            for (int i = 0; i < xLen+1; i++)    {
                stringX[i] = fgetc(inputThree);
            }
            
            fscanf(inputThree, "%d\n", &yLen);
            stringY = (char *)malloc(sizeof(char) * yLen + 1);
            
            for (int i = 0; i < yLen+1; i++)    {
                stringY[i] = fgetc(inputThree);
            }
            
            int inputThreeCost = editDistance(stringX, stringY, xLen, yLen);
            
            printf("Input 3 Cost: %d\n", inputThreeCost);
            fclose(inputThree);
            free(stringX);
            free(stringY);
        } else {
            printf("File not found! Please input either input1, input2, or input3\n");
        }
    } else {    //if dealing with inputted string
        for (int i = 0; i < argc; i++) {
            if ((strcmp(argv[i], "to")) == 0)   {   //find the "to" from the arguments
                indexOf = i;
            }
        }
        
        for (int i = 1; i < indexOf; i++)   {   //everything before "to"
            testOne += argv[i];
            testOne += " ";
        }
        
        for (int j = indexOf+1; j < argc; j++)  {   //everything after "to"
            testTwo += argv[j];
            testTwo += " ";
        }
        
        strcpy(stringX, testOne.c_str());   //setting everything before/after "to" to appropriate string
        strcpy(stringY, testTwo.c_str());
        
        //length of strings
        int xLen = strlen(stringX)-1;
        int yLen = strlen(stringY)-1;
        
        int partOneCost = editDistance(stringX, stringY, xLen, yLen);
        
        printf("Part 1 Cost: %d\n", partOneCost);
    }
  return 0;
}
