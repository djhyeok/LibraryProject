#include "common.h"
#include "admin.h"
#include "function.h"

extern CLIENT** G_client;	 
extern int G_numOfClient;	 
extern BOOK** G_book;		 
extern int G_numOfBook;		 
extern BORROW** G_borrow;	 
extern int G_numOfBorrow;	

typedef enum searchType{Bname=1,Bpubl,Bisbn,Bauthor,Ball,Bexit} SEARCHTYPE;	//�˻� Ÿ�� enum 

/*------------------------------------
void adminLogin(): �����ڷα����Լ� 
-------------------------------------*/ 
void adminLogin(){
	char loginPW[20];
	int choice;	
	
	//�ӽ� ���ڿ��� pw���� 
	printf("PW�Է�:");
	scanf("%s",loginPW);
	
	//������ �α��� PW�� "admin1"�� ������ Ȯ�� 
	if(strcmp(loginPW,"admin1")==0){
		printf("�����ڴ� �α��� ����\n");
		
		while((choice=adminMenu())!=7){//7.�α� �ƿ�
			if(choice==1){			//1.�������
				registBook();
				sort();
				indexSort();
			}else if(choice==2){	//2.��������
				deleteBook();
				indexSort();
			}else if(choice==3){	//3.���� �뿩
				borrowBook();
				indexSort(); 
			}else if(choice==4){	//4.���� �ݳ�
				returnBook();
			}else if(choice==5){	//5.���� �˻�
				if(G_numOfBook>0){
					searchBook();
				}else{
					printf("���� �������� ������ �����ϴ�.\n");
				}
			}else if(choice==6){	//6.ȸ�� ���
				if(G_numOfClient>0){
					clientList();
				}else{
					printf("���� ȸ���� �����ϴ�.\n");
				}
			}else if(choice==8){	//8.���α׷� ���� 
				uploadFile();
				exit(1);
			}else{
				printf("�߸��� �Է��Դϴ�. �ٽ��Է��ϼ���.\n");
			}
		}
	}
}

/*------------------------------------
int adminMenu():������ �޴� ���� �Լ�
-------------------------------------*/
int adminMenu(){
	int choice;
	
	printf("\n1.���� ��� 2.���� ����\n3.���� �뿩 4.���� �ݳ�\n5.���� �˻� 6.ȸ�� ���\n7.�α� �ƿ� 8.���α׷� ����\n");
	printf("\n��ȣ�� �Է��ϼ���:");
	scanf("%d",&choice);
	
	return choice;
} 

/*------------------------------------
void registBook(): ���� ��� �Լ� 
-------------------------------------*/
void registBook(){
	BOOK* temp;
	
	temp = registInput();
	if(G_numOfBook==0){
		G_book[0] = (BOOK*)malloc(sizeof(BOOK));
	}else{
		G_book = (BOOK**)realloc(G_book,(G_numOfBook+1)*sizeof(BOOK *));
	}
	G_book[G_numOfBook++] = temp;
}

/*------------------------------------
BOOK* registInput(): ����� ���� ���� �Է��Լ� 
-------------------------------------*/
BOOK* registInput(){
	BOOK* temp;
	
	temp = (BOOK*)malloc(sizeof(BOOK));
	
	fflush(stdin);
	printf("������:");
	gets(temp->name);
	printf("���ǻ�:");
	scanf("%s",temp->publ);
	printf("����:");
	fflush(stdin);
	gets(temp->author);
	printf("ISBN:");
	scanf("%s",temp->isbn);
	printf("������ġ:");
	scanf("%s",temp->storage);
	temp->available = 'Y';
	
	return temp;
}

/*------------------------------------
void sort(): ���� ���� �Լ� 
-------------------------------------*/ 
void sort(){
	int i,j;
	BOOK* temp;
	
	for(i=0;i<G_numOfBook;i++){
		for(j=0;j<G_numOfBook;j++){
			if(strcmp(G_book[i]->name,G_book[j]->name)<0){
				temp = G_book[i];
				G_book[i] = G_book[j];
				G_book[j] = temp;
			}
		}
	}
}

