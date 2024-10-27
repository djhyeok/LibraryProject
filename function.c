#include "common.h"
#include "client.h"
#include "admin.h"
#include "function.h"

extern CLIENT** G_client;	//�� �������� 
extern int G_numOfClient;	//�� �� 
extern BOOK** G_book;		//å �������� 
extern int G_numOfBook;		//å �Ǽ� 
extern BORROW** G_borrow;	//�뿩 �������� 
extern int G_numOfBorrow;	//�뿩 �Ǽ� 
typedef enum filetype{clientF=1,bookF,borrowF} FILETYPE; 	//������,å����,�뿩���� enum

/*------------------------------------
int menu(): �޴� ��� ��� 
-------------------------------------*/
int menu(){
	int choice;
	
	printf("1.ȸ������ 2.�α��� 3.���α׷� ���� 4.�α⵵�� 5.�ִٴ뿩ȸ�� 6.����ݳ�ȸ�� 7.�뿩�Ⱓ�� ��ȸ");
	scanf("%d",&choice);
	
	return choice;
}

/*------------------------------------
void Login():������ �� �� �α��� �Լ� 
-------------------------------------*/
void Login(){
	char loginID[10];		//�α��ν� ���� ���̵� char�� ���� 
	int i;
	
	printf("ID�Է�:");
	scanf("%s",loginID);
	
	if(strcmp(loginID,"admin")==0){		//admin �α��� ID : "admin" 
		adminLogin();
	}else{
		//�Ϲ� �α��� �� �����̵�� ��ġ�ϴ��� Ȯ�� �� �� �α��� �Լ� ȣ�� 
		for(i=0;i<G_numOfClient;i++){
			if(strcmp(G_client[i]->num,loginID)==0){
				//�й��� ��ġ�ϴ� �� �α��� �Լ�ȣ�� 
				clientLogin(i);
				break;
			}
		}
		//�����̵�� ��ġ���� �ʴ� �α��νõ��� 
		if(i==G_numOfClient){
			printf("�������� �ʴ� ID\n");
			return;
		}
	}
}

/*------------------------------------
void Join(): ȸ�����Ա�� 
-------------------------------------*/
void Join(){
	CLIENT *temp;		//ȸ������ ������ ���� �ӽð����� 
	temp = joinInput();	//joinInput �Լ��� ���� �ӽú����� ���������� ��� 
	
	//ȸ�������� ������ �߻��� ��� return 
	if(temp==NULL){
		return;
	}
	
	//ù ȸ���� ��� 
	if(G_numOfClient==0){
		G_client[0] = (CLIENT *)malloc(sizeof(CLIENT));
	}else{		//ù ȸ���� �ƴҰ�� 
		G_client = (CLIENT **)realloc(G_client, (G_numOfClient+1)*sizeof(CLIENT *));
	}
	
	G_client[G_numOfClient++] = temp;
	printf("ȸ������ ����!!\n");
}

/*------------------------------------
void create(): ���ѱ׷� �ʱ� �����Լ�(�޸� �Ҵ�,������ ���� �ҷ�����)
-------------------------------------*/
void create(){
	int i;
	
	G_client = (CLIENT**)malloc(sizeof(CLIENT*));
	G_book = (BOOK**)malloc(sizeof(BOOK*));
	G_borrow = (BORROW**)malloc(sizeof(BORROW*));
	
	//���ο��� create ȣ��� ���Ͽ� ����� ���� �ҷ����� 
	downloadFile();
}

