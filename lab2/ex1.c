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
#include <sys/wait.h>
#include <unistd.h>     //for fork(), wait()
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void readFromFile (char who[50], int fd);
void readFromFile2 (char who[50], FILE *fd);

int main(int argc, char **argv) {
    char* fileName = NULL;
    int nChild = 0;
    int fd = 0;
    FILE *fd2;
    char who[50];
    int pid;
    int i;
    char charBuf = 0;
    char strBuf[128];
    
    if (argc < 3)
    {
        printf ("Usage: %s <filename> <number_of_processes>\n", argv[0]);
        return 1;
    }
    
    fileName = argv[1];
    nChild = atoi(argv[2]);

    fd = open( fileName, O_RDONLY ); 
    //TODO check the error code
    //TODO add your code
    sprintf(who, "Parent [%d]", getpid());
    read(fd, &charBuf, 1 );
    sprintf(strBuf, "%s: %c\n", who, charBuf);
    write (STDOUT_FILENO, strBuf, strlen(strBuf));
    for (i=0; i < nChild; i++) {
        pid = fork();
        if (pid == 0) {
            break;
        }
    }

    if (pid == 0) {
        sprintf(who, "Child %d [%d]", i+1, getpid());
        readFromFile(who, fd );
        printf( "Parent: %s done\n", who);
        exit(0);
        
    } else {
        
        readFromFile(who, fd );
        close (fd);
    }

    // part2 using fopen,fread..
    for (i=0; i < nChild; i++) {
        pid = fork();
        fd2 = fopen(fileName, "r"); 
        if (pid == 0) {
            break;
        }
    }

    if (pid == 0) {
        sprintf(who, "Child %d [%d]", i+1, getpid());
        readFromFile2(who, fd2);
        usleep(5000);
        printf( "Parent: %s done\n", who);
        exit(0);
    } else {
        sprintf(who, "Parent [%d]", getpid());
        readFromFile2(who, fd2);
        while (wait(NULL) > 0);
        fclose (fd2);
        printf("Parent: Exiting\n");
    }

    
    
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
