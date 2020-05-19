#include "../include/parentFunctions.h"



long CreateWorker(long processID, long totalWorkers, PathNode * subDirectoriesPathList)
{
    // array of arguments
    char ** workerArguments;

    pid_t workerPid;

    if( (workerArguments = (char **)malloc(sizeof(char *)*3)) == NULL)
    {
        perror("Error: 1st malloc has been failed from CreateWorker function!");
        exit(EXIT_FAILURE);
    }

    if( (workerArguments[0] = (char *)malloc(sizeof(char)* PROCESSIDSTRING)) == NULL)
    {
        perror("Error: malloc has been failed from CreateWorker function!");
        exit(EXIT_FAILURE);
    }


    // convert processID to string
    sprintf(workerArguments[0], "%ld", processID);


    // Length of list , Number of subdirectories
    long totalSubDirectories = LenOfList(subDirectoriesPathList);

    long i = 0;
    while(i < totalSubDirectories)
    {
        if( (i%totalWorkers) == processID)
        {
            workerArguments[1] = (char *)malloc(1 + sizeof(char)* strlen(GetValue_Path(&subDirectoriesPathList,i) ));
            strcpy(workerArguments[1],GetValue_Path(&subDirectoriesPathList,i));
        }
        i++;
    }

    // Put Null because argv from workers need to know where to stop
    workerArguments[2] = NULL;

    // if child
    if( (workerPid = fork()) == 0)
    {
        execvp("./worker", workerArguments);


    }
    else
    {
        usleep(1);
        for (long j = 0; j < 3; j++)
        {
            free(workerArguments[j]);
        }
        free(workerArguments);
        return workerPid;
    }

}
