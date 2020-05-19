#ifndef __WORKERFUNCTIONS_H__
#define __WORKERFUNCTIONS_H__

#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include "workerNamedPipesFifo.h"
#include "list.h"
#include "hash.h"



SumStatistics * FillStructures(const char * patientRecordsFile, Hash * diseaseHash, Date * date, char * country);

#endif
