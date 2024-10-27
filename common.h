#ifndef _Library_h
#define _Library_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>

//�� ����ü 
typedef struct client{
	char num[20];	//�й�(ID)
	char pw[20];	//��й�ȣ 
	char name[9];	//�̸� 
	char addr[40];	//�ּ� 
	char phone[13];	//��ȭ��ȣ 
} CLIENT;

//���� ����ü 
typedef struct book{
	int index; 			//������ȣ 
	char name[41];		//������ 
	char publ[21];		//���ǻ�
	char author[31];	//����
	char isbn[11];		//isbn
	char storage[11];	//å��ġ 
	char available;		//�뿩���ɿ��� 
} BOOK; 

//�뿩 ����ü 
typedef struct borrow{
	char num[10];		//�������
	BOOK *bk;			//����(���)
	char returnNum[10];	//�ݳ���
	int ind;			//(��Ͽ뵵����ȣ)
	char BookName[41];	//(��Ͽ뵵����)
	char BookIsbn[11];	//(��Ͽ�isbn)
	int rentDay[6];		//�뿩����
	int returnDay[6];	//�ݳ�����
	 
} BORROW;

#endif






