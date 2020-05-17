#ifndef __WORKERFUNCTIONS_H__
#define __WORKERFUNCTIONS_H__

#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include "workerNamedPipesFifo.h"
#include "list.h"
#include "hash.h"


typedef struct SumStatistics
{
    char * diseaseID;
    long cases_0_20;
    long cases_21_40;
    long cases_41_60;
    long cases_over_60;
}SumStatistics;


long ReadFile(const char * patientRecordsFile, Hash * diseaseHash, Date * date, char * country);

#endif