/*------------------------------------
void indexSort(): ���� �ε��� �����Լ� 
-------------------------------------*/
void indexSort(){
	int i,cnt;
	for(i=0,cnt=1;i<G_numOfBook;i++,cnt++){
		G_book[i]->index = cnt;
	}
}

/*------------------------------------
void delteBook(): �������� �Լ� 
-------------------------------------*/
void deleteBook(){
	int i, j, k, l, cnt, cnt2, choiceType;
	char temp[41];
	cnt = 0;
	cnt2 = 0;
	while(1){
		printf(">>���� ����<<\n");
		printf("1.���������� ���� 2.ISBN���� ����");
		scanf("%d",&choiceType);
		if(choiceType==1){//1.���������� ���� 
			printf("������ ������:");
			fflush(stdin);
			gets(temp);
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->name)==0){
					cnt++;
					printf("%d\n",cnt); 
					printf("������ȣ:%d",G_book[i]->index);
					printf("������:%s ",G_book[i]->name);
					printf("���ǻ�:%s ",G_book[i]->publ);
					printf("����:%s ",G_book[i]->author);
					printf("ISBN:%s ",G_book[i]->isbn);
					printf("������ġ%s ",G_book[i]->storage);
					printf("�뿩����:%c\n",G_book[i]->available);
				}
			}
			break;
		}else if(choiceType==2){//2.ISBN���� ���� 
			printf("������ ������ ISBN:");
			fflush(stdin);
			gets(temp);
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->isbn)==0){
					cnt++;
					printf("%d\n",cnt); 
					printf("������ȣ:%d",G_book[i]->index);
					printf("������:%s ",G_book[i]->name);
					printf("���ǻ�:%s ",G_book[i]->publ);
					printf("����:%s ",G_book[i]->author);
					printf("ISBN:%s ",G_book[i]->isbn);
					printf("������ġ%s ",G_book[i]->storage);
					printf("�뿩����:%c\n",G_book[i]->available);
				}
			}
			break;
		}else{
			printf("�߸��� �Է��Դϴ�.\n");
		}
	}
	
	if(cnt==0){
		printf("\"%s\" �� �����ϴ�.\n",temp);
	}else{
		printf("�� �� ���° ������ �����ұ��?:");
		scanf("%d",&k);
		if(k<=0||k>cnt){
			printf("�ش� �����Ͱ� �������� �ʽ��ϴ�.\n");
			return;
		}
		if(choiceType==Bname){
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->name)==0){
					cnt2++;
				}
				if(cnt2==k){ 
					if(G_book[i]->available=='Y'){
						for(l=0;l<G_numOfBorrow;l++){
							if(G_borrow[l]->bk->index==G_book[i]->index){
								G_borrow[l]->ind = -1;
								strcpy(G_borrow[l]->BookName,G_book[i]->name);
								strcpy(G_borrow[l]->BookIsbn,G_book[i]->isbn);
							}
						}
						if(i<G_numOfBook-1){
							for(j=i;j<G_numOfBook-1;j++){
								G_book[j] = G_book[j+1];
							}
						}
						G_book = (BOOK**)realloc(G_book,(G_numOfBook-1)*sizeof(BOOK*));
						printf("%s ���� �Ϸ�.\n",temp);
						G_numOfBook--;
					}else{
						printf("���� �뿩���� �����Դϴ�.");
					}
					break;
				}
			}
		}else{
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->isbn)==0){
					cnt2++;
				}
				if(cnt2==k){ 
					if(G_book[i]->available=='Y'){
						for(l=0;l<G_numOfBorrow;l++){
							if(G_borrow[l]->bk->index==G_book[i]->index){
								G_borrow[l]->ind = -1;
								strcpy(G_borrow[l]->BookName,G_book[i]->name);
								strcpy(G_borrow[l]->BookIsbn,G_book[i]->isbn);
							}
						}
						if(i<G_numOfBook-1){
							for(j=i;j<G_numOfBook-1;j++){
								G_book[j] = G_book[j+1];
							}
						}
						G_book = (BOOK**)realloc(G_book,(G_numOfBook-1)*sizeof(BOOK*));
						printf("%s ���� �Ϸ�.\n",temp);
						G_numOfBook--;
					}else{
						printf("���� �뿩���� �����Դϴ�.");
					}
					break;
				}
			}
		} 
	}
	
}

