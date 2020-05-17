#include <stdio.h>
#include <dirent.h>


#include "../include/parentFunctions.h"

Node * writeNamedPipeList = NULL;
long totalWorkers;
long bufferSize;
char * path = NULL;
Node * workersPidList = NULL;
Node * readNamedPipeList = NULL;

PathNode * subDirectoriesPathList = NULL;

void StartReadingFiles_Workers()
{
    printf("\n\n\n\n\n\n\n\nStart Reading\n");
    char message[MAXIMUMBUFFER];

    for (size_t i = 0; i < totalWorkers; i++)
    {
        sprintf(message,"/readingFiles %ld %s",i,GetValue_Path(&subDirectoriesPathList,i));
        WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
        kill(GetValue(&workersPidList,i),SIGUSR1);
    }
    sleep(1);
    char result[200];
    int n;
    for (size_t i = 0; i < totalWorkers; i++)
    {
        do
        {
            usleep(1000*100);
            printf("Check3\n");
        }while((n=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);

        printf("\n%s ========= %ld %d\n", result, i, n);
    }

}


int main(int argc, char const *argv[])
{


    if (argc != 7)                          // Check if we have !=7 arguments
    {
      printf("ERROR INPUT!!\nGive for example : ./diseaseAggregator -w 4 -b 50 -i etc/input_dir\n");
      return -1;
    }

    for(int i = 1; i < argc; i++)           // Get arguments
    {
      if(!strcmp(argv[i],"-w"))         // Get number of workers
      {
          totalWorkers = atoi(argv[i+1]);
      }
      else if(!strcmp(argv[i],"-b"))    // Get buffer Size
      {
         bufferSize = atoi(argv[i+1]);

      }
      else if(!strcmp(argv[i],"-i"))    // Path of input _dir
      {
          path = (char *)malloc(sizeof(char)*strlen(argv[i+1])+1);
          strcpy(path,argv[i+1]);
      }

    }

    printf("workers:%lu\nbufferSize:%lu\npath:%s\n",totalWorkers,bufferSize,path);
    bool result;

    for (long i = 0; i < totalWorkers; i++)
    {
        result = CreateNamedPipe_FIFO(i,"main");
        result = CreateNamedPipe_FIFO(i,"secondary");

    }


    // GetListOfSubDirectories(path,subDirectoriesPathList);
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


    // Create Workers
    for(long i = 0; i < totalWorkers; i++)
    {
        PushNode(&workersPidList,CreateWorker(i, totalWorkers, subDirectoriesPathList));
    }
    PrintList(&workersPidList);


    // Open write and read for every worker
    for (long i = 0; i < totalWorkers; i++)
    {
        // Open Write
        long fd = OpenWrite(i);
        PushNode(&writeNamedPipeList,fd);

        // Open Read
        fd = OpenRead(i);
        PushNode(&readNamedPipeList,fd);
    }



    printf("Waiting!!!\n");
    sleep(2);

    StartReadingFiles_Workers();

    printf("Exiting..\n");
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
