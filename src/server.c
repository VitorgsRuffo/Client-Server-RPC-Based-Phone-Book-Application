#include <stdio.h>
#include "phone_book.h"

int * create_1_svc (record *argp, struct svc_req *rqstp){
    int* operationResult = (int*) malloc(sizeof(int));
    
    FILE* database;
    database = fopen("database.bin", "ab");
    if(database != NULL)
        if(fwrite(argp, sizeof(record), 1, database) == 1)
            *operationResult = 1;
    fclose(database);

    return (operationResult);
}

record * read_1_svc (record *argp, struct svc_req *rqstp){
    record* rec = (record*) malloc(sizeof(record));
    strcpy(rec->name, "error");

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

int * update_1_svc (record *argp, struct svc_req *rqstp){

    printf("%s\n%s\n%s\n", argp->name, argp->address, argp->phone);
    
    // pegar contato do parametro
    // trocar as informações dele de acordo com o que veio no segundo parametro
    // retornar o contato original
}

int* delete_1_svc (record *argp, struct svc_req *rqstp){

}