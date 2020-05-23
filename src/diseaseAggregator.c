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
        sprintf(message,"/ReadingFiles %ld %s",i,GetValue_Path(&subDirectoriesPathList,i));
        WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
        kill(GetValue(&workersPidList,i),SIGUSR1);
    }
    sleep(1);
    char result[512];
    int bytes;
    for (size_t i = 0; i < totalWorkers; i++)
    {
        do
        {
            usleep(100*1000);
        }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);
        printf("%s\n",result);
    }
}

void Request_1()
{

    char message[MAXIMUMBUFFER];

    for (size_t i = 0; i < totalWorkers; i++)
    {
        // printf("pid = %ld\n", GetValue(&workersPidList,i));
        sprintf(message,"/listCountries %s",GetValue_Path(&subDirectoriesPathList,i));
        WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
        kill(GetValue(&workersPidList,i),SIGUSR1);

    }

    char result[512];
    int bytes;
    for (size_t i = 0; i < totalWorkers; i++)
    {
        do
        {
            usleep(100000);

        }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);
    }

}

bool Request_2(char * tok)
{
    char message[MAXIMUMBUFFER];
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    char * diseaseID;

    tok = strtok(NULL," ");
    if(tok == NULL)
    {
        printf("error\n");
        return true;
    }
    diseaseID = ( char *)malloc(1 + sizeof(char) * strlen(tok));
    strcpy(diseaseID,(const  char *)tok);


    Date * date1 = NULL;
    Date * date2 = NULL;
    date1 = malloc(sizeof(*date1));
    date2 = malloc(sizeof(*date2));

    // // date1
    tok = strtok(NULL, delimiters);

    date1 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> year = (long)atoi(tok);

    // date2
    tok = strtok(NULL,delimiters);

    if(tok == NULL)
    {
        free(date1);
        free(date2);
        printf("error\n");
        return true;
    }
    // date2
    date2 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> year = (long)atoi(tok);
// diseaseFrequency H1N1 10-10-2010 20-20-2020 Greece

    tok = strtok(NULL,delimiters);
    // without country

    if(tok == NULL)
    {

        for (size_t i = 0; i < totalWorkers; i++)
        {

            sprintf(message,"/diseaseFrequency %s %ld-%ld-%ld %ld-%ld-%ld",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year);

            WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
            kill(GetValue(&workersPidList,i),SIGUSR1);
        }
    }
    else
    {
        // store country
        char * country;
        country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(country,(const  char *)tok);

        for (size_t i = 0; i < totalWorkers; i++)
        {

            char * path = malloc(sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)) + 1);
            strcpy(path,GetValue_Path(&subDirectoriesPathList,i));
            path = strtok(tok,delimiters);
            path = strtok(tok,delimiters);
            path = strtok(tok,delimiters);

            if(!strcmp(path,country))
            {
                sprintf(message,"/diseaseFrequency %s %ld-%ld-%ld %ld-%ld-%ld %s",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year, country);

                WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
                kill(GetValue(&workersPidList,i),SIGUSR1);

            }

        }
    }

    free(date1);
    free(date2);


    char result[512];
    int bytes;
    long res = 0;
    for (size_t i = 0; i < totalWorkers; i++)
    {
        do
        {
            usleep(100000);

        }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);
        res += atol(result);

    }
    printf("%ld\n",res);
}

bool Request_3(char * tok)
{
    char message[MAXIMUMBUFFER];
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    // store k
    char * k;
    tok = strtok(NULL," ");
    k = ( char *)malloc(1 + sizeof(char) * strlen(tok));
    strcpy(k,(const  char *)tok);

    // store country
    tok = strtok(NULL," ");
    char * country;
    country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
    strcpy(country,(const  char *)tok);

    // store diseaseID
    char * diseaseID;
    tok = strtok(NULL," ");
    diseaseID = ( char *)malloc(1 + sizeof(char) * strlen(tok));
    strcpy(diseaseID,(const  char *)tok);


    Date * date1 = NULL;
    Date * date2 = NULL;
    date1 = malloc(sizeof(*date1));
    date2 = malloc(sizeof(*date2));

    // // date1
    tok = strtok(NULL, delimiters);
    date1 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> year = (long)atoi(tok);

    // date2
    tok = strtok(NULL,delimiters);
    date2 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> year = (long)atoi(tok);
    //
    //

    for (size_t i = 0; i < totalWorkers; i++)
    {
        tok = NULL;
        char * currentCountry = (char *)malloc(1 + sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)));
        strcpy(currentCountry, GetValue_Path(&subDirectoriesPathList,i));

        tok = strtok(currentCountry, delimiters);
        tok = strtok(NULL, delimiters);
        tok = strtok(NULL, delimiters);
        if(!country)
        {
            printf("edw\n");
        }
        else
        {
            if(!strcmp(country,tok))
            {
                sprintf(message,"/topk-AgeRanges %s %s %s %ld-%ld-%ld %ld-%ld-%ld",k, country, diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year);

                WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
                kill(GetValue(&workersPidList,i),SIGUSR1);
            }
        }

    }
    //
    //
    //
    //
    // free(date1);
    // free(date2);
    //

    char result[MAXBUFFER];
    int bytes;
    long res = 0;
    for (long i = 0; i < totalWorkers; i++)
    {
        char * currentCountry = (char *)malloc(1 + sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)));
        strcpy(currentCountry, GetValue_Path(&subDirectoriesPathList,i));

        tok = strtok(currentCountry, delimiters);
        tok = strtok(NULL, delimiters);
        tok = strtok(NULL, delimiters);

            if(!strcmp(country,tok))
            {
                do
                {
                    usleep(100000);

                }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);

                printf("%s\n",result);
            }


    }



    // printf("%ld\n",res);
}