/*------------------------------------
void downloadFile(): ��,����,�뿩 ���Ͽ��� ���� �ҷ��� 
-------------------------------------*/
void downloadFile(){
	FILE *clientFile,*bookFile,*borrowFile;
	int t, i;
	FILETYPE x;
	BORROW *tempBorrow;
	BOOK *tempBook;
	CLIENT *tempClient;
	 
	clientFile = fopen("LibraryClient.txt","rb");
	if(clientFile!=NULL){
		while(1){//Client���� 
			tempClient = (CLIENT *)readF(&clientFile,1); 
			if(tempClient==NULL){
				break;
			}
			
			if(G_numOfClient==0){
				G_client[0] = (CLIENT *)malloc(sizeof(CLIENT));
			}else{
				G_client = (CLIENT**)realloc(G_client,(G_numOfClient+1)*sizeof(CLIENT*));
			}
			G_client[G_numOfClient++] = tempClient;
		}
		fclose(clientFile);
	}
	
	bookFile = fopen("LibraryBook.txt","rb");
	if(bookFile!=NULL){
		while(1){//Book���� 
			tempBook = (BOOK *)readF(&bookFile,2); 
			if(tempBook==NULL){
				break;
			}
			
			if(G_numOfBook==0){
				G_book[0] = (BOOK *)malloc(sizeof(BOOK));
			}else{
				G_book = (BOOK**)realloc(G_book,(G_numOfBook+1)*sizeof(BOOK*));
			}
			G_book[G_numOfBook++] = tempBook;	
		}		
		fclose(bookFile);
	}

	borrowFile = fopen("LibraryBorrow.txt","rb");	
	if(borrowFile!=NULL){
		while(1){//Borrow���� 
			tempBorrow = (BORROW *)readF(&borrowFile,3);
			if(tempBorrow==NULL){
				break;
			}
			
			if(G_numOfBorrow==0){
				G_borrow[0] = (BORROW *)malloc(sizeof(BORROW));
			}else{
				G_borrow = (BORROW**)realloc(G_borrow,(G_numOfBorrow+1)*sizeof(BORROW*));
			}
			G_borrow[G_numOfBorrow] = tempBorrow;
			if(G_borrow[G_numOfBorrow]->ind!=-1){
				for(i=0;i<G_numOfBook;i++){
					if(G_borrow[G_numOfBorrow]->ind==G_book[i]->index){
						G_borrow[G_numOfBorrow]->bk = G_book[i];
					}
				}
			}else{
				G_borrow[G_numOfBorrow] = NULL;
			}	
			G_numOfBorrow++;
		}	
		fclose(borrowFile);
	}
	
}

/*------------------------------------
void* readF(FILE**,int type): ���������� �о ������ �����ϴ� �Լ�
	 type : <1> client
			<2> book
			<3> borrow
-------------------------------------*/
void* readF(FILE **fp,int type){
	int i;
	BOOK *bookp;
	BORROW *borrowp;
	CLIENT *clientp;
	
	if(type==1){
		clientp = (CLIENT*)malloc(sizeof(CLIENT));
		fread(clientp,sizeof(CLIENT),1,*fp);
		if(feof(*fp)!=0){
			return NULL;
		}
		return clientp;
	}else if(type==2){
		bookp = (BOOK*)malloc(sizeof(BOOK));
		fread(bookp,sizeof(BOOK),1,*fp);
		if(feof(*fp)!=0){
			return NULL;
		} 
		return bookp;
	}else if(type==3){
		borrowp = (BORROW*)malloc(sizeof(BORROW));
		fread(borrowp,sizeof(BORROW),1,*fp);
		if(feof(*fp)!=0){
			return NULL;
		}
		return borrowp;
	}	
}

/*------------------------------------
void uploadFile(): ��,����,�뿩 ���� �������Ϸ� ��� 
-------------------------------------*/
void uploadFile(){
	FILE *clientFile,*bookFile,*borrowFile;
	int i;
		
	bookFile = fopen("LibraryBook.txt","wb");
	for(i=0;i<G_numOfBook;i++){
		fwrite(G_book[i],sizeof(BOOK),1,bookFile);	
	}
	
	clientFile = fopen("LibraryClient.txt","wb");
	for(i=0;i<G_numOfClient;i++){
		fwrite(G_client[i],sizeof(CLIENT),1,clientFile);
	}
	
	borrowFile = fopen("LibraryBorrow.txt","wb");
	for(i=0;i<G_numOfBorrow;i++){
		if(G_borrow[i]->ind!=-1){
			G_borrow[i]->ind = G_borrow[i]->bk->index;
		}
		G_borrow[i]->bk = NULL; 
		fwrite(G_borrow[i],sizeof(BORROW),1,borrowFile);
	}
	
	
	fclose(clientFile);
	fclose(bookFile);
	fclose(borrowFile);
}

/*------------------------------------
CLIENT* joinInput(): ȸ���������� �Է��Լ� 
-------------------------------------*/
CLIENT* joinInput(){
	CLIENT *temp;
	int j;
	
	temp = (CLIENT *)malloc(sizeof(CLIENT));
	//�й�,��й�ȣ,�̸�,�ּ�,��ȭ��ȣ
	printf("�й��� �Է��ϼ���:");
	scanf("%s",temp->num);
	fflush(stdin);
	for(j=0;j<G_numOfClient;j++){
		if(strcmp(G_client[j]->num,temp->num)==0){
			printf("�̹� �����ϴ� ���̵�\n");
			return NULL;
		}
	}
	printf("��й�ȣ�� �Է��ϼ���:");
	scanf("%s",temp->pw);
	printf("�̸��� �Է��ϼ���:");
	scanf("%s",temp->name);
	printf("�ּҸ� �Է��ϼ���:");
	fflush(stdin);
	gets(temp->addr); 
	printf("��ȭ��ȣ�� �Է��ϼ���:"); 
	fflush(stdin);
	gets(temp->phone);
	for(j=0;j<G_numOfClient;j++){
		if(strcmp(G_client[j]->phone,temp->phone)==0){
			printf("�̹� �����ϴ� ��ȭ��ȣ\n");
			return NULL;
		}
	}
	
	return temp;
}

