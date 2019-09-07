/*************************************
 * Lab 2 Exercise 2
 * Name: Calvin
 * Student No: A0190624H
 * Lab Group: 4
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer);
void freeTokenArray(char** strArr, int size);
int* readTokenNum;
char** tokenStrArr;
int commandFound;
struct stat buf;

int main() {
    char* input;
    scanf("%s", input);
    while(strcmp(input,"quit")!= 0) {
        tokenStrArr = readTokens(10, 20, readTokenNum, input);
        if (strchr(tokenStrArr[0],'/') != NULL) { //found in bin
            if (stat(strcat("/bin/",tokenStrArr[0]), &buf) == 0) {
                if (fork() == 0) {
                    execl(strcat("/bin/",tokenStrArr[0]), tokenStrArr[0], NULL);
                }
                wait(NULL);
            } else {
                printf(strcat(tokenStrArr[0], "not found"));
            }

        } else if(stat(tokenStrArr[0], &buf) == 0) { //command path found

        } else {
            printf(strcat(tokenStrArr[0], "not found"));

        }
        scanf("%s", input);
    }
    printf("Goodbye!");

    return 0;



    
}



char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer)
//Tokenize buffer
//Assumptions:
//  - the tokens are separated by " " and ended by newline
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - should use the freeTokenArray to free memory after use!
{
    char** tokenStrArr;
    char* token;
    int i;

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum);
    
    //Nullify all entries
    for (i = 0; i < maxTokenNum; i++) {
        tokenStrArr[i] = NULL;
    }

    token = strtok(buffer, " \n");
    
    i = 0;
    while (i < maxTokenNum && (token != NULL)) {
         //Allocate space for token string
        tokenStrArr[i] = (char*) malloc(sizeof(char*) * maxTokenSize);

        //Ensure at most 19 + null characters are copied
        strncpy(tokenStrArr[i], token, maxTokenSize - 1);

        //Add NULL terminator in the worst case
        tokenStrArr[i][maxTokenSize-1] = '\0';
        
        i++;
        token = strtok(NULL, " \n");
    }
    
    *readTokenNum = i;
    
    return tokenStrArr;
}

void freeTokenArray(char** tokenStrArr, int size) {
    int i = 0;

    //Free every string stored in tokenStrArr
    for (i = 0; i < size; i++) {
        if (tokenStrArr[i] != NULL) {
            free(tokenStrArr[i]);
            tokenStrArr[i] = NULL;
        }
    }
    //Free entire tokenStrArr
    free(tokenStrArr);

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}


