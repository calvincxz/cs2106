/*************************************
 * Lab 2 Exercise 1
 * Name:
 * Student No:
 * Lab Group:
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     //for fork(), wait()
#include <string.h>
#include <math.h>

void readFromFile (char who[50], int fd);
void readFromFile2 (char who[50], FILE *fd);

int main(int argc, char **argv) {
    //char *
    char* temp = "env1";
    setenv(temp, "file_abc.txt", 0);
    if (getenv(temp) == NULL) {
        printf("error\n");
    }
    
    return 0;
}   

void readFromFile (char who[50], int fd) 
{
    ssize_t readBytes = 1;
    char charBuf = 0;
    char strBuf[128];

    while (readBytes > 0) {
        usleep (1000);
        charBuf = 0;
        readBytes = read( fd, &charBuf, 1 );
        if( readBytes != 1 ) 
        {
            if( readBytes == 0 ) 
            {
                return;
            }
        }
        sprintf(strBuf, "%s: %c\n", who, charBuf);
        write (STDOUT_FILENO, strBuf, strlen(strBuf));
        //exit(0);
    }
}

void readFromFile2 (char who[50], FILE *fd) 
{
    char strBuf[128];
    char buffer[100];
    char c;

    while( fread(&c, 1, 1, fd) == 1 )
    {
        usleep (1000);
        sprintf(strBuf, "%s: %c\n", who, c);
        fwrite(strBuf, strlen(strBuf)+1, 1, stdout);
    }
    //exit(0);
}