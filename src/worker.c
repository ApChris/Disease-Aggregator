#include <stdio.h>
#include "../include/workerFunctions.h"

long fileDescriptorR;
long fileDescriptorW;
long processID;

void readingFiles(char * procID, char * path)
{
    printf("Hello from readingfiles %s %s\n", procID, path);

    // Get country from subDirectoryPath
    char * country = (char *)malloc(1 + sizeof(char) * strlen(path));
    strcpy(country,path);

    char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
    char * tok = NULL;
    tok = strtok(country, delimiters);
    tok = strtok(NULL, delimiters);
    tok = strtok(NULL, delimiters);
    strcpy(country,tok);

    // Read files from subDirectory
    PathNode * filesPathList = NULL;

    Date * datesArray = malloc(sizeof(*datesArray));

    struct dirent * subDirectory;

    DIR * subDirectoryPointer;

    if( ( subDirectoryPointer = opendir(path)) == NULL)
    {
        perror("Error: SubDirectory has not been opened! - WORKER");
        exit(EXIT_FAILURE);
    }

    long counter = 0;
    char * currentDate = NULL;
    while( ( subDirectory = readdir(subDirectoryPointer)) != NULL)
    {

        if((!strcmp(subDirectory -> d_name, ".")) || (!strcmp(subDirectory -> d_name, "..")))
        {
            continue;
        }
        currentDate = malloc(1 + sizeof(char) * strlen(subDirectory -> d_name));
        strcpy(currentDate,subDirectory -> d_name);
        char * tok = NULL;
        if(counter == 0) // only for first file
        {

            tok = strtok(currentDate, delimiters);
            datesArray[0].day = atol(tok);
            tok = strtok(NULL, delimiters);
            datesArray[0].month = atol(tok);
            tok = strtok(NULL, delimiters);
            datesArray[0].year = atol(tok);
            counter++;
            free(currentDate);
            continue;
        }
        if( (datesArray = realloc(datesArray, sizeof(*datesArray)*(counter + 1)) ) == NULL)
        {
            perror("Error: Realloc has been failed!- WORKER");
            exit(EXIT_FAILURE);
        }

        tok = strtok(currentDate, delimiters);

        datesArray[counter].day = atol(tok);

        tok = strtok(NULL, delimiters);
        datesArray[counter].month = atol(tok);
        tok = strtok(NULL, delimiters);
        datesArray[counter].year = atol(tok);

        counter++;
        free(currentDate);

    }
    closedir(subDirectoryPointer);


    // Sort dates and push them in a list
    long min = 0;
    for (long i = 0; i < counter; i++)
    {
        for (long j = 0; j < counter; j++)
        {
            if(j+1 <= counter)
            {
                long result = Compare_Date(&datesArray[j],&datesArray[min]);
                if(result == -1)
                {
                    min = j;
                }
            }

        }

        char * currentMinDate[12];

        sprintf(currentMinDate, "%ld-%ld-%ld", datesArray[min].day, datesArray[min].month, datesArray[min].year);

        datesArray[min].year = TAG;
        min = 0;

        char * name = (char *)malloc(sizeof(char) * strlen(path) + strlen(currentMinDate) + 2);
        strcpy(name, path);
        strcat(name, "/");
        strcat(name, currentMinDate);

        PushNode_Path(&filesPathList,name);
        free(name);
    }

    Reverse_Path(&filesPathList);
    PrintList_Path(&filesPathList);


    Hash * diseaseHash = Hash_Init(11, 512);

    for (long i = 0; i < LenOfList(filesPathList); i++)
    {
        // printf("%s\n",GetValue_Path(&filesPathList, i));
        char * currentDate = malloc(1 + sizeof(char) * strlen(GetValue_Path(&filesPathList, i)));
        strcpy(currentDate,GetValue_Path(&filesPathList, i));
        Date * cdate = malloc(sizeof(*cdate));
        char * tok = NULL;
        tok = strtok(currentDate, delimiters);
        tok = strtok(NULL, delimiters);
        tok = strtok(NULL, delimiters);

        // day
        tok = strtok(NULL, delimiters);

        cdate -> day = atol(tok);

        tok = strtok(NULL, delimiters);
        cdate -> month = atol(tok);
        tok = strtok(NULL, delimiters);
        cdate -> year = atol(tok);


        long errorRecords = ReadFile(GetValue_Path(&filesPathList, i), diseaseHash, cdate, country);
        WriteToNamedPipe(fileDescriptorW,"Hello from readingfiles");
        free(currentDate);
        free(cdate);
    }
    // Hash_Print(diseaseHash);

    // Deallocates
    free(datesArray);
    free(country);

    DeleteList_Path(&filesPathList);
    free(filesPathList);

    Hash_Deallocate(&diseaseHash,1);
    free(diseaseHash);



    //
}


