#include <stdio.h>
#include <string.h>
#include "phone_book.h"

void flush_in(){
   int ch;
   while( (ch = fgetc(stdin)) != EOF && ch != '\n' ){}
}

void printRecord(record* rec){
   if(rec == NULL) {
      return;
   }
   printf("\nName: %s", rec->name);
   printf("address: %s", rec->address);
   printf("phone: %s\n", rec->phone);
} 

void delete(CLIENT* clnt){
   record rec;
   printf("\nName: ");
   flush_in();
   fgets(rec.name, 50, stdin);

   printf("\nAre you sure you want to delete the record identified by \"%s\"? (Y/N)\n", rec.name);
   char checkChar;
   scanf("%c", &checkChar);
   if(checkChar == 'N' || checkChar == 'n')
      return;

   int* operationStatus;   
   while((operationStatus = delete_1(&rec, clnt)) == NULL)
   
   if(!*operationStatus)
      printf("delete error: record not found.\n");
}

record* read(CLIENT* clnt){
   record rec1;
   printf("\nName: ");
   flush_in();
   fgets(rec1.name, 50, stdin);
   record* rec2;
   while((rec2 = read_1(&rec1, clnt)) == NULL)

   flush_in();
   
   if (!strcmp(rec2->name, "error")){
      printf("read error: record could not be found.\n");
      return NULL;
   }
   return rec2;
}

void update(CLIENT* clnt){
   record* rec = read(clnt);
   if(rec == NULL)
      return;
   
   printf("-------------------------------------\n");
   printf("Current record state:\n");
   printRecord(rec);
   
   int option;
   printf("Choose the update:\n");
   printf("(1) - address.\n");
   printf("(2) - phone.\n");
   printf("(3) - address and phone.\n");
   printf("(4) - cancel update.\n");
   do {
      scanf("%d", &option);
      if (option < 1 || option > 4)
         printf("\nInvalid option. Please try again.\n\n");
      
      else if (option == 4)
         return;
      
      else{
         if(option == 1 || option == 3){
            printf("\naddress: ");
            flush_in();
            fgets(rec->address, 100, stdin);
         }

         if(option == 2 || option == 3){
            printf("\nphone: ");
            // clear buffer only when changing phone number only
            if(option == 2) 
               flush_in();
            fgets(rec->phone, 30, stdin);
         }
      }
   } while(option < 1 || option > 4);

   int* operationStatus;
   while((operationStatus = update_1(rec, clnt)) == NULL)

   if(*operationStatus == 0)
      printf("update error: could not update record.\n");
}

record createRecord(){
   record rec;
   flush_in();
   printf("Name: ");
   fgets(rec.name, 50, stdin);
   printf("address: ");
   fgets(rec.address, 100, stdin);
   printf("phone: ");
   fgets(rec.phone, 30, stdin);
   return rec;
}

void create(CLIENT* clnt){
   int* operationStatus;
   record rec = createRecord();
   
   while((operationStatus = create_1(&rec, clnt)) == NULL)
   
   if(!(*operationStatus))
      printf("create error: could not create new record.\n");
}

void menu(void){
   printf("\n---------------------------------\n");
   printf("----------VW PHONE BOOK----------\n");
   printf("---------------------------------\n");
   printf("\t(1) create new record.\n");
   printf("\t(2) read record.\n");
   printf("\t(3) update record.\n");
   printf("\t(4) delete record.\n");
   printf("\t(5) exit.\n");
   printf("---------------------------------\n");
}

CLIENT* initClient(char* hostname){
   CLIENT *clnt;
   clnt = clnt_create (hostname, PHONE_BOOK, PHONE_BOOK_VERSION, "udp");
   if (clnt == (CLIENT *) NULL){
      clnt_pcreateerror (hostname);
      exit(1);
   }
   return clnt;
}


int main( int argc, char *argv[]) {
   //checking execution parameters...
   if (argc!=2){
      fprintf (stderr,"Usage: %s hostname\n",argv[0]);
      exit (1);
   }

   //creating a client instance...
   CLIENT* clnt = initClient(argv[1]);
   
   int option;
   do {
      menu();
      scanf("%d", &option);
      switch(option){
         case 1:
               create(clnt);
               break;
         case 2: ;
               record* rec;
               rec = read(clnt);
               printRecord(rec);
               break;
         case 3:
               update(clnt);
               break;
         case 4:
               delete(clnt);
               break;
         case 5:
               printf("\nexiting...\n\n");
               break;
         default:
               printf("\nInvalid option. Please try again.\n\n");
      }
    }while(option != 5);

   return (0);
} 