/*------------------------------------
void searchBook(): �����˻� �Լ� 
-------------------------------------*/
void searchBook(){
	int i,cnt;
	char temp[41];
	SEARCHTYPE sType;
	
	while((sType=BsearchType())!=6){
		cnt = 0;
		if(sType==Bname){
			printf("�˻��� ������:");
			fflush(stdin);
			gets(temp); 
			
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->name)==0){
					printf("������ȣ:%d",G_book[i]->index);
					printf("������:%s ",G_book[i]->name);
					printf("���ǻ�:%s ",G_book[i]->publ);
					printf("����:%s ",G_book[i]->author);
					printf("ISBN:%s ",G_book[i]->isbn);
					printf("������ġ%s ",G_book[i]->storage);
					printf("�뿩����:%c\n",G_book[i]->available);
					cnt++;
				}
			}
			break;
		}else if(sType==Bpubl){
			printf("�˻��� ������ ���ǻ��:");
			fflush(stdin);
			gets(temp);
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->name)==0){
					printf("������ȣ:%d",G_book[i]->index);
					printf("������:%s ",G_book[i]->name);
					printf("���ǻ�:%s ",G_book[i]->publ);
					printf("����:%s ",G_book[i]->author);
					printf("ISBN:%s ",G_book[i]->isbn);
					printf("������ġ%s ",G_book[i]->storage);
					printf("�뿩����:%c\n",G_book[i]->available);
					cnt++;
				}
			}
			break;
		}else if(sType==Bisbn){
			printf("�˻��� ������ ISBN:");
			fflush(stdin);
			scanf("%s",temp); 
			
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->isbn)==0){
					printf("������ȣ:%d",G_book[i]->index);
					printf("������:%s ",G_book[i]->name);
					printf("���ǻ�:%s ",G_book[i]->publ);
					printf("����:%s ",G_book[i]->author);
					printf("ISBN:%s ",G_book[i]->isbn);
					printf("������ġ%s ",G_book[i]->storage);
					printf("�뿩����:%c\n",G_book[i]->available);
					cnt++;
				}
			}
			break;
		}else if(sType==Bauthor){
			printf("�˻��� ������ ���ڸ�:");
			fflush(stdin);
			gets(temp);
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->author)==0){
					printf("������ȣ:%d",G_book[i]->index);
					printf("������:%s ",G_book[i]->name);
					printf("���ǻ�:%s ",G_book[i]->publ);
					printf("����:%s ",G_book[i]->author);
					printf("ISBN:%s ",G_book[i]->isbn);
					printf("������ġ%s ",G_book[i]->storage);
					printf("�뿩����:%c\n",G_book[i]->available);
					cnt++;
				}
			}
			break;
		}else if(sType==Ball){
			for(i=0;i<G_numOfBook;i++){
				printf("������ȣ:%d",G_book[i]->index);
				printf("������:%s ",G_book[i]->name);
				printf("���ǻ�:%s ",G_book[i]->publ);
				printf("����:%s ",G_book[i]->author);
				printf("ISBN:%s ",G_book[i]->isbn);
				printf("������ġ%s ",G_book[i]->storage);
				printf("�뿩����:%c\n",G_book[i]->available);
				cnt++;
			}
			break;
		}else if(sType==Bexit){
			break;
		}else{
			printf("�߸��� �Է��Դϴ�.\n");
		}
	}

	if(cnt==0){
		printf("������ �����ϴ� ������ �����ϴ�.\n");
	}
}

