#include "client.h"
#include "common.h"
#include "admin.h"
#include "function.h"

extern CLIENT** G_client;
extern int G_numOfClient;
extern BOOK** G_book;
extern int G_numOfBook;
extern BORROW** G_borrow;
extern int G_numOfBorrow;

/*------------------------------------
void clientLogin(int n): n��° ȸ���α��� �Լ� 
-------------------------------------*/
void clientLogin(int n){
	char loginPW[20];
	int i, choice, d;
	
	printf("PW�Է�:");
	scanf("%s",loginPW);
	
	if(strcmp(G_client[n]->pw,loginPW)==0){
		printf("%s�� �α��� ����\n",G_client[n]->name);
		while((choice=clientMenu())!=5){//5.�α� �ƿ� 
			if(choice==1){//1.�����˻�
				if(G_numOfBook==0){
					printf("���� �������� ������ �����ϴ�.\n");
				}else{
					searchBook();
				}
			}else if(choice==2){//2.�� �뿩��� 
				borrowList(n);
			}else if(choice==3){//3.�������� ���� 
				editInfo(n);
			}else if(choice==4){//4.ȸ�� Ż�� 
				d = deleteAccount(n);
				if(d==1){
					break;
				}
			}else if(choice==6){//6.���α׷� ���� 
				uploadFile();
				exit(1);
			}else{
				printf("�߸��� �Է�\n");
			}
		}
	}else{
		printf("idȤ�� pw�� �߸��Ǿ����ϴ�.\n");
	}
}

/*------------------------------------
int clientMenu(): ȸ�� �޴� ���� �Լ� 
-------------------------------------*/
int clientMenu(){
	int choice;
	
	printf("1.���� �˻�       2.�� �뿩���\n3.�������� ����   4.ȸ�� Ż��\n5.�α� �ƿ�       6.���α׷� ����\n");
	printf("\n��ȣ�� �Է��ϼ���:");
	scanf("%d",&choice);
	
	return choice;
}

/*------------------------------------
void borrowList(int n): n��° ȸ���� �뿩 ��� �Լ� 
-------------------------------------*/
void borrowList(int n){
	int i, j, cnt;
	cnt = 0;
	printf(">>%s���� �뿩 ���<<\n",G_client[n]->name);
	for(i=0;i<G_numOfBorrow;i++){
		printf("%s\n",G_client[n]->num); 
		if(strcmp(G_client[n]->num,G_borrow[i]->num)==0 && strcmp(G_borrow[i]->returnNum,G_borrow[i]->num)!=0){
			printf("if������");
			cnt++;
			printf("%d\n",cnt);
			printf("|������ȣ:%d ",G_borrow[i]->bk->index);
			printf("|������:%s\n",G_borrow[i]->bk->name);
			printf("�뿩��:");
			for(j=0;j<6;j++){
				printf("-%d-",G_borrow[i]->rentDay[j]);
			}
			printf("�ݳ�����:");
			for(j=0;j<6;j++){
				printf("-%d-",G_borrow[i]->returnDay[j]);
			}
			printf("\n");
			
		}
	}
	if(cnt==0){
		printf("���� �뿩���� ������ �����ϴ�.\n");
	}
	printf("\n"); 
}

/*------------------------------------
void editInfo(int n): n��° ȸ�� �������� �Լ� 
-------------------------------------*/
void editInfo(int n){
	int i, cnt;
	CLIENT* temp;
	cnt = 0;
	temp = G_client[n];
	printf("--���� %s���� ����--\n",temp->name);
	printf("�й�:%s ",temp->num);
	printf("��й�ȣ:%s\n",temp->pw);
	printf("�̸�:%s ",temp->name);
	printf("�ּ�:%s\n",temp->addr);
	printf("��ȭ��ȣ:%s\n",temp->phone);
	printf("--���� ���� �Է�--\n");
	printf("��й�ȣ:");
	scanf("%s",temp->pw);
	printf("�ּ�:");
	fflush(stdin);
	gets(temp->addr);
	while(1){
		printf("��ȭ��ȣ:");
		scanf("%s",temp->phone);
		for(i=0;i<G_numOfClient;i++){
			if(strcmp(G_client[i]->phone,temp->phone)==0){
				cnt++;
				if(i==n){
					cnt--;
					continue;
				}
				printf("�̹� �����ϴ� ��ȭ��ȣ\n");
			}
		}
		if(cnt==0){
			break;
		}
	}
}

/*------------------------------------
int deleteAccount(int n):n��° ȸ�� Ż�� �Լ� 
-------------------------------------*/
int deleteAccount(int n){
	int i, j, choice, cnt, del;
	del = 0;
	cnt = 0;
	while(1){
		printf("���� Ż���Ͻðڽ��ϱ�?\n");
		printf("1.��2.�ƴϿ�\n");
		scanf("%d",&choice);
		if(choice==1){
			for(i=0;i<G_numOfBorrow;i++){
				if(strcmp(G_borrow[i]->num,G_client[n]->num)==0 && strcmp(G_borrow[i]->returnNum,G_borrow[i]->num)!=0){
					printf("count�� ������\n");
					cnt++;
				}
			}
			break;
		}else if(choice!=2){
			printf("�߸��� �Է��Դϴ�.\n");
			continue;
		}
	}
	if(cnt==0){
		if(n<G_numOfClient-1){
			for(j=n;j<G_numOfClient-1;j++){
				G_client[j] = G_client[j+1];
			}
		}
		if(G_numOfClient>1){
			G_client = (CLIENT**)realloc(G_client,(G_numOfClient-1)*sizeof(CLIENT*));
		}
		G_numOfClient--;
		printf("ȸ��Ż�� �Ϸ�Ǿ����ϴ�.\n");
		del = 1;
	}else{
		printf("���� �뿩���� ������ �־ Ż���Ͻ� �� �����ϴ�.\n");
		printf("�뿩���� �������� �ݳ����ּ���.\n");
	}
	
	return del;
}


























