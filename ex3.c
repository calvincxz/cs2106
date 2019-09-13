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
#include <ctype.h>

#define READ_END 0
#define WRITE_END 1

char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer, char* delim);
void freeTokenArray(char** strArr, int size);
int PipeTwo (char** command, int size);
char *ltrim(char *s);
char *rtrim(char *s);
char *trim(char *s);
int isValidPath(char *str1, char *str2);


int main() {

    char* input;
    size_t bufsize = 2000;
    int* tokenNum;
    int* commandNum;
    struct stat *buf; 
    char **tokenStrArr;
    char **command;
    char* inputCopy;
    int len;
    //read user input
    printf("GENIE > ");
    input = (char *)malloc(bufsize * sizeof(char));
    inputCopy = (char *)malloc(bufsize * sizeof(char));
    getline(&input,&bufsize,stdin);
    strcpy(inputCopy, input); //inputcopy for error output msg
    len = strlen(inputCopy);
    if( inputCopy[len-1] == '\n' ) {
        inputCopy[len-1] = 0;
    }

    while(strcmp(input,"quit\n") != 0) {

        buf = malloc(sizeof(struct stat));
        tokenNum = malloc(sizeof(int));
        commandNum = malloc(sizeof(int));
        command = readTokens(10, 200, commandNum, input, "\\|");
        if (*commandNum == 1) {
            tokenStrArr = readTokens(10, 20, tokenNum, command[0], " \n");
            if (isValidPath("",tokenStrArr[0]) == 0) {
                int pid = fork(); // Create a new process
                if (pid != 0) { // parent
                    wait(NULL);
                } else {
                    if(execv(tokenStrArr[0], tokenStrArr) == -1){ 
                        fprintf(stderr,"%s not found\n", tokenStrArr[0]);
                        exit(0);
                    }
                }
            } else {
                char temp[] = "/bin/";
                strcat(temp, tokenStrArr[0]);
                int pid = fork(); // Create a new process
                if (pid != 0) { // parent
                    wait(NULL); // Wait for child
                } else {
                    if (isValidPath("", temp) != 0) {
                        fprintf(stderr,"%s not found\n", temp);
                        exit(0);
                    }
                    if(execvp(temp, tokenStrArr) == -1){ 
                        fprintf(stderr,"%s not found\n", temp);
                        exit(0);
                    }
                }
            }
            free(buf);
            free(tokenNum);
            free(tokenStrArr);
        } else {  //more than 1 command
            //printf("commadnum%d", *commandNum); 
            if (PipeTwo(command, *commandNum) == 1) {
                fprintf(stderr,"%s not found\n", inputCopy);
            }
        }
        free(commandNum);
        free(input);
        free(inputCopy);
        input = (char *)malloc(bufsize * sizeof(char));
        inputCopy = (char *)malloc(bufsize * sizeof(char));
        printf("GENIE > ");
        getline(&input,&bufsize,stdin);
            strcpy(inputCopy, input); //inputcopy for error output msg
        len = strlen(inputCopy);
        if( inputCopy[len-1] == '\n' ) {
            inputCopy[len-1] = 0;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
    
}

int PipeTwo(char** command, int size) {
    pid_t pid;
    int* tokenNum1 = malloc(sizeof(int));
    int maxPipeNum = (size-1);
    int fd[2 * maxPipeNum];
    int *status;

    for (int i = 0; i < maxPipeNum; i++) {
        pipe(fd + 2*i);
    }

    for (int i = 0; i < size; i++) {
        char** command1 = readTokens(10, 20, tokenNum1, command[i], " \n");
        if (isValidPath("/bin/", command1[0]) != 0 
            && isValidPath("/usr/bin/", command1[0]) != 0 ) {
            for  (int i = 0; i < 2* size; i++) {
                close(fd[i]);
            }
            for  (int j = 0;  j < i; j++) {
                wait(NULL);
            }

            return 1;
        }

        if ((pid = fork()) == -1) {
            perror("bad fork1");

        } else if (pid == 0) { // main frame for child
            if (i != 0) { //if not firstcommand
                dup2(fd[2*i - 2], 0);
            }

            if (i != size-1) { // if not last command
                dup2(fd[2*i + 1], 1);
            }
            for  (int i = 0; i < 2* size; i++) {
                close(fd[i]);
            }

            if (execvp(command1[0], command1) == -1) {
                exit(1);
            }
            
        }
    } // end of for loop

    //parent reaches here
    for  (int i = 0; i < 2* size; i++) {
        close(fd[i]);
    }

    for  (int i = 0;  i < size; i++) {
        wait(NULL);
    }

    free(tokenNum1);
    return 0;

}

char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer, char *delim)
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

    token = strtok(buffer, delim);
    
    i = 0;
    while (i < maxTokenNum && (token != NULL)) {
         //Allocate space for token string
        tokenStrArr[i] = (char*) malloc(sizeof(char*) * maxTokenSize);

        //Ensure at most 19 + null characters are copied
        strncpy(tokenStrArr[i], trim(token), maxTokenSize - 1);

        //Add NULL terminator in the worst case
        tokenStrArr[i][maxTokenSize-1] = '\0';
        
        i++;
        token = strtok(NULL, delim);
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



char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}

int isValidPath(char *str1, char *str2) {
    struct stat *buf;
    buf = malloc(sizeof(struct stat));
    char temp[100];
    strcpy(temp, str1);
    strcat(temp, str2);
    //printf("%s\n", temp);
    int i = stat(temp, buf);
    return i;
}