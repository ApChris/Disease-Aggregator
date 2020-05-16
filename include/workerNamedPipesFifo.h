#ifndef __WORKERNAMEDPIPESFIFO_H__
#define __WORKERNAMEDPIPESFIFO_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFOPATH "etc/fifo/myFIFO"

// #define MBUFFER 1000
#define INITCOUNTER 0
#define PROCESSIDSTRING 50

bool UnlinkNamedPipe_FIFO(long pid, char * flag);
long ReadFromNamedPipe(long fileDescriptor, char * buffer, long bufferSize);
void WriteToNamedPipe(long fileDescriptor, char * buffer);
long OpenRead(long pid);
long OpenWrite(long pid);

#endif
