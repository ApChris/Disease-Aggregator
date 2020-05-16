#include <stdio.h>


#include "../include/workerFunctions.h"



int main(int argc, const char *argv[])
{
    printf("WORKER HEREE!!!!\n");
    for (size_t i = 0; i < argc; i++)
    {
        printf("-->%s\n", argv[i]);
    }

    // char * pidStringFormat;
    // if( (pidStringFormat = (char *)malloc(sizeof(char)* PROCESSIDSTRING)) == NULL)
    // {
    //     perror("Error: malloc has been failed in worker.c! - WORKER");
    //     exit(EXIT_FAILURE);
    // }
    // // convert processID to string
    // sprintf(pidStringFormat, "%ld", atol(argv[0]));
    // printf("%ld\n",atol(argv[0]));
    //
    // Get country from subDirectoryPath
    char * country = (char *)malloc(1 + sizeof(char) * strlen(argv[1]));
    strcpy(country,argv[1]);
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

    if( ( subDirectoryPointer = opendir(argv[1])) == NULL)
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

        char * name = (char *)malloc(sizeof(char) * strlen(argv[1]) + strlen(currentMinDate) + 2);
        strcpy(name, argv[1]);
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
        // printf("HELLOOO\n\n\n\n\n\n\n\n\n");
        free(currentDate);
        free(cdate);

    }
    // Hash_Print(diseaseHash);
    // printf("\n\n\n\n\n\n\n\nHELLOOO\n");


    // Deallocates
    free(datesArray);
    free(country);

    DeleteList_Path(&filesPathList);
    free(filesPathList);

    Hash_Deallocate(&diseaseHash,1);
    free(diseaseHash);


    // 
    // // Open Read/Write
    // printf("edwwwwww %ld\n",atol(argv[0]));
    //
    // char * str[30];
    // sprintf(str,"Hello world from %ld",(long)getpid());


    if(atol(argv[0]) == 0)
    WriteToNamedPipe(6,str);

    long fileDescriptorR = OpenRead(atol(argv[0]));
    long fileDescriptorW = OpenWrite(atol(argv[0]));



    for(;;)
    {
        sleep(1);
    }



}
