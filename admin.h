#ifndef _admin_h
#define _admin_h


void adminLogin();		 
int adminMenu();	
void registBook();	
BOOK* registInput();
void sort();
void deleteBook();
void indexSort();
void searchBook();
void borrowBook();
BORROW* tempBorrow();
void setDate(int *);
void returnBook();
void clientList(); 

#endif
