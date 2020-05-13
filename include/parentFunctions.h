#ifndef __PARENTFUNCTIONS_H_
#define __PARENTFUNCTIONS_H_

#include "../include/parentNamedPipesFifo.h"
#include "../include/list.h"
#define PROCESSIDSTRING 50


long CreateWorker(long processID, long totalWorkers, PathNode * subDirectoriesPathList);

#endif
