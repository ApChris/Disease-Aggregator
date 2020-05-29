#include <stdio.h>
#include "../include/workerFunctions.h"

long fileDescriptorR;
long fileDescriptorW;
long processID;
PathNode * filesPathList = NULL;
Hash * diseaseHash;
Hash * patientHash;

long errorRecords = 0;
long successRecords = 0;
PathNode * countries = NULL;

SumStatistics * generalStatistics;
long buffersize;


void listCountries(char * path)
{

    char messageStatistics[MAXBUFFER];

    sprintf(messageStatistics, "%s %ld", path, (long)getpid());
    WriteToNamedPipe(fileDescriptorW,messageStatistics);

}

void diseaseFrequency(char * arguments)
{
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    char * diseaseID;

    char * tok = strtok(arguments," ");
    if(tok == NULL)
    {
        printf("error\n");
        return ;
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
        return;
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
        tResult = 0;
        long res;
        res = Hash_getPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,"NULL",0);

        char message[MAXBUFFER];

        sprintf(message,"%ld\n", res);
        WriteToNamedPipe(fileDescriptorW,message);

    }
    // user gave a country
    else
    {
        tResult = 0;
        // store country
        char * country;
        country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(country,(const  char *)tok);

        tResult = Hash_getPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,country,1);
        char message[MAXBUFFER];
        sprintf(message,"%ld\n",tResult);
        WriteToNamedPipe(fileDescriptorW,message);
        free(country);
    }
    free(date1);
    free(date2);

}



void topkAgeRanges(char * arguments)
{
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    // store k
    char * k;

    char * tok = strtok(arguments," ");
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

    generalStatistics = SumStatistics_Init();
    Hash_getStatisticsPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,country,generalStatistics);

    char message[MAXBUFFER];
    long totalPatients = generalStatistics -> cases_0_20 + generalStatistics -> cases_21_40 + generalStatistics -> cases_41_60 + generalStatistics -> cases_over_60;
    double percentCase[4];
    percentCase[0] = 100*((double)(generalStatistics -> cases_0_20)/totalPatients);
    percentCase[1] = 100*((double)(generalStatistics -> cases_21_40)/totalPatients);
    percentCase[2] = 100*((double)(generalStatistics -> cases_41_60)/totalPatients);
    percentCase[3] = 100*((double)(generalStatistics -> cases_over_60)/totalPatients);
    if(totalPatients == 0)
    {
        sprintf(message,"Not found any patient!\n");
        WriteToNamedPipe(fileDescriptorW,message);
        free(date1);
        free(date2);
        return;
    }
    long counter = 0;
    long max = -1;
    long position = -1;
    while(counter < atol(k))
    {
        for (long i = 0; i < 4; i++)
        {
            if(percentCase[i] > max)
            {
                max = percentCase[i];
                position = i;
            }
        }

        if(position == 0)
        {
            sprintf(message,"0-20: %0.lf%%\n",percentCase[position]);
            WriteToNamedPipe(fileDescriptorW,message);
        }
        else if(position == 1)
        {
            sprintf(message,"21-40: %0.lf%%\n",percentCase[position]);
            WriteToNamedPipe(fileDescriptorW,message);
        }
        else if(position == 2)
        {
            sprintf(message,"41-60: %0.lf%%\n",percentCase[position]);
            WriteToNamedPipe(fileDescriptorW,message);
        }
        else
        {

            sprintf(message,"60+: %0.lf%%\n",percentCase[position]);
            WriteToNamedPipe(fileDescriptorW,message);
        }
        percentCase[position] = -1;
        counter++;
        max = -1;
        position = -1;
    }

    // printf("\n %0.lf%\n",percentCase60plus);
    //
    // // sprintf(message,"\n0-20: %ld\n20-40: %ld\n41-60: %ld\n60+:%ld\n", (generalStatistics -> cases_0_20/totalPatients)*100, (generalStatistics -> cases_21_40/totalPatients)*100, (generalStatistics -> cases_41_60/totalPatients)*100, (generalStatistics -> cases_over_60/totalPatients)*100);
    // sprintf(message,"%ld \n0-20: %ld\n20-40: %ld\n41-60: %ld\n60+:%ld\n",totalPatients, generalStatistics -> cases_0_20, generalStatistics -> cases_21_40, generalStatistics -> cases_41_60, generalStatistics -> cases_over_60);
    // printf("%ld %ld %ld %ld \n", generalStatistics -> cases_0_20, generalStatistics -> cases_21_40, generalStatistics -> cases_41_60, generalStatistics -> cases_over_60);

    free(date1);
    free(date2);
    return;
}




