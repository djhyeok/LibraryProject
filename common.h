#ifndef _Library_h
#define _Library_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>

//고객 구조체 
typedef struct client{
	char num[20];	//학번(ID)
	char pw[20];	//비밀번호 
	char name[9];	//이름 
	char addr[40];	//주소 
	char phone[13];	//전화번호 
} CLIENT;

//도서 구조체 
typedef struct book{
	int index; 			//도서번호 
	char name[41];		//도서명 
	char publ[21];		//출판사
	char author[31];	//저자
	char isbn[11];		//isbn
	char storage[11];	//책위치 
	char available;		//대여가능여부 
} BOOK; 

//대여 구조체 
typedef struct borrow{
	char num[10];		//빌린사람
	BOOK *bk;			//도서(운영용)
	char returnNum[10];	//반납자
	int ind;			//(기록용도서번호)
	char BookName[41];	//(기록용도서명)
	char BookIsbn[11];	//(기록용isbn)
	int rentDay[6];		//대여일자
	int returnDay[6];	//반납일자
	 
} BORROW;

#endif






