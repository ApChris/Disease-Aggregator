#include "../include/parentFunctions.h"


extern Node * writeNamedPipeList;
extern long totalWorkers;
extern long bufferSize;
extern char * path;
extern Node * workersPidList;
extern Node * readNamedPipeList;
extern PathNode * subDirectoriesPathList;
extern bool flagEliminate;

long CreateWorker(long processID)
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
    return workerPid;
}



void Elimination()
{
    printf("Elimination has been activated!\n");
    long currentActiveWorkers;
    currentActiveWorkers = totalWorkers;
    long stat;
    long i = 0;
    flagEliminate = true;
    while(i < totalWorkers)
    {
        close(GetValue(&writeNamedPipeList,i));
        close(GetValue(&readNamedPipeList,i));
        UnlinkNamedPipe_FIFO(i,"main");
        UnlinkNamedPipe_FIFO(i,"secondary");
        kill(GetValue(&workersPidList,i), SIGKILL);
        wait(&stat);
        currentActiveWorkers--;
        printf("Active workers: %ld\n", currentActiveWorkers);
        i++;
    }
    free(path);
    DeleteList_Path(&subDirectoriesPathList);
    DeleteList(&workersPidList);
    DeleteList(&readNamedPipeList);
    DeleteList(&writeNamedPipeList);

}

//
void ReCreateWorker(long signal)
{
    long i = 0;
    if(flagEliminate == true)
    {
        return;
    }
    pid_t pid;
    pid = wait(NULL);
    printf("Worker has been killed : %ld\n", pid);

    while (i < totalWorkers)
    {
        if(GetValue(&workersPidList,i) == pid)
        {

            UpdateNode(&workersPidList, i, CreateWorker(i));
            printf("New worker has been created using pid : %ld\n", GetValue(&workersPidList,i));
            break;
        }
        i++;
    }

}
