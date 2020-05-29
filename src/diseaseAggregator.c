#include <stdio.h>
#include <dirent.h>

#include "../include/requests.h"

Node * writeNamedPipeList = NULL;
long totalWorkers;
long bufferSize;
char * path = NULL;
Node * workersPidList = NULL;
Node * readNamedPipeList = NULL;
long flagEliminate = 0;
PathNode * subDirectoriesPathList = NULL;



void StartReadingFiles_Workers()
{
    printf("\n\n\n\n\n\n\n\nStart Reading\n");
    char message[MAXIMUMBUFFER];
    printf("Check %ld %ld\n",LenOfList(subDirectoriesPathList), bufferSize);

    for (long i = 0; i < totalWorkers; i++)
    {

        sprintf(message,"/ReadingFiles %ld %s",i,GetValue_Path(&subDirectoriesPathList,i));

        WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);

        kill(GetValue(&workersPidList,i),SIGUSR1);
    }
    sleep(1);

    char result[MAXIMUMBUFFER] = "";
    for (long i = 0; i < totalWorkers; i++)
    {

        long bytes = 0;

        do
        {
            usleep(1);
        }while((bytes = ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);

        printf("%s\n",result);
        printf("--------------------------------------\n");
    }

}




// ./create_infiles.sh ../../etc/diseaseFile.txt ../../etc/countriesFile.txt input_dir 5 400


// /listCountries
// /numPatientAdmissions COVID-2019 10-10-2010 10-10-2020
// /diseaseFrequency COVID-2019 10-10-2010 10-10-2020
// /diseaseFrequency MERS-COV 10-10-2010 10-10-2020
// /diseaseFrequency MERS-COV 10-10-2010 10-10-2020 Greece


// /topk-AgeRanges 3 Greece COVID-2019 10-10-2010 10-10-2020
// /topk-AgeRanges 4 China COVID-2019 10-10-2010 10-10-2020
// /topk-AgeRanges 3 USA COVID-2019 10-10-2010 10-10-2020
// /topk-AgeRanges 4 China MERS-COV 10-10-2010 10-10-2020



int main(int argc, char const *argv[])
{
    static struct sigaction action;

    action.sa_handler = Elimination;
    sigfillset(&(action.sa_mask));
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    // sigaction(SIGKILL, &action, NULL);

    // static struct sigaction workerAction;
    // workerAction.sa_handler = ReCreateWorker;
    // sigfillset(&(workerAction.sa_mask));
    // sigaction(SIGCHLD, &workerAction, NULL);

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

    struct dirent * directory;

    DIR * directoryPointer;

    if( ( directoryPointer = opendir(path)) == NULL)
    {
        perror("Error: Directory has not been opened!");
        exit(EXIT_FAILURE);
    }

    // Open
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

    if(totalWorkers > LenOfList(subDirectoriesPathList))
    {
        long diff = totalWorkers - LenOfList(subDirectoriesPathList);
        totalWorkers -= diff;
    }


    for (long i = 0; i < totalWorkers; i++)
    {
        CreateNamedPipe_FIFO(i,"main");
        CreateNamedPipe_FIFO(i,"secondary");
    }

    // Create Workers
    for(long i = 0; i < totalWorkers; i++)
    {
        PushNode(&workersPidList,CreateWorker(i));
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
    sleep(1);

    StartReadingFiles_Workers();

    printf("Requests\n");

    while(!Read_Requests()){}


    return 0;
}
