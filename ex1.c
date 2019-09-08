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
    int i;
    
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
    for (i=0; i<nChild; i++) 
    { 
        //pid = fork();
        //childPid[i] = pid;
    if(fork() == 0) //child
        { 
            sprintf(who, "Child %d [%d]", i+1, getpid());
            readFromFile(who, fd );
            printf( "Parent: %s done\n", who);
            //close (fd);
            //exit(0);

            exit(0); 
        } 
    }
    sprintf(who, "Parent [%d]", getpid());
	readFromFile(who, fd );
    close (fd);

    fd2 = fopen(fileName, "r");
    for (i=0; i<nChild; i++) { 
    	if(fork() == 0) //child
        { 
            sprintf(who, "Child %d [%d]", i+1, getpid());
            readFromFile2(who, fd2);
            printf( "Parent: %s done\n", who);
            //close (fd);
            //exit(0);

            //exit(0); 
        } 
    }

    sprintf(who, "Parent [%d]", getpid());
	readFromFile2(who, fd2 );
    close (fd);


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
				//printf( "EOF\n");
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
    char buffer[5];
    int i;

	//charBuf = 0;
	fread(buffer, 1, 5, fd);
	sprintf(strBuf, "%s: %c\n", who, buffer[0]);
	fwrite(strBuf, strlen(strBuf)+1, 1, stdout);
	for (i = 0; i < 5; i++) {
		usleep (1000);
		fread(buffer+i, 1, 1, fd);
		sprintf(strBuf, "%s: %c\n", who, buffer[i]);
		fwrite(strBuf, strlen(strBuf)+1, 1, stdout);
		//exit(0);
	}

	//exit(0);
}
