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
int isValidPath(char *str1, char *str2);
int hasDoubleQuote(char *str);
char* removeQuote(char *c);


int main() {

    char* input;
    size_t bufsize = 200;
    int* tokenNum;
    char **tokenStrArr;
    input = malloc(200);
    //read user input
    printf("GENIE > ");
    input = (char *)malloc(bufsize * sizeof(char));
    getline(&input,&bufsize,stdin);

    while(strcmp(input,"quit\n") != 0) {
        tokenNum = malloc(sizeof(int));
        tokenStrArr = readTokens(10, 20, tokenNum, input);
        if (isValidPath("", tokenStrArr[0]) == 0) {  //command has valid path
            int pid = fork(); // Create a new process
            if (pid != 0) { // parent
                wait(NULL);
            } else {
                if(execv(tokenStrArr[0], tokenStrArr) == -1){ 
                    fprintf(stderr,"%s not found\n", tokenStrArr[0]);
                }
            }
        } else {  //command path is invalid
            char temp[200];
            strcpy(temp,"/bin/");
            strcat(temp, tokenStrArr[0]); //append command to /bin/
            int pid = fork(); 
            if (pid != 0) { // parent
                wait(NULL); // wait for child
            } else { // child
                if (isValidPath("", temp) != 0) { // /bin/command is still invalid
                    fprintf(stderr,"%s not found\n", temp);
                    exit(0);
                }
                execvp(temp, tokenStrArr); 
            }
        }
        free(tokenNum);
        free(tokenStrArr);
        free(input);
        printf("GENIE > ");
        input = (char *)malloc(bufsize * sizeof(char));
        getline(&input,&bufsize,stdin);
    }

    free(input);

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
        //token = trim(token);
        if (hasDoubleQuote(token)) {
            strncpy(tokenStrArr[i], removeQuote(token), maxTokenSize - 1);
        } else {
           strncpy(tokenStrArr[i], token, maxTokenSize - 1); 
        }

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

int isValidPath(char *str1, char *str2) {
    struct stat *buf;
    buf = malloc(sizeof(struct stat));
    char temp[100];
    strcpy(temp, str1);
    strcat(temp, str2);
    //printf("%s\n", temp);
    int i = stat(temp, buf);
    free(buf);
    return i;
}

int hasDoubleQuote(char *str) {
    char c[100];
    strcpy(c, str);
    int len = strlen(c);
    if (c[0] == '\"' && c[len-1] == '\"') {
        
        return 1;
    } else {
        return 0;
    }
}

char* removeQuote(char *c) {
    int i;
    char* str2 = malloc(200);
    char str[100];
    strcpy(str, c);
    int len = strlen(c);
    for(i=1;i<len-1;i++)
        {
            str[i-1]=str[i];
        }
    str[i-1]='\0';
    str2 = str;
    return str2;
}