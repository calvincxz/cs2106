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


int main() {

    char *input = (char*)malloc(200);
    int* tokenNum;
    struct stat *buf; 
    char **tokenStrArr;
    

    //read user input
    printf("GENIE > ");
    scanf("%s", input);
    while(strcmp(input,"quit") != 0) {

        //Handle 'R' Request
        buf = malloc(sizeof(struct stat));
        tokenNum = malloc(sizeof(int));
        tokenStrArr = readTokens(10, 19, tokenNum, input);
        //printf("aaa");
        //printf("%s", tokenStrArr[0]);
        //freeTokenArray(arr, *tokenNum);
        //printf("stat: %d", stat(tokenStrArr[0], buf));
        int i = stat(tokenStrArr[0], buf);
        if ( i == 0) {
        	printf("inside if statement\n");
        	printf("%s", tokenStrArr[0]);
        	printf("%s", tokenStrArr[1]);
        	printf("%s", tokenStrArr[2]);
            int pid = fork(); // Create a new process
            if (pid != 0) { // parent
                wait(NULL); // Wait for process termination
                //free(input);
                //char *input = (char*)malloc(200);
            } else {
                execl(tokenStrArr[0], tokenStrArr[0], tokenStrArr[1], tokenStrArr[2], tokenStrArr[3], tokenStrArr[4], NULL);
                printf("inside else");
                exit(0);
            }
        } else {
            char temp[100] = "/bin/";
            strcat(temp, tokenStrArr[0]);
            tokenStrArr[0] = temp;
            //printf("%s\n", tokenStrArr[0]);
            int pid = fork(); // Create a new process
            if (pid != 0) { // parent
                //int s;
                wait(NULL); // Wait for process termination
            } else {
                    //if(execvp(tokenStrArr[0], tokenStrArr) == -1){ 
            	execl(temp, tokenStrArr[0], tokenStrArr[1], tokenStrArr[2], tokenStrArr[3], tokenStrArr[4], NULL);
                    //printf("%s not found", tokenStrArr[0]); // Display error message
                    exit(0);
            }
        }
        free(buf);
        free(tokenNum);
        free(tokenStrArr);
        printf("GENIE > ");
        scanf("%s", input);
    }
    
    printf("Goodbye!\n");
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
    for (int i = 0; i < maxTokenNum; i++) {
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


