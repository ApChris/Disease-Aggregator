#include <stdio.h>
#include <dirent.h>


#include "../include/parentFunctions.h"


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
        result = CreateNamedPipe_FIFO(i,"main");
        result = CreateNamedPipe_FIFO(i,"secondary");

        i++;
    }

    PathNode * subDirectoriesPathList = NULL;

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


    Node * workersPidList = NULL;
    for(long i = 0; i < totalWorkers; i++)
    {
        PushNode(&workersPidList,CreateWorker(i, totalWorkers, subDirectoriesPathList));
    }

    // Node * writeNamedPipeList = NULL;
    // i = INITCOUNTER;
    // while (i < totalWorkers)
    // {
    //     PushNode(&writeNamedPipeList,OpenRead(i));
    //     i++;
    // }
    //
    // Node * readNamedPipeList = NULL;
    // i = INITCOUNTER;
    // while (i < totalWorkers)
    // {
    //     PushNode(&readNamedPipeList,OpenRead(i));
    //     i++;
    // }


    printf("Waiting!!!\n");
    sleep(5);

    for (long i = 0; i < totalWorkers; i++)
    {
        result = UnlinkNamedPipe_FIFO(i,"main");
        result = UnlinkNamedPipe_FIFO(i,"secondary");
    }


    free(path);
    DeleteList_Path(&subDirectoriesPathList);
    DeleteList(&workersPidList);

    return 0;
}