void SigHandler(long a)
{
    char buffer[MAXIMUMBUFFER];
    ReadFromNamedPipe(fileDescriptorR, buffer);
    //printf("Command is: %s\n", buffer);
    printf("----------------------------------------------------> %s   %d\n", buffer, getpid());
    char command[15];
    char * arguments, * query;


    // char * answer = (char *)malloc(sizeof(char) * 100);
    // sprintf(answer, "This is my answer from %d worker",getpid());
    // WriteToNamedPipe(fileDescriptorW,answer);
    //
    // sprintf(answer, "This is my answer from %d- worker",getpid());
    // WriteToNamedPipe(fileDescriptorW,answer);
    //
    // sprintf(answer, "This is my answer from %d-- worker",getpid());
    // WriteToNamedPipe(fileDescriptorW,answer);
    // free(answer);

    printf("BUFFER = %s\n",buffer);
    //
    // readingFiles("Hello");
    if( (sscanf(buffer, "%14s%m[^\n]", &command, &arguments)) != EOF )
    {
        // readingFiles(arguments);
        printf("Command = %s, ---> %s\n",command, arguments);
        if(!strcmp(command, "/readingFiles"))
        {
            // Preprocess arguments that are going to be send
            char * path;
            char * procID;
            char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
            char * tok = NULL;
            tok = strtok(arguments, delimiters);
            procID = (char *)malloc(sizeof(char)* strlen(tok));
            strcpy(procID,tok);
            tok = strtok(NULL, " \n");
            path = (char *)malloc(sizeof(char)* strlen(tok));
            strcpy(path,tok);

            if (arguments < 0)
            {
                printf("Error in input!\n");
                printf("\nWaiting....\n");
                exit(EXIT_FAILURE);
            }
            readingFiles(procID, path);
            printf("Edwww %s %s\n", procID, path);
            // sscanf(args, "%m[^\n]", &query);
            // if (!query)
            // {
            //     printf("Error in input\n");
            //     exit(EXIT_FAILURE);
            // }
            //
            // search(query);

        }
        // else if(strcmp(command, "/maxcount") == 0)
        // {
        //     sscanf(args, "%ms", &query);
        //     maxcount(query);
        //     free(query);
        //
        // }
        // else if(strcmp(command, "/mincount") == 0)
        // {
        //     sscanf(args, "%ms", &query);
        //     mincount(query);
        //     free(query);
        //
        // }
        // else if(strcmp(command, "/wc") == 0)
        // {
        //     wc();
        // }
    }
}

void Terminating(int signal){
    close(fileDescriptorW);
    close(fileDescriptorR);
    // fclose(logsFile);
    // delete files_trie;
    // delete file_paths;
    // delete lines;
    exit(0);
}