/*------------------------------------
int BsearchType(): �����˻�Ÿ�� �����Լ� 
-------------------------------------*/
int BsearchType(){
	SEARCHTYPE t;
	
	printf("1.������ �˻� 2.���ǻ�� �˻�\n3.ISBN �˻�   4.���ڸ� �˻�\n5.��ü�˻�    6.������\n");
	printf("��ȣ�� �Է��ϼ���:");
	scanf("%d",&t);
	if(G_numOfBook==0){
		t = -1;
	}
	return t;
}

/*------------------------------------
void borrowBook(): �����뿩�Լ� 
-------------------------------------*/ 
void borrowBook(){
	int i, j;
	BORROW *temp;
	
	temp = tempBorrow();
	if(temp!=NULL){
		if(G_numOfBorrow==0){
			G_borrow[0] = (BORROW*)malloc(sizeof(BORROW));
		}else{
			G_borrow = (BORROW**)realloc(G_borrow,(G_numOfBorrow+1)*sizeof(BORROW*));
		}
		G_borrow[G_numOfBorrow++] = temp;
		printf("�뿩ó�� �Ϸ�.\n");
	}
}

/*------------------------------------
BORROW* tempBorrow(): �뿩���� �Է� �Լ� 
-------------------------------------*/ 
BORROW* tempBorrow(){
	char Cnum[10], bookName[41];
	int i,bookIndex,cnt,cnt2,choice;
	cnt = 0;
	cnt2 = 0;
	BORROW *temp;
	temp = (BORROW*)malloc(sizeof(BORROW));
	
	printf("�뿩���� �й��Է�:");
	scanf("%s",Cnum);
	for(i=0;i<G_numOfClient;i++){
		if(strcmp(Cnum,G_client[i]->num)==0){
			strcpy(temp->num,Cnum);
			break;
		}
	}
	if(i==G_numOfClient){
		printf("�ش� �й��� ȸ���� �������� �ʽ��ϴ�.\n");
		return NULL;
	}
	
	while(1){
		printf("1.������ �˻� 2.ISBN �˻�\n");
		printf("��ȣ�� �Է��ϼ���:");
		scanf("%d",&choice);
		if(choice==1||choice==2){
			break;	
		}
	}
	if(choice==1){
		printf("�뿩���� å �̸�:");
		scanf("%s",bookName);
		for(i=0;i<G_numOfBook;i++){
			if(strcmp(bookName,G_book[i]->name)==0){
				cnt++;
				printf("%d\n",cnt); 
				printf("������ȣ:%d",G_book[i]->index);
				printf("������:%s ",G_book[i]->name);
				printf("���ǻ�:%s ",G_book[i]->publ);
				printf("����:%s ",G_book[i]->author);
				printf("ISBN:%s ",G_book[i]->isbn);
				printf("������ġ%s ",G_book[i]->storage);
				printf("�뿩����:%c\n",G_book[i]->available);
			}
		}
	}else{
		printf("�뿩���� å�� ISBN:");
		scanf("%s",bookName);
		for(i=0;i<G_numOfBook;i++){
			if(strcmp(bookName,G_book[i]->isbn)==0){
				cnt++;
				printf("%d\n",cnt);
				printf("������ȣ:%d",G_book[i]->index);
				printf("������:%s ",G_book[i]->name);
				printf("���ǻ�:%s ",G_book[i]->publ);
				printf("����:%s ",G_book[i]->author);
				printf("ISBN:%s ",G_book[i]->isbn);
				printf("������ġ%s ",G_book[i]->storage);
				printf("�뿩����:%c\n",G_book[i]->available);
			}
		}
	}
	if(cnt==0){
		printf("�ش� ������ �������� �ʽ��ϴ�.\n");
		return NULL;
	}
	printf("���° å�� �뿩���� �Է�:");
	scanf("%d",&bookIndex);
	if(bookIndex<=0||bookIndex>cnt){
		printf("�ش� �����Ͱ� �������� �ʽ��ϴ�.\n");
		return NULL;
	}
	if(choice==1){
		for(i=0;i<G_numOfBook;i++){
			if(strcmp(bookName,G_book[i]->name)==0){
				cnt2++;
				if(cnt2==bookIndex){
					if(G_book[i]->available=='Y'){
						temp->bk = G_book[i];
						temp->ind = G_book[i]->index;
						
						printf("������ȣ:%d ,",G_book[i]->index);
						printf("������:%s ,",G_book[i]->name);
						printf("ISBN:%s\n",G_book[i]->isbn);
						G_book[i]->available = 'N';
					}else{
						printf("�̹� �뿩���� �����Դϴ�.\n");
						return NULL;
					}
					break;
				}
			}
		}
	}else{
		for(i=0;i<G_numOfBook;i++){
			if(strcmp(bookName,G_book[i]->isbn)==0){
				cnt2++;
				if(cnt2==bookIndex){
					if(G_book[i]->available=='Y'){
						temp->bk = G_book[i];
						temp->ind = G_book[i]->index;
						
						printf("������ȣ:%d ,",G_book[i]->index);
						printf("������:%s ,",G_book[i]->name);
						printf("ISBN:%s\n",G_book[i]->isbn);
						G_book[i]->available = 'N';
					}else{
						printf("�̹� �뿩���� �����Դϴ�.\n");
						return NULL;
					}
					break;
				}
			}
		}
	}
	strcpy(temp->returnNum,"-1");
	temp->ind = 0;
	strcpy(temp->BookName,"-1");
	strcpy(temp->BookIsbn,"-1");
	if(cnt==0){
		printf("�ش� index�� ���� ������ �������� �ʽ��ϴ�.\n");
		return NULL;
	}
	
	setDate(temp->rentDay);
	for(i=0;i<6;i++){
		temp->returnDay[i] = temp->rentDay[i];
	}
	
	temp->returnDay[2] += 30;
	if(temp->rentDay[1]==1){
		if(temp->returnDay[2]>31){
			temp->returnDay[1]++;
			temp->returnDay[2] -= 31;
		}
	}else if(temp->rentDay[1]==2){
		if(temp->returnDay[2]>28){
			temp->returnDay[1]++;
			temp->returnDay[2] -= 28;
		}	
	}else if(temp->rentDay[1]==3){
		if(temp->returnDay[2]>31){
			temp->returnDay[1]++;
			temp->returnDay[2] -= 31;
		}
	}else if(temp->rentDay[1]==4){
		if(temp->returnDay[2]>30){
			temp->returnDay[1]++;
			temp->returnDay[2] -= 30;
		}		
	}else if(temp->rentDay[1]==5){
		if(temp->returnDay[2]>31){
			temp->returnDay[1]++;
			temp->returnDay[2] -= 31;
		}	
	}else if(temp->rentDay[1]==6){
		if(temp->returnDay[2]>30){
			temp->returnDay[1]++;
			temp->returnDay[2] -= 30;
		}
	}else if(temp->rentDay[1]==7){
		if(temp->returnDay[2]>31){
			temp->returnDay[1]++;
			temp->returnDay[2] -= 31;
		}
	}else if(temp->rentDay[1]==8){
		if(temp->returnDay[2]>31){
			temp->returnDay[1]++;
			temp->returnDay[2] -= 31;
		}
	}else if(temp->rentDay[1]==9){
		if(temp->returnDay[2]>30){
			temp->returnDay[1]++;
			temp->returnDay[2] -= 30;
		}
	}else if(temp->rentDay[1]==10){
		if(temp->returnDay[2]>31){
			temp->returnDay[1]++;
			temp->returnDay[2] -= 31;
		}
	}else if(temp->rentDay[1]==11){
		if(temp->returnDay[2]>30){
			temp->returnDay[1]++;
			temp->returnDay[2] -= 30;
		}
	}else if(temp->rentDay[1]==12){
		if(temp->returnDay[2]>31){
			temp->returnDay[0]++;
			temp->returnDay[1] = 1;
			temp->returnDay[2] -= 31;
		}
	}
	
	return temp;	
}

