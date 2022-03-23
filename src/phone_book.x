/* phone_book.x : interface definition */

#define PROGRAM_NUMBER 1111111
#define VERSION_NUMBER 1

/* remote procedure parameter expected data type */

struct record
{
  char name[50];
  char address[100];
  char phone[30];
};


program PHONE_BOOK
{
   version PHONE_BOOK_VERSION
   {
     int CREATE (record) = 1;
     record READ (char*) = 2;
     int UPDATE (record) = 3;
     int DELETE (char*) = 4;
   }
   = VERSION_NUMBER;
}
= PROGRAM_NUMBER;
