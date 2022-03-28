#include <stdio.h>
#include "phone_book.h"

int * create_1_svc (record *argp, struct svc_req *rqstp){
    int* operationResult = (int*) malloc(sizeof(int));
    *operationResult = 0;

    if (argp == NULL || rqstp == NULL) return operationResult;

    FILE* database;
    
    database = fopen("database.bin", "ab");
    if (database == NULL) return operationResult;
    fclose(database);

    database = fopen("database.bin", "r+b");
    if(database == NULL)
        return operationResult;
        
    //calculating database size:
    long databaseSizeInBytes;
    fseek(database, 0, SEEK_END);
    databaseSizeInBytes = ftell(database);
    rewind(database);

    long databaseSizeInRecords = databaseSizeInBytes / sizeof(record);

    //allocating memory to contain whole database in memory:
    //(for the cases where the database is greater than memory this obviously is not going to work)
    record* buffer;
    buffer = (record*) malloc(sizeof(record)*databaseSizeInRecords);
    if (buffer == NULL) {
        fclose(database);
        return operationResult;
    }

    //copying the file (database) into the memory buffer:
    if(fread(buffer, sizeof(record), databaseSizeInRecords, database) != databaseSizeInRecords)
        return operationResult;

    //iterating through buffer in order to find the requested record: 
    //(This is not the most efficient way to do this. Implement the database with a hashfile would make searching faster.)
    for(int i = 0; i<databaseSizeInRecords; i++){
        if(!strcmp(buffer[i].name, "invalid")){
            fseek(database, i*sizeof(record), SEEK_SET);
            fwrite(argp, sizeof(buffer[i]), 1, database);
            fclose(database);
            *operationResult = 1;
            return (operationResult);
        }
    }

    if(fwrite(argp, sizeof(record), 1, database) == 1) {
        fclose(database);
        *operationResult = 1;
    }

    return (operationResult);
}

record * read_1_svc (record *argp, struct svc_req *rqstp){
    record* rec = (record*) malloc(sizeof(record));
    strcpy(rec->name, "error");

    if (argp == NULL || rqstp == NULL) return rec;

    FILE* database;
    database = fopen("database.bin", "rb");
    if(database == NULL)
        return rec;

    //calculating database size:
    long databaseSizeInBytes;
    fseek(database, 0, SEEK_END);
    databaseSizeInBytes = ftell(database);
    rewind(database);

    long databaseSizeInRecords = databaseSizeInBytes / sizeof(record);

    //allocating memory to contain whole database in memory:
    //(for the cases where the database is greater than memory this obviously is not going to work)
    record* buffer;
    buffer = (record*) malloc(sizeof(record)*databaseSizeInRecords);
    if (buffer == NULL) {
        fclose(database);
        return rec;
    }

    //copying the file (database) into the memory buffer:
    if(fread(buffer, sizeof(record), databaseSizeInRecords, database) != databaseSizeInRecords)
        return rec;

    //iterating through buffer in order to find the requested record: 
    //(This is not the most efficient way to do this. Implement the database with a hashfile would make searching faster.)
    for(int i = 0; i<databaseSizeInRecords; i++){
        if(!strcmp(buffer[i].name, argp->name)){
            memcpy(rec, &buffer[i], sizeof(record));
            break;
        }
    }
    
    fclose(database);
    return rec;
}

int* update_1_svc (record *argp, struct svc_req *rqstp){
    
    int* operationStatus = (int*) malloc(sizeof(int));
    *operationStatus = 0;

    if (argp == NULL || rqstp == NULL) return operationStatus;

    FILE* database;
    database = fopen("database.bin", "r+b");
    if(database == NULL)
        return operationStatus;

    //calculating database size:
    long databaseSizeInBytes;
    fseek(database, 0, SEEK_END);
    databaseSizeInBytes = ftell(database);
    rewind(database);

    long databaseSizeInRecords = databaseSizeInBytes / sizeof(record);

    //allocating memory to contain whole database in memory:
    //(for the cases where the database is greater than memory this obviously is not going to work)
    record* buffer;
    buffer = (record*) malloc(sizeof(record)*databaseSizeInRecords);
    if (buffer == NULL) {
        fclose(database);
        return operationStatus;
    }

    //copying the file (database) into the memory buffer:
    if(fread(buffer, sizeof(record), databaseSizeInRecords, database) != databaseSizeInRecords)
        return operationStatus;

    //iterating through buffer in order to find the requested record: 
    for(int i = 0; i<databaseSizeInRecords; i++){
        if(!strcmp(buffer[i].name, argp->name)){
            fseek(database, i*sizeof(record), SEEK_SET);
            fwrite(argp->name, sizeof(buffer[i].name), 1, database);
            fwrite(argp->address, sizeof(buffer[i].address), 1, database);
            fwrite(argp->phone, sizeof(buffer[i].phone), 1, database);
            break;
        }
    }
    
    fclose(database);
    *operationStatus = 1;
    return operationStatus;
}

int* delete_1_svc (record *argp, struct svc_req *rqstp){

    int* operationStatus = (int*) malloc(sizeof(int));
    *operationStatus = 0;

    if (argp == NULL || rqstp == NULL) return operationStatus;

    FILE* database;
    database = fopen("database.bin", "r+b");
    if(database == NULL)
        return operationStatus;

    //calculating database size:
    long databaseSizeInBytes;
    fseek(database, 0, SEEK_END);
    databaseSizeInBytes = ftell(database);
    rewind(database);

    long databaseSizeInRecords = databaseSizeInBytes / sizeof(record);

    //allocating memory to contain whole database in memory:
    //(for the cases where the database is greater than memory this obviously is not going to work)
    record* buffer;
    buffer = (record*) malloc(sizeof(record)*databaseSizeInRecords);
    if (buffer == NULL) {
        fclose(database);
        return operationStatus;
    }

    //copying the file (database) into the memory buffer:
    if(fread(buffer, sizeof(record), databaseSizeInRecords, database) != databaseSizeInRecords)
        return operationStatus;

    //iterating through buffer in order to find the requested record: 
    for(int i = 0; i<databaseSizeInRecords; i++){
        if(!strcmp(buffer[i].name, argp->name)){
            record invalidRec;
            strcpy(invalidRec.name, "invalid"); strcpy(invalidRec.address, "invalid"); strcpy(invalidRec.phone, "invalid");
            fseek(database, i*sizeof(record), SEEK_SET);
            fwrite(invalidRec.name, sizeof(buffer[i].name), 1, database);
            fwrite(invalidRec.address, sizeof(buffer[i].address), 1, database);
            fwrite(invalidRec.phone, sizeof(buffer[i].phone), 1, database);
            break;
        }
    }
    
    fclose(database);
    *operationStatus = 1;
    return operationStatus;
}