// /topk-AgeRanges 3 China COVID-2019 10-10-2010 10-10-2020

void Request_4(char * recordID)
{

    char message[MAXIMUMBUFFER];

    for (size_t i = 0; i < totalWorkers; i++)
    {
        sprintf(message,"/searchPatientRecord %s",recordID);
        WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
        kill(GetValue(&workersPidList,i),SIGUSR1);

    }

    char result[512];
    int bytes;
    for (size_t i = 0; i < totalWorkers; i++)
    {
        do
        {
            usleep(100000);

        }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);
        if(bytes > 0)
        {
            printf("%s\n", result);
        }

    }

}


bool Request_5(char * tok)
{
    char message[MAXIMUMBUFFER];
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    char * diseaseID;

    tok = strtok(NULL," ");
    if(tok == NULL)
    {
        printf("error\n");
        return true;
    }
    diseaseID = ( char *)malloc(1 + sizeof(char) * strlen(tok));
    strcpy(diseaseID,(const  char *)tok);


    Date * date1 = NULL;
    Date * date2 = NULL;
    date1 = malloc(sizeof(*date1));
    date2 = malloc(sizeof(*date2));

    // // date1
    tok = strtok(NULL, delimiters);

    date1 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> year = (long)atoi(tok);

    // date2
    tok = strtok(NULL,delimiters);

    if(tok == NULL)
    {
        free(date1);
        free(date2);
        printf("error\n");
        return true;
    }
    // date2
    date2 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> year = (long)atoi(tok);


    tok = strtok(NULL,delimiters);
    // without country
    char * country = NULL;

    if(tok == NULL)
    {

        for (size_t i = 0; i < totalWorkers; i++)
        {

            sprintf(message,"/numPatientAdmissions %s %ld-%ld-%ld %ld-%ld-%ld",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year);

            WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
            kill(GetValue(&workersPidList,i),SIGUSR1);
        }
    }
    else
    {
        // store country

        country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(country,(const  char *)tok);

        for (size_t i = 0; i < totalWorkers; i++)
        {

            char * path = malloc(sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)) + 1);
            strcpy(path,GetValue_Path(&subDirectoriesPathList,i));
            path = strtok(tok,delimiters);
            path = strtok(tok,delimiters);
            path = strtok(tok,delimiters);

            if(!strcmp(path,country))
            {
                sprintf(message,"/numPatientAdmissions %s %ld-%ld-%ld %ld-%ld-%ld %s",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year, country);

                WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
                kill(GetValue(&workersPidList,i),SIGUSR1);

            }

        }
    }

    free(date1);
    free(date2);


    char result[512];
    int bytes;
    long res = 0;
    for (size_t i = 0; i < totalWorkers; i++)
    {
        do
        {
            usleep(100000);

        }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);
        res += atol(result);
        char * tok = NULL;
        char * currentCountry = (char *)malloc(1 + sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)));
        strcpy(currentCountry, GetValue_Path(&subDirectoriesPathList,i));

        tok = strtok(currentCountry, delimiters);
        tok = strtok(NULL, delimiters);
        tok = strtok(NULL, delimiters);
        if(!country)
        {
            printf("%s %ld\n",tok, atol(result));
        }
        else
        {
            if(!strcmp(country,tok))
            {
                printf("%s %ld\n",tok, atol(result));
            }
        }

    }

}



