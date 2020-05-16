#include "../include/workerNamedPipesFifo.h"


bool UnlinkNamedPipe_FIFO(long pid, char * flag)
{
    char name[50];
    if(!strcmp(flag,"main"))
    {
        sprintf(name, "%s_%ld_main", FIFOPATH, pid);
    }
    else if(!strcmp(flag,"secondary"))
    {
        sprintf(name, "%s_%ld_secondary", FIFOPATH, pid);
    }

    if(unlink(name) < 0)
    {
        perror("Error:Fifo has not been unlinked -> WORKER!!!");
        exit(EXIT_FAILURE);
    }

    return true;
}

long ReadFromNamedPipe(long fileDescriptor, char * buffer, long bufferSize)
{
    long bytesNumber;
    if( (bytesNumber = read(fileDescriptor, buffer, bufferSize)) >= 0)
    {
        buffer[bytesNumber] = '\0';
        return bytesNumber;
    }
    return -1;
}

void WriteToNamedPipe(long fileDescriptor, char * buffer)
{

    if(write(fileDescriptor, buffer, strlen(buffer)) < 0)
    {
        perror("ERROR:WriteToNamedPipe has been failed -> WORKER");
        exit(EXIT_FAILURE);
    }
}

long OpenRead(long pid)
{
    char name[50];
    long fileDescriptor;
    sprintf(name, "%s_%ld_main", FIFOPATH, pid);
    if( (fileDescriptor = open(name, O_NONBLOCK | O_RDONLY)) < 0)
    {
        perror("Error:OpenRead from Fifo has been failed -> WORKER");
        exit(EXIT_FAILURE);
    }
    return fileDescriptor;
}


long OpenWrite(long pid)
{
    char name[50];
    long fileDescriptor;
    sprintf(name, "%s_%ld_secondary", FIFOPATH, pid);
    if( (fileDescriptor = open(name, O_WRONLY)) < 0)
    {
        perror("Error:OpenRead from Fifo has been failed -> WORKER");
        exit(EXIT_FAILURE);
    }
    return fileDescriptor;
}