/*------------------------------------
void setDate(int *date): ��¥ ���� �Լ� 
-------------------------------------*/ 
void setDate(int *date){
	struct tm *t;
	time_t timer;
	
	timer = time(NULL);
	t = localtime(&timer);
	
	date[0] = t->tm_year + 1900;
	date[1] = t->tm_mon + 1;
	date[2] = t->tm_mday;
	date[3] = t->tm_hour;
	date[4] = t->tm_min;
	date[5] = t->tm_sec;
}

/*------------------------------------
void returnBook(): ���� �ݳ� �Լ� 
-------------------------------------*/ 
void returnBook(){
	int i, j, cnt, cnt2, numExist;
	char  Cnum[10];
	cnt = 0;
	cnt2 = 0;
	numExist = 0;
	fflush(stdin);
	printf(">>�����ݳ�<<\n");
	printf("�й��� �Է��ϼ���:");
	scanf("%s",Cnum);
	for(i=0;i<G_numOfClient;i++){
		if(strcmp(Cnum,G_client[i]->num)==0){
			numExist = 1;
			break;
		}
	}
	if(numExist!=1){
		printf("�ش��й��� ���� ���� �������� �ʽ��ϴ�.\n");
		return;
	}
	printf("%s���� �뿩 ���\n",Cnum);
	
	for(i=0;i<G_numOfBorrow;i++){
		if(strcmp(G_borrow[i]->num,Cnum)==0 && strcmp(G_borrow[i]->returnNum,G_borrow[i]->num)!=0){
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
	}else{
		cnt = 0;
		printf("���° ������ �ݳ��Ͻðڽ��ϱ�?");
		scanf("%d",&cnt2);
		for(i=0;i<G_numOfBorrow;i++){
			if(strcmp(G_borrow[i]->num,Cnum)==0){
				cnt++;
				if(cnt==cnt2){
					setDate(G_borrow[i]->returnDay);
					G_borrow[i]->bk->available = 'Y';
					strcpy(G_borrow[i]->returnNum,Cnum);
					printf("�ݳ��� �Ϸ�Ǿ����ϴ�.\n");
				}
			}
		}
	}
}

/*------------------------------------
void clientList(): ȸ�� ��� ����Լ� 
-------------------------------------*/
void clientList(){
	int i, choice;
	char Cname[9],Cnum[10];
	
	while(1){
		printf(">>ȸ�� ���<<\n");
		printf("1.�̸� �˻� 2.�й� �˻�\n3.��ü �˻� 4.���� �޴�\n");
		printf("��ȣ�� �Է��ϼ���:");
		scanf("%d",&choice);
		if(choice>0&&choice<=4){
			break;
		}
	}
	if(choice==1){
		printf("�˻��� ȸ���� �̸�:");
		scanf("%s",Cname);
		for(i=0;i<G_numOfClient;i++){
			if(strcmp(Cname,G_client[i]->name)==0){
				printf("�й�:%s ",G_client[i]->num);
				printf("�̸�:%s \n",G_client[i]->name);
				printf("�ּ�:%s ",G_client[i]->addr);
				printf("��ȭ��ȣ:%s\n",G_client[i]->phone);
			}
		}
	}else if(choice==2){
		printf("�˻��� ȸ���� �й�:");
		scanf("%s",Cnum);
		for(i=0;i<G_numOfClient;i++){
			if(strcmp(Cnum,G_client[i]->num)==0){
				printf("�й�:%s ",G_client[i]->num);
				printf("�̸�:%s \n",G_client[i]->name);
				printf("�ּ�:%s",G_client[i]->addr);
				printf("��ȭ��ȣ:%s\n",G_client[i]->phone);
			}
		}
	}else if(choice==3){
		printf("��ü ȸ�� ���\n");
		for(i=0;i<G_numOfClient;i++){
			printf("�й�:%s ",G_client[i]->num);
			printf("�̸�:%s \n",G_client[i]->name);
			printf("�ּ�:%s",G_client[i]->addr);
			printf("��ȭ��ȣ:%s\n\n",G_client[i]->phone);
		}
		printf("�� ȸ����:%d\n",G_numOfClient);
	}
	
}