bool Request_6(char * tok)
{
    char message[MAXIMUMBUFFER];
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    char * diseaseID;

    tok = strtok(NULL," ");
    if(tok == NULL)
    {
        printf("error\n");
        return true;
    }
    diseaseID = ( char *)malloc(1 + sizeof(char) * strlen(tok));
    strcpy(diseaseID,(const  char *)tok);


    Date * date1 = NULL;
    Date * date2 = NULL;
    date1 = malloc(sizeof(*date1));
    date2 = malloc(sizeof(*date2));

    // // date1
    tok = strtok(NULL, delimiters);

    date1 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> year = (long)atoi(tok);

    // date2
    tok = strtok(NULL,delimiters);

    if(tok == NULL)
    {
        free(date1);
        free(date2);
        printf("error\n");
        return true;
    }
    // date2
    date2 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> year = (long)atoi(tok);


    tok = strtok(NULL,delimiters);
    // without country
    char * country = NULL;

    if(tok == NULL)
    {

        for (size_t i = 0; i < totalWorkers; i++)
        {

            sprintf(message,"/numPatientAdmissions %s %ld-%ld-%ld %ld-%ld-%ld",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year);

            WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
            kill(GetValue(&workersPidList,i),SIGUSR1);
        }
    }
    else
    {
        // store country

        country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(country,(const  char *)tok);

        for (size_t i = 0; i < totalWorkers; i++)
        {

            char * path = malloc(sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)) + 1);
            strcpy(path,GetValue_Path(&subDirectoriesPathList,i));
            path = strtok(tok,delimiters);
            path = strtok(tok,delimiters);
            path = strtok(tok,delimiters);

            if(!strcmp(path,country))
            {
                sprintf(message,"/numPatientAdmissions %s %ld-%ld-%ld %ld-%ld-%ld %s",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year, country);

                WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
                kill(GetValue(&workersPidList,i),SIGUSR1);

            }

        }
    }

    free(date1);
    free(date2);


    char result[512];
    int bytes;
    long res = 0;
    for (size_t i = 0; i < totalWorkers; i++)
    {
        do
        {
            usleep(100000);

        }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);
        res += atol(result);
        char * tok = NULL;
        char * currentCountry = (char *)malloc(1 + sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)));
        strcpy(currentCountry, GetValue_Path(&subDirectoriesPathList,i));

        tok = strtok(currentCountry, delimiters);
        tok = strtok(NULL, delimiters);
        tok = strtok(NULL, delimiters);
        if(!country)
        {
            printf("%s %ld\n",tok, atol(result));
        }
        else
        {
            if(!strcmp(country,tok))
            {
                printf("%s %ld\n",tok, atol(result));
            }
        }

    }

}



static long Read_Requests_Parse(char * request)
{

    char * tok;
    char delimiters[] = " \n\t\r\v\f\n:,.><[]{}|=+*@#$";
    tok = strtok(request,delimiters);

    if(tok != NULL)
    {
        if(!strcmp(tok,"/listCountries"))
        {
            Request_1();

            return false;

        }
        else if(!strcmp(tok,"/diseaseFrequency"))
        {
            Request_2(tok);
            return false;

        }
        else if(!strcmp(tok,"/topk-AgeRanges"))
        {
            Request_3(tok);
            return false;

        }
        else if(!strcmp(tok,"/searchPatientRecord"))
        {
            tok = strtok(NULL, delimiters);
            Request_4(tok);
            return false;
        }
        else if(!strcmp(tok,"/numPatientAdmissions"))
        {
            Request_5(tok);
            return false;
        }
        else if(!strcmp(tok,"/numPatientDischarges"))
        {
            Request_6(tok);
            return false;
        }
        // /diseaseFrequency COVID-2019 10-10-2010 10-10-2020
        // /numPatientAdmissions COVID-2019 10-10-2010 10-10-2020
        // else if(strcmp(tok,"/numCurrentPatients") == 0)
        // {
        //     Request_7(diseaseHash,tok);
        //
        //     return false;
        // }
        else if(!strcmp(tok,"/exit"))
        {
            printf("exiting\n");
            return true;

        }
        return false;

    }
    return true;

}

// /topk-AgeRanges 10 Greece COVID-2019 10-10-2010 10-10-2020

long Read_Requests()
{
    char * request = NULL;
    size_t length;
    long read;
    while((read = getline(&request,&length, stdin)) != -1)
    {
        if(Read_Requests_Parse(request))
        {
            free(request);
            break;
        }
    }
    return -1;
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

    printf("Requests\n");

    while(!Read_Requests()){}

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