void searchPatientRecord(char * recordID)
{
    char message[MAXBUFFER];

    PatientInfo * info = Hash_Find_Patient(patientHash,Hash_Function_DJB2((unsigned char *)recordID), recordID);
    PatientInfo_Print(info);
    if(info == NULL)
    {
        sprintf(message, "\nNot Found\n");
        WriteToNamedPipe(fileDescriptorW,message);
    }
    else
    {
        if(info -> exitDate -> day == TAG)
        {
            sprintf(message, "%s %s %s %s %s %ld %ld-%ld-%ld --\n",info -> recordID, info -> patientFirstName, info -> patientLastName,info -> diseaseID, info -> country, info -> age, info -> entryDate -> day, info -> entryDate -> month, info -> entryDate -> year);
        }
        else
        {
            sprintf(message, "%s %s %s %s %s %ld %ld-%ld-%ld %ld-%ld-%ld\n",info -> recordID, info -> patientFirstName, info -> patientLastName,info -> diseaseID, info -> country, info -> age, info -> entryDate -> day, info -> entryDate -> month, info -> entryDate -> year, info -> exitDate -> day, info -> exitDate -> month, info -> exitDate -> year);
        }
        WriteToNamedPipe(fileDescriptorW,message);
    }
}



void numPatientAdmissions(char * arguments)
{
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    char * diseaseID;

    char * tok = strtok(arguments," ");
    if(tok == NULL)
    {
        printf("1error\n");
        return;
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
        return;
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
        tResult = 0;
        long res;
        res = Hash_getPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,"NULL",0);

        char message[MAXBUFFER];

        sprintf(message,"%ld\n", res);
        WriteToNamedPipe(fileDescriptorW,message);

    }
    // user gave a country
    else
    {
        tResult = 0;
        // store country
        char * country;
        country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(country,(const  char *)tok);

        tResult = Hash_getPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,country,1);
        char message[MAXBUFFER];
        sprintf(message,"%ld\n", tResult);
        WriteToNamedPipe(fileDescriptorW,message);
        free(country);
    }
    free(date1);
    free(date2);
    return;
}



void numPatientDischarges(char * arguments)
{
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    char * diseaseID;

    char * tok = strtok(arguments," ");
    if(tok == NULL)
    {
        printf("error\n");
        return;
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
        return;
    }
    // date2
    date2 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> year = (long)atoi(tok);


    tok = strtok(NULL,delimiters);
    // without country
    if(tok == NULL)
    {
        tResult = 0;
        long res;
        res = Hash_getExitPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,"NULL",0);

        char message[MAXBUFFER];

        sprintf(message,"%ld\n", res);
        WriteToNamedPipe(fileDescriptorW,message);

    }
    // user gave a country
    else
    {
        tResult = 0;
        // store country
        char * country;
        country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(country,(const  char *)tok);

        tResult = Hash_getExitPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,country,1);
        char message[MAXBUFFER];
        sprintf(message,"%ld\n", tResult);
        WriteToNamedPipe(fileDescriptorW,message);
        free(country);
    }
    free(date1);
    free(date2);
    return;
}




void ReadingFiles(char * path)
{
    // Get country from subDirectoryPath
    char * country = (char *)malloc(1 + sizeof(char) * strlen(path));
    strcpy(country,path);

    char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
    char * tok = NULL;
    tok = strtok(country, delimiters);
    tok = strtok(NULL, delimiters);
    tok = strtok(NULL, delimiters);
    strcpy(country,tok);
    PushNode_Path(&countries,country);
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

        char currentMinDate[12];

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


    diseaseHash = Hash_Init(11, 512);
    patientHash = Hash_Init(11, 512);
    for (long i = 0; i < LenOfList(filesPathList); i++)
    {

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

        SumStatistics * statistics = FillStructures(GetValue_Path(&filesPathList, i), diseaseHash, patientHash, cdate, country);

        char messageStatistics[MAXBUFFER];

        long flag = 0;
        while(statistics != NULL)
        {
            errorRecords += statistics -> errorRecords;
            successRecords += statistics -> successRecords;
            if(flag == 0)
            {
                sprintf(messageStatistics, "\n%ld-%ld-%ld\n%s\n%s\nAge range 0-20 years: %ld cases\nAge range 21-40 years: %ld cases\nAge range 41-60 years: %ld cases\nAge range 65+ years: %ld cases\n",cdate -> day, cdate -> month, cdate -> year, country, statistics -> diseaseID,statistics -> cases_0_20,statistics -> cases_21_40,statistics -> cases_41_60,statistics -> cases_over_60);
                flag++;
                statistics = statistics -> next;
                WriteToNamedPipe(fileDescriptorW,messageStatistics);
            }
            else
            {
                sprintf(messageStatistics, "\n%s\nAge range 0-20 years: %ld cases\nAge range 21-40 years: %ld cases\nAge range 41-60 years: %ld cases\nAge range 65+ years: %ld cases\n",statistics -> diseaseID,statistics -> cases_0_20,statistics -> cases_21_40,statistics -> cases_41_60,statistics -> cases_over_60);
                statistics = statistics -> next;
                WriteToNamedPipe(fileDescriptorW,messageStatistics);
            }
            // printf("%s\n",messageStatistics);
        }


        free(currentDate);
        free(cdate);
        free(statistics);
    }
    // Hash_Print(diseaseHash);

    // Deallocates
    free(datesArray);
    free(country);

    DeleteList_Path(&filesPathList);
    free(filesPathList);

    // Hash_Deallocate(&diseaseHash,1);
    // free(diseaseHash);

}



