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
void PipeTwo (char** command, int size);
char *ltrim(char *s);
char *rtrim(char *s);
char *trim(char *s);

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

int main() {

    char* input;
    size_t bufsize = 2000;
    int* tokenNum;
    int* commandNum;
    struct stat *buf; 
    char **tokenStrArr;
    char **command;
    //read user input
    printf("GENIE > ");
    input = (char *)malloc(bufsize * sizeof(char));
    getline(&input,&bufsize,stdin);

    while(strcmp(input,"quit") != 0) {

        //printf("%s\n", input);
        buf = malloc(sizeof(struct stat));
        tokenNum = malloc(sizeof(int));
        commandNum = malloc(sizeof(int));
        command = readTokens(10, 200, commandNum, input, "\\|");
        //tokenStrArr = readTokens(10, 200, tokenNum, input, " \n");
        if (*commandNum == 1) {
            tokenStrArr = readTokens(10, 20, tokenNum, command[0], " \n");
        //printf("aaa");
        //printf("%s\n", tokenStrArr[0]);
            int i = stat(tokenStrArr[0], buf);
            if (i == 0) {
                //printf("%s\n", tokenStrArr[0]);
                int pid = fork(); // Create a new process
                if (pid != 0) { // parent
                    //int *status;
                    //waitpid(pid, status, WNOHANG); // Wait for process termination
                    wait(NULL);
                    //char *input = (char*)malloc(200);
                } else {
                    if(execv(tokenStrArr[0], tokenStrArr) == -1){ 
                        fprintf(stderr,"%s not found\n", tokenStrArr[0]);
                    }
                    //execv(tokenStrArr[0], tokenStrArr[0], tokenStrArr[1], tokenStrArr[2], tokenStrArr[3], tokenStrArr[4], NULL);
                }
            } else {
                char temp[] = "/bin/";
                strcat(temp, tokenStrArr[0]);
                int pid = fork(); // Create a new process
                if (pid != 0) { // parent
                    //int s;
                    wait(NULL); // Wait for child
                } else {
                    //execvp(temp,tokenStrArr);
                    if (stat(temp,buf) != 0) {
                        fprintf(stderr,"%s not found\n", temp);
                        exit(0);
                    }
                    if(execvp(temp, tokenStrArr) == -1){ 
                        fprintf(stderr,"%s not found\n", temp);
                        exit(0);
                    }
                    //execvp(temp, tokenStrArr[0], tokenStrArr[1], tokenStrArr[2], tokenStrArr[3], tokenStrArr[4], NULL);
                }
            }
            free(buf);
            free(tokenNum);
            free(tokenStrArr);
        } else {  //more than 1 command
            //printf("commadnum%d", *commandNum); 
            PipeTwo(command, *commandNum);
        }

        free(commandNum);
        free(input);
        input = (char *)malloc(bufsize * sizeof(char));
        printf("GENIE > ");
        getline(&input,&bufsize,stdin);
    }
    
    printf("Goodbye!\n");
    return 0;
    
}

void PipeTwo(char** command, int size) {
    pid_t pid;
    int* tokenNum1 = malloc(sizeof(int));
    int maxPipeNum = (size-1);
    int fd[2 * maxPipeNum];

    for (int i = 0; i < maxPipeNum; i++) {
        pipe(fd + 2*i);
    }

      // create pipe1
    for (int i = 0; i < size; i++) {
        
        char** command1 = readTokens(10, 20, tokenNum1, command[i], " \n");
        //pipe(fd + 2*i);

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
            execvp(command1[0], command1);


        } 
    }
    //parent reaches here
    for  (int i = 0; i < 2* size; i++) {
        close(fd[i]);
    }

    for  (int i = 0;  i < size; i++) {
        //printf("aaa");
        wait(NULL);
    }

    free(tokenNum1);


      // parent
      // close unused fds
    

    /*for (count = 0; count < size - 1; count++) {

    char** command1 = readTokens(10, 20, tokenNum1, command[count], " \n");
    char** command2 = readTokens(10, 20, tokenNum2, command[count+1], " \n");
    pid = fork();

    if(pid==0) //child
    {
        if (count == 0) {
            dup2(fd[2*count+1], STDOUT_FILENO);
            close(fd[2*count+1]);
            close(fd[2*count]);
            if (execvp(command1[0], command1) == -1)
        fprintf(stderr, "Failed to execute '%s'\n", "ls");
        exit(1);
        } else {
            dup2(fd[2*count], STDIN_FILENO);
            close(fd[2*count+1]);
            close(fd[2*count]);
        }
        
        
        //printf
        
    }
    else
    { 
        pid=fork();
        if(pid==0)
        {
            dup2(fd[2*count + 2], STDIN_FILENO);
            //execlp("wc", "wc", "-l",(char*) NULL);
            if (count == size-2) {
                close(fd[2*count+1]);
                close(fd[2*count]);
                execvp(command2[0], command2);
            } else {

                close(fd[2*count]);
                dup2(fd[2*count+1], STDOUT_FILENO);
                close(fd[2*count+1]);
                execvp(command2[0], command2);
            }
            
            
        }
        else
        {
            int i;
            int status;
            for(i = 0; i < size; i++){
                   close(fd[i]);
                }
            waitpid(pid, &status, 0);
        }
    }
    }*/
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