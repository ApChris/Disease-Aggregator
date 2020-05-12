#include <stdio.h>
#include <dirent.h>

#include "../include/namedPipesFifo.h"
#include "../include/list.h"

#define PROCESSIDSTRING 50


PathNode * subDirectoriesPathList = NULL;

long CreateWorker(long processID, long totalWorkers)
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

int main(int argc, char const *argv[])
{
    long totalWorkers;
    long bufferSize;
    char * path = NULL;
    if (argc != 7)                          // Check if we have !=7 arguments
    {
      printf("ERROR INPUT!!\nGive for example : ./diseaseAggregator -w 4 -b 50 -i etc/input_dir\n");
      return -1;
    }

    for(int i = 1; i < argc; i++)           // Get arguments
    {
      if(strcmp(argv[i],"-w") == 0)         // Get number of workers
      {
          totalWorkers = atoi(argv[i+1]);
      }
      else if(strcmp(argv[i],"-b") == 0)    // Get buffer Size
      {
         bufferSize = atoi(argv[i+1]);

      }
      else if(strcmp(argv[i],"-i") == 0)    // Path of input _dir
      {
          path = (char *)malloc(sizeof(char)*strlen(argv[i+1])+1);
          strcpy(path,argv[i+1]);
      }

    }

    printf("workers:%lu\nbufferSize:%lu\npath:%s\n",totalWorkers,bufferSize,path);
    bool result;
    long i = INITCOUNTER;
    while (i < totalWorkers)
    {
        result = CreateNamedPipe_FIFO(i,"write");
        result = CreateNamedPipe_FIFO(i,"read");
        i++;
    }

    Node * workersList = NULL;
    for(long i = 0; i < totalWorkers; i++)
    {
            PushNode(&workersList, i);
    }
    printf("Length of list %ld\n", LenOfList(workersList));
    PrintList(&workersList);
    DeleteNode(&workersList, 3);
    printf("Length of list %ld\n", LenOfList(workersList));
    PrintList(&workersList);

    DeleteList(&workersList);



    PushNode_Path(&subDirectoriesPathList, "chris");
    PushNode_Path(&subDirectoriesPathList, "xristoforos");
    PushNode_Path(&subDirectoriesPathList, "farox");

    printf("Length of list %ld\n", LenOfList(subDirectoriesPathList));
    PrintList_Path(&subDirectoriesPathList);
    DeleteNode_Path(&subDirectoriesPathList, "chris");
    printf("Length of list %ld\n", LenOfList(subDirectoriesPathList));
    PrintList_Path(&subDirectoriesPathList);

    DeleteList_Path(&subDirectoriesPathList);



    struct dirent * directory;

    DIR * directoryPointer;

    if( ( directoryPointer = opendir(path)) == NULL)
    {
        perror("Error: Directory has not been opened!");
        exit(EXIT_FAILURE);
    }

    while( ( directory = readdir(directoryPointer)) != NULL)
    {
        if((!strcmp(directory -> d_name, ".")) || (!strcmp(directory -> d_name, "..")))
        {
            continue;
        }
        printf("%s\n", directory -> d_name);
        char * name = (char *)malloc(sizeof(char) * strlen(path) + strlen(directory -> d_name) + 2);
        strcpy(name, path);
        strcat(name, "/");
        strcat(name, directory -> d_name);

        PushNode_Path(&subDirectoriesPathList,name);
        free(name);
    }
    closedir(directoryPointer);
    PrintList_Path(&subDirectoriesPathList);

    long x =CreateWorker(0, totalWorkers);

        free(path);
        DeleteList_Path(&subDirectoriesPathList);



    // FILE * directoryFilePointer;
    // if( (directoryFilePointer = fopen(path,"r")) == NULL)
    // {
    //     perror("File has not been opened! diseaseAggregator.c");
    //     exit(EXIT_FAILURE);
    //
    // }
    //
    //
    // char * subdirectoryPath;
    // long res;
    // while( (res = fscanf(directoryFilePointer,"%ms",&subdirectoryPath)) != EOF)
    // {
    //     if(res != 1)
    //     {
    //         perror("Error in fscanf diseaseAggregator.c!");
    //         exit(EXIT_FAILURE);
    //     }
    //     else
    //     {
    //         PushNode_Path(&subDirectoriesPathList,subdirectoryPath);
    //     }
    // }

    // PrintList_Path(&pathList);
    // fclose(directoryFilePointer);
    // i = INITCOUNTER;
    // while (i < workers)
    // {
    //     printf("%ld\n",OpenWrite(i));
    //     i++;
    // }
    //
    // i = INITCOUNTER;
    // while (i < workers)
    // {
    //     result = OpenRead(i);
    //     i++;
    // }
    //
    // for (long i = 0; i < workers; i++)
    // {
    //     result = UnlinkNamedPipe_FIFO(i,"write");
    //     result = UnlinkNamedPipe_FIFO(i,"read");
    // }

    // static struct sigaction act;
    // static struct sigaction childAct;
    // act.sa_handler = terminate;






    return 0;
}