int main(int argc, const char *argv[])
{


    static struct sigaction terminatingAction;
    static struct sigaction answerAction;

    terminatingAction.sa_handler = Terminating;
    sigfillset(&(terminatingAction.sa_mask));
    sigaction(SIGINT, &terminatingAction, NULL);
    sigaction(SIGTERM, &terminatingAction, NULL);

    answerAction.sa_handler= SigHandler;
    sigfillset(&(answerAction.sa_mask));
    sigaction(SIGUSR1, &answerAction, NULL);
    printf("WORKER HEREE!!!!\n");

    // // Get country from subDirectoryPath
    // char * country = (char *)malloc(1 + sizeof(char) * strlen(argv[1]);
    // strcpy(country,argv[1]);
    // char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
    // char * tok = NULL;
    // tok = strtok(country, delimiters);
    // tok = strtok(NULL, delimiters);
    // tok = strtok(NULL, delimiters);
    // strcpy(country,tok);
    //
    //
    // // Read files from subDirectory
    // PathNode * filesPathList = NULL;
    //
    // Date * datesArray = malloc(sizeof(*datesArray));
    //
    // struct dirent * subDirectory;
    //
    // DIR * subDirectoryPointer;
    //
    // if( ( subDirectoryPointer = opendir(argv[1])) == NULL)
    // {
    //     perror("Error: SubDirectory has not been opened! - WORKER");
    //     exit(EXIT_FAILURE);
    // }
    //
    // long counter = 0;
    // char * currentDate = NULL;
    // while( ( subDirectory = readdir(subDirectoryPointer)) != NULL)
    // {
    //
    //     if((!strcmp(subDirectory -> d_name, ".")) || (!strcmp(subDirectory -> d_name, "..")))
    //     {
    //         continue;
    //     }
    //     currentDate = malloc(1 + sizeof(char) * strlen(subDirectory -> d_name));
    //     strcpy(currentDate,subDirectory -> d_name);
    //     char * tok = NULL;
    //     if(counter == 0) // only for first file
    //     {
    //
    //         tok = strtok(currentDate, delimiters);
    //         datesArray[0].day = atol(tok);
    //         tok = strtok(NULL, delimiters);
    //         datesArray[0].month = atol(tok);
    //         tok = strtok(NULL, delimiters);
    //         datesArray[0].year = atol(tok);
    //         counter++;
    //         free(currentDate);
    //         continue;
    //     }
    //     if( (datesArray = realloc(datesArray, sizeof(*datesArray)*(counter + 1)) ) == NULL)
    //     {
    //         perror("Error: Realloc has been failed!- WORKER");
    //         exit(EXIT_FAILURE);
    //     }
    //
    //     tok = strtok(currentDate, delimiters);
    //
    //     datesArray[counter].day = atol(tok);
    //
    //     tok = strtok(NULL, delimiters);
    //     datesArray[counter].month = atol(tok);
    //     tok = strtok(NULL, delimiters);
    //     datesArray[counter].year = atol(tok);
    //
    //     counter++;
    //     free(currentDate);
    //
    // }
    // closedir(subDirectoryPointer);
    //
    //
    //
    //
    //
    //
    //
    //
    //
    // // Sort dates and push them in a list
    // long min = 0;
    // for (long i = 0; i < counter; i++)
    // {
    //     for (long j = 0; j < counter; j++)
    //     {
    //         if(j+1 <= counter)
    //         {
    //             long result = Compare_Date(&datesArray[j],&datesArray[min]);
    //             if(result == -1)
    //             {
    //                 min = j;
    //             }
    //         }
    //
    //     }
    //
    //     char * currentMinDate[12];
    //
    //     sprintf(currentMinDate, "%ld-%ld-%ld", datesArray[min].day, datesArray[min].month, datesArray[min].year);
    //
    //     datesArray[min].year = TAG;
    //     min = 0;
    //
    //     char * name = (char *)malloc(sizeof(char) * strlen(argv[1]) + strlen(currentMinDate) + 2);
    //     strcpy(name, argv[1]);
    //     strcat(name, "/");
    //     strcat(name, currentMinDate);
    //
    //     PushNode_Path(&filesPathList,name);
    //     free(name);
    // }
    //
    // Reverse_Path(&filesPathList);
    // PrintList_Path(&filesPathList);
    //
    //
    // Hash * diseaseHash = Hash_Init(11, 512);
    //
    // for (long i = 0; i < LenOfList(filesPathList); i++)
    // {
    //     // printf("%s\n",GetValue_Path(&filesPathList, i));
    //     char * currentDate = malloc(1 + sizeof(char) * strlen(GetValue_Path(&filesPathList, i)));
    //     strcpy(currentDate,GetValue_Path(&filesPathList, i));
    //     Date * cdate = malloc(sizeof(*cdate));
    //     char * tok = NULL;
    //     tok = strtok(currentDate, delimiters);
    //     tok = strtok(NULL, delimiters);
    //     tok = strtok(NULL, delimiters);
    //
    //     // day
    //     tok = strtok(NULL, delimiters);
    //
    //     cdate -> day = atol(tok);
    //
    //     tok = strtok(NULL, delimiters);
    //     cdate -> month = atol(tok);
    //     tok = strtok(NULL, delimiters);
    //     cdate -> year = atol(tok);
    //
    //
    //     long errorRecords = ReadFile(GetValue_Path(&filesPathList, i), diseaseHash, cdate, country);
    //     // printf("HELLOOO\n\n\n\n\n\n\n\n\n");
    //     free(currentDate);
    //     free(cdate);
    //
    // }
    // // Hash_Print(diseaseHash);
    // // printf("\n\n\n\n\n\n\n\nHELLOOO\n");
    //
    //
    // // Deallocates
    // free(datesArray);
    // free(country);
    //
    // DeleteList_Path(&filesPathList);
    // free(filesPathList);
    //
    // Hash_Deallocate(&diseaseHash,1);
    // free(diseaseHash);


    processID = atol(argv[0]);
    fileDescriptorR = OpenRead(processID);
    fileDescriptorW = OpenWrite(processID);



    for(;;)
    {
        sleep(1);
    }



}