/*------------------------------------
void top3(): top3 �α⵵�� ����Լ� 
-------------------------------------*/
void top3(){
	int i, j, k, count, temp;
	char **isbn, tempc[11];
	int *cnt;
	
	//isbn�� cnt�� �뿩����ŭ �Ҵ��� �ʱ�ȭ 
	isbn = (char**)malloc(G_numOfBorrow*sizeof(char*));
	cnt = (int*)malloc(G_numOfBorrow*sizeof(int));
	for(i=0;i<G_numOfBorrow;i++){
		cnt[i] = 0;
	}
	for(i=0;i<G_numOfBorrow;i++){
		isbn[i] = (char*)malloc(11*sizeof(char));
		strcpy(isbn[i],"");
	}
	
	//isbn�� �뿩������ isbn�� ��� cnt�� ��ġ�� ���� ���� ���� 
	for(i=0;i<G_numOfBorrow;i++){
		count = 0;
		for(j=0;j<G_numOfBorrow;j++){
			if(strcmp(G_borrow[i]->bk->isbn,isbn[j])==0){
				count++;
				break;
			}
		}
		if(count>0){
			cnt[j]++;
		}else{
			strcpy(isbn[i],G_borrow[i]->bk->isbn);
			cnt[i]++;
		}
	}
	//isbn�� cnt�� ������������ 
	for(i=0;i<G_numOfBorrow;i++){
		for(j=0;j<G_numOfBorrow;j++){
			if(cnt[i]>cnt[j]){
				strcpy(tempc,isbn[i]);
				strcpy(isbn[i],isbn[j]);
				strcpy(isbn[j],tempc);
				temp = cnt[i];
				cnt[i] = cnt[j];
				cnt[j] = temp;
			}
		}
	}
	//���� 3���� ��� 
	for(i=0;i<3;i++){
		if(cnt[i]==0||i>G_numOfBorrow){
			break;
		}
		for(j=0;j<G_numOfBook;j++){
			if(strcmp(isbn[i],G_book[j]->isbn)==0){
				printf("�뿩��:%d ",cnt[i]);
				printf("å�̸�:%s ",G_book[j]->name);
				printf("���ǻ�:%s ",G_book[j]->publ);
				printf("����:%s ",G_book[j]->author);
				printf("ISBN:%s\n",G_book[j]->isbn);
				break;
			}
		}
	}
	free(isbn);
	free(cnt);
}

/*------------------------------------
void mostBorrow(): �ִٴ뿩 ȸ����� �Լ� 
-------------------------------------*/
void mostBorrow(){
	int i, j, count, temp;
	char **Bclient;
	int *cnt;
	
	Bclient = (char **)malloc(G_numOfClient*sizeof(char*));
	for(i=0;i<G_numOfClient;i++){
		Bclient[i] = (char *)malloc(10*sizeof(char));
	}
	cnt = (int *)malloc(G_numOfClient*sizeof(int));
	
	for(i=0;i<G_numOfBorrow;i++){
		count = 0;
		for(j=0;j<G_numOfClient;j++){
			if(strcmp(G_borrow[i]->num,Bclient[j])==0){
				count++;
				break;
			}
		}
		if(count>0){
			cnt[j]++;
		}else{
			strcpy(Bclient[i],G_borrow[i]->num);
			cnt[i]++;
		}
	}
	temp = cnt[0];
	for(i=0;i<G_numOfClient;i++){
		if(temp<cnt[i]){
			temp = cnt[i];
			j = i;
		}
	}
	
	for(i=0;i<G_numOfClient;i++){
		if(strcmp(Bclient[j],G_client[i]->num)==0){
			printf("�й�:%s ",G_client[i]->num);
			printf("�̸�:%s ",G_client[i]->name);
			printf("�뿩��:%d \n",temp);
			break;
		}
	}	
}

