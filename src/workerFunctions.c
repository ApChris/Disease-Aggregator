#include "../include/workerFunctions.h"




//
long ReadFile(const char * patientRecordsFile, Hash * diseaseHash, Date * date, char * country)
{
    // for getline
    char * line = NULL;
    size_t length = 0;
    long read;
    FILE * file;

    long errorRecords = 0;

    // variables that I read from file
    char * recordID = NULL;
    char * status = NULL;
    char * patientFirstName = NULL;
    char * patientLastName = NULL;
    char * diseaseID = NULL;
    long age = TAG;
    Date * entryDate = NULL;
    Date * exitDate = NULL;

    // for strtok
    char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
    char * tok = NULL;

    // The struct that we are going to fill

    entryDate = malloc(sizeof(*entryDate));
    exitDate = malloc(sizeof(*exitDate));

    file = fopen(patientRecordsFile, "r");
    // printf("%s has been opened successfully!!\n\n", patientRecordsFile); // Feedback

    while((read = getline(&line,&length, file)) != -1)
    {
        PatientInfo * info = NULL;

        // Get recordID
        tok = strtok(line, " ");
        recordID = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(recordID,(const char *)tok);

        // Read status
        tok = strtok(NULL," ");
        status = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(status,(const  char *)tok);


        // Read patientFirstName
        tok = strtok(NULL," ");
        patientFirstName = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(patientFirstName,(const  char *)tok);

        // Read patientLastName
        tok = strtok(NULL," ");
        patientLastName = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(patientLastName,(const  char *)tok);

        // Read diseaseID
        tok = strtok(NULL," ");
        diseaseID = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(diseaseID,(const  char *)tok);

        // Read age
        tok = strtok(NULL," ");
        age = atol(tok);

        printf("--->%s %s %s %s %s %ld\n",recordID,status,patientFirstName,patientLastName,diseaseID,age);


        // if current patient doessn't
        if(!strcmp(status,"ENTER"))
        {
        //     // Flag to fix the print function
            exitDate -> day = TAG;
            entryDate -> day = date -> day;
            entryDate -> month = date -> month;
            entryDate -> year = date -> year;
            info = PatientInfo_Init(recordID,patientFirstName,patientLastName,diseaseID,country, age, entryDate, exitDate);      // create the
            Hash_Insert(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),info);

        //
            free(recordID);
            free(patientFirstName);
            free(patientLastName);
            free(diseaseID);

        //
            continue;
        }


        info = Hash_Find_Patient(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID), recordID);
        if(info == NULL)
        {
            printf("Not found\n");
            printf("RecordID = %s - %s\n",recordID, status);
            errorRecords++;
            free(recordID);
            free(patientFirstName);
            free(patientLastName);
            free(diseaseID);
            continue;
        }
        info -> exitDate -> day = date -> day;
        info -> exitDate -> month = date -> month;
        info -> exitDate -> year = date -> year;


        free(recordID);
        free(patientFirstName);
        free(patientLastName);
        free(diseaseID);


    }
    free(entryDate);
    free(exitDate);
    free(line);
    fclose(file);
    return errorRecords;
}

void Print_Input(char * patientRecordsFile, long diseaseHashtableNumOfEntries, long countryHashtableNumOfEntries, long bucketSize)
{
    printf("\nYour input was:\n\n-> patientRecordsFile = %s\n-> diseaseHashtableNumOfEntries = %ld\n"
    "-> countryHashtableNumOfEntries = %ld\n-> bucketSize = %ld\n",patientRecordsFile,diseaseHashtableNumOfEntries,countryHashtableNumOfEntries,bucketSize);

}
