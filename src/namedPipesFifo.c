#include "../include/namedPipesFifo.h"

bool CreateNamedPipe_FIFO(long pid, char * flag)
{
    char name[50];
    if(!strcmp(flag,"write"))
    {
        sprintf(name, "%s_%ld_write", FIFOPATH, pid);
    }
    else if(!strcmp(flag,"read"))
    {
        sprintf(name, "%s_%ld_read", FIFOPATH, pid);
    }

    if(mkfifo(name, 0666) < 0)
    {
        if(errno != EEXIST)
        {
            perror("Error:Fifo has not been created!!!");
            exit(EXIT_FAILURE);
        }

    }
    return true;
}

bool UnlinkNamedPipe_FIFO(long pid, char * flag)
{
    char name[50];
    if(!strcmp(flag,"write"))
    {
        sprintf(name, "%s_%ld_write", FIFOPATH, pid);
    }
    else if(!strcmp(flag,"read"))
    {
        sprintf(name, "%s_%ld_read", FIFOPATH, pid);
    }

    if(unlink(name) < 0)
    {
        perror("Error:Fifo has not been unlinked!!!");
        exit(EXIT_FAILURE);
    }

    return true;
}

long OpenRead(long pid)
{
    char name[50];
    long fileDescriptor;
    sprintf(name, "%s_%ld_read", FIFOPATH, pid);
    if( (fileDescriptor = open(name, O_NONBLOCK | O_RDONLY)) < 0)
    {
        perror("Error:OpenRead from Fifo has been failed");
        exit(EXIT_FAILURE);
    }
    return fileDescriptor;
}
