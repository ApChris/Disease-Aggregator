#ifndef __NAMEDPIPESFIFO_H__
#define __NAMEDPIPESFIFO_H__

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

bool CreateNamedPipe_FIFO(long pid, char * flag);
bool UnlinkNamedPipe_FIFO(long pid, char * flag);
#endif