void SigHandler()
{


    char buffer[MAXIMUMBUFFER] = "";
    ReadFromNamedPipe(fileDescriptorR, buffer);


    char command[50];
    char * arguments;

    if( (sscanf(buffer, "%49s%m[^\n]", &command, &arguments)) != EOF )
    {
        // // readingFiles(arguments);
        // printf("Command = %s ---> %s\n",command, arguments);
        if(!strcmp(command, "/ReadingFiles"))
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

            ReadingFiles(path);



        }
        else if(!strcmp(command, "/listCountries"))
        {
            char * path;

            char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
            char * tok = NULL;
            tok = strtok(arguments, delimiters);

            tok = strtok(NULL, delimiters);
            tok = strtok(NULL, delimiters);
            path = (char *)malloc(sizeof(char)* strlen(tok));
            strcpy(path,tok);

            listCountries(path);

        }
        else if(!strcmp(command, "/diseaseFrequency"))
        {

            diseaseFrequency(arguments);

        }
        else if(!strcmp(command, "/topk-AgeRanges"))
        {

            topkAgeRanges(arguments);

        }
        else if(!strcmp(command, "/numPatientAdmissions"))
        {
            numPatientAdmissions(arguments);

        }
        else if(!strcmp(command, "/searchPatientRecord"))
        {
            char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
            char * tok = NULL;
            tok = strtok(arguments, delimiters);

            searchPatientRecord(tok);

        }
        else if(!strcmp(command, "/numPatientDischarges"))
        {
            numPatientDischarges(arguments);

        }
        else
        {
            printf("Wrong input\n");
        }


    }

}

void Elimination()
{

    close(fileDescriptorW);
    close(fileDescriptorR);
    PrintList_Path(&countries);
    printf("%ld %ld\n",successRecords,errorRecords);

    FILE * file;
    char fileName[30];

    sprintf(fileName, "etc/logfiles/log_file.%ld.txt", (long)getpid());
    file = fopen(fileName, "w+");

    for (long i = 0; i < LenOfList(countries); i++)
    {
        fprintf(file,"%s\n",GetValue_Path(&countries,i));
        // fputs("asdadaaaa",file);

    }
    fprintf(file, "TOTAL %ld\nSUCCESS %ld\nFAIL %ld\n",successRecords + errorRecords, successRecords, errorRecords);

    fclose(file);
    exit(0);
}

int main(int argc, const char *argv[])
{

    static struct sigaction terminatingAction;
    static struct sigaction answerAction;

    terminatingAction.sa_handler = Elimination;
    sigfillset(&(terminatingAction.sa_mask));
    sigaction(SIGINT, &terminatingAction, NULL);
    sigaction(SIGTERM, &terminatingAction, NULL);
    sigaction(SIGQUIT, &terminatingAction, NULL);

    answerAction.sa_handler = SigHandler;
    sigfillset(&(answerAction.sa_mask));
    sigaction(SIGUSR1, &answerAction, NULL);


    printf("WORKER HEREE!!!!\n");

    if(argc > 0)
    {
        processID = atol(argv[0]);
        buffersize = atol(argv[2]);
        fileDescriptorR = OpenRead(processID);
        fileDescriptorW = OpenWrite(processID);
    }

    for(;;)
    {
        sleep(1);
    }



}