/*------------------------------------
void earlyReturn(): ����ݳ� ȸ����� �Լ� 
-------------------------------------*/
void earlyReturn(){
	int i, j, bDate[6], rDate[6];
	double bor,ret, ddd;
	bor = 0;
	ret = 0;
	
	for(i=0;i<G_numOfBorrow;i++){
		if(strcmp(G_borrow[i]->returnNum,"-1")==0){
			continue;
		}
		for(j=0;j<6;j++){
			bDate[j] = G_borrow[i]->rentDay[j];
			rDate[j] = bDate[j];
		}
		
		rDate[2] += 30;
		if(bDate[1]==1){
			if(rDate[2]>31){
				rDate[1]++;
				rDate[2] -= 31;
			}
		}else if(bDate[1]==2){
			if(rDate[2]>28){
				rDate[1]++;
				rDate[2] -= 28;
			}	
		}else if(bDate[1]==3){
			if(rDate[2]>31){
				rDate[1]++;
				rDate[2] -= 31;
			}
		}else if(bDate[1]==4){
			if(rDate[2]>30){
				rDate[1]++;
				rDate[2] -= 30;
			}		
		}else if(bDate[1]==5){
			if(rDate[2]>31){
				rDate[1]++;
				rDate[2] -= 31;
			}	
		}else if(bDate[1]==6){
			if(rDate[2]>30){
				rDate[1]++;
				rDate[2] -= 30;
			}
		}else if(bDate[1]==7){
			if(rDate[2]>31){
				rDate[1]++;
				rDate[2] -= 31;
			}
		}else if(bDate[1]==8){
			if(rDate[2]>31){
				rDate[1]++;
				rDate[2] -= 31;
			}
		}else if(bDate[1]==9){
			if(rDate[2]>30){
				rDate[1]++;
				rDate[2] -= 30;
			}
		}else if(bDate[1]==10){
			if(rDate[2]>31){
				rDate[1]++;
				rDate[2] -= 31;
			}
		}else if(bDate[1]==11){
			if(rDate[2]>30){
				rDate[1]++;
				rDate[2] -= 30;
			}
		}else if(bDate[1]==12){
			if(rDate[2]>31){
				rDate[0]++;
				rDate[1] = 1;
				rDate[2] -= 31;
			}
		}
		
		bor += bDate[0]*60*60*24*12*365;
		bor += bDate[1]*60*60*24*12;
		bor += bDate[2]*60*60*24;
		bor += bDate[3]*60*60;
		bor += bDate[4]*60;
		bor += bDate[5];
		
		
		ret += rDate[0]*60*60*24*12*365;
		ret += rDate[1]*60*60*24*12;
		ret += rDate[2]*60*60*24;
		ret += rDate[3]*60*60;
		ret += rDate[4]*60;
		ret += rDate[5];
		
		ddd = 30*24*60*60;
		
		if(ret-bor<ddd){
			printf("�й�:%s ",G_borrow[i]->num);
			printf("�̸�:%s ",G_borrow[i]->returnNum);
			printf("å�̸�:%s\n",G_borrow[i]->bk->name); 
		}
	}
}

/*------------------------------------
void returnTerm(): �뿩�Ⱓ�� ��ȸ �Լ� 
-------------------------------------*/
void returnTerm(){

	int i, j, k, dat[3], dat2[3];
	double bor, ret, rent;
	
	printf("�뿩�Ⱓ�� ��ȸ\n");
	printf("�˻��� �Ⱓ�� �����ϼ���\n");
	printf("���۳�:");
	scanf("%d",&dat[0]);
	printf("���ۿ�:");
	scanf("%d",&dat[1]);
	printf("������:");
	scanf("%d",&dat[2]);
	
	printf("�����:");
	scanf("%d",&dat2[0]);
	printf("�����:");
	scanf("%d",&dat2[1]);
	printf("������:");
	scanf("%d",&dat2[2]);
	
	bor += dat[0]*60*60*24*12*365;
	bor += dat[1]*60*60*24*12;
	bor += dat[2]*60*60*24;
	
	ret += dat2[0]*60*60*24*12*365;
	ret += dat2[1]*60*60*24*12;
	ret += dat2[2]*60*60*24;
	
	
	for(i=0;i<G_numOfBorrow;i++){
		rent = 0;
		rent += G_borrow[i]->rentDay[0]*60*60*24*12*365;
		rent += G_borrow[i]->rentDay[1]*60*60*24*12;
		rent += G_borrow[i]->rentDay[2]*60*60*24;

		if(bor<=rent && rent<=ret){
			printf("�뿩���й�:%s ",G_borrow[i]->num);
			printf("�뿩����:%s ",G_borrow[i]->bk->name);
			printf("�뿩��:");
			for(k=0;k<6;k++){
				printf("-%d",G_borrow[i]->rentDay[k]);
			}
			printf("\n"); 
		}
	}
}
















