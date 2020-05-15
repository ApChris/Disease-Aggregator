#include <stdio.h>


#include "../include/workerFunctions.h"


int main(int argc, const char *argv[])
{
    printf("WORKER HEREE!!!!\n");
    for (size_t i = 0; i < argc; i++)
    {
        printf("-->%s\n", argv[i]);
    }

    char * pidStringFormat;
    if( (pidStringFormat = (char *)malloc(sizeof(char)* PROCESSIDSTRING)) == NULL)
    {
        perror("Error: malloc has been failed in worker.c! - WORKER");
        exit(EXIT_FAILURE);
    }
    // convert processID to string
    sprintf(pidStringFormat, "%ld", atol(argv[0]));
    printf("%ld\n",atol(argv[0]));

    // Get country from subDirectoryPath
    // char * country = (char *)malloc(1 + sizeof(char) * strlen(argv[1]));
    // strcpy(country,argv[1]);
    char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
    // char * tok = NULL;
    // tok = strtok(country, delimiters);
    // tok = strtok(NULL, delimiters);
    // tok = strtok(NULL, delimiters);
    // printf("%s\n", tok);


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
        currentDate = malloc(sizeof(char) * strlen(subDirectory -> d_name));
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

        char * currentMinDate[10];

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




    for (long i = 0; i < LenOfList(&filesPathList); i++)
    {
        // ReadFile(const char * patientRecordsFile, Hash * diseaseHash, Date * date, char * country)
    }

    // Get date
    // Date * entryDate;
    // entryDate = (Date *)malloc(sizeof(Date));
    // tok = strtok(NULL,delimiters);
    // entryDate -> day = (long)atoi(tok);
    // tok = strtok(NULL,delimiters);
    // entryDate -> month = (long)atoi(tok);
    // tok = strtok(NULL,delimiters);
    // entryDate -> year = (long)atoi(tok);

    // printf("%ld-%ld-%ld\n",entryDate -> day, entryDate -> month, entryDate -> year);


    // DeleteList_Path(&filesPathList);
    // free(filesPathList);






    long fileDescriptorR = OpenRead(atoi(argv[0]));
    long fileDescriptorW = OpenWrite(atoi(argv[0]));

    for(;;)
    {
        sleep(1);
    }

}
