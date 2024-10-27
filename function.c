#include "common.h"
#include "client.h"
#include "admin.h"
#include "function.h"

extern CLIENT** G_client;	//고객 전역변수 
extern int G_numOfClient;	//고객 수 
extern BOOK** G_book;		//책 전역변수 
extern int G_numOfBook;		//책 권수 
extern BORROW** G_borrow;	//대여 전역변수 
extern int G_numOfBorrow;	//대여 권수 
typedef enum filetype{clientF=1,bookF,borrowF} FILETYPE; 	//고객파일,책파일,대여파일 enum

/*------------------------------------
int menu(): 메뉴 출력 기능 
-------------------------------------*/
int menu(){
	int choice;
	
	printf("1.회원가입 2.로그인 3.프로그램 종료 4.인기도서 5.최다대여회원 6.조기반납회원 7.대여기간별 조회");
	scanf("%d",&choice);
	
	return choice;
}

/*------------------------------------
void Login():관리자 및 고객 로그인 함수 
-------------------------------------*/
void Login(){
	char loginID[10];		//로그인시 받을 아이디 char형 변수 
	int i;
	
	printf("ID입력:");
	scanf("%s",loginID);
	
	if(strcmp(loginID,"admin")==0){		//admin 로그인 ID : "admin" 
		adminLogin();
	}else{
		//일반 로그인 시 고객아이디와 일치하는지 확인 후 고객 로그인 함수 호출 
		for(i=0;i<G_numOfClient;i++){
			if(strcmp(G_client[i]->num,loginID)==0){
				//학번이 일치하는 고객 로그인 함수호출 
				clientLogin(i);
				break;
			}
		}
		//고객아이디와 일치하지 않는 로그인시도시 
		if(i==G_numOfClient){
			printf("존재하지 않는 ID\n");
			return;
		}
	}
}

/*------------------------------------
void Join(): 회원가입기능 
-------------------------------------*/
void Join(){
	CLIENT *temp;		//회원가입 정보를 담을 임시고객변수 
	temp = joinInput();	//joinInput 함수를 통해 임시변수에 고객가입정보 담기 
	
	//회원가입중 에러가 발생할 경우 return 
	if(temp==NULL){
		return;
	}
	
	//첫 회원일 경우 
	if(G_numOfClient==0){
		G_client[0] = (CLIENT *)malloc(sizeof(CLIENT));
	}else{		//첫 회원이 아닐경우 
		G_client = (CLIENT **)realloc(G_client, (G_numOfClient+1)*sizeof(CLIENT *));
	}
	
	G_client[G_numOfClient++] = temp;
	printf("회원가입 성공!!\n");
}

/*------------------------------------
void create(): 프롤그램 초기 설정함수(메모리 할당,파일의 내용 불러오기)
-------------------------------------*/
void create(){
	int i;
	
	G_client = (CLIENT**)malloc(sizeof(CLIENT*));
	G_book = (BOOK**)malloc(sizeof(BOOK*));
	G_borrow = (BORROW**)malloc(sizeof(BORROW*));
	
	//메인에서 create 호출시 파일에 저장된 정보 불러오기 
	downloadFile();
}

/*------------------------------------
void downloadFile(): 고객,도서,대여 파일에서 정보 불러옴 
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
		while(1){//Client파일 
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
		while(1){//Book파일 
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
		while(1){//Borrow파일 
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
void* readF(FILE**,int type): 이진파일을 읽어서 데이터 리턴하는 함수
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
void uploadFile(): 고객,도서,대여 정보 이진파일로 기록 
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
CLIENT* joinInput(): 회원가입정보 입력함수 
-------------------------------------*/
CLIENT* joinInput(){
	CLIENT *temp;
	int j;
	
	temp = (CLIENT *)malloc(sizeof(CLIENT));
	//학번,비밀번호,이름,주소,전화번호
	printf("학번을 입력하세요:");
	scanf("%s",temp->num);
	fflush(stdin);
	for(j=0;j<G_numOfClient;j++){
		if(strcmp(G_client[j]->num,temp->num)==0){
			printf("이미 존재하는 아이디\n");
			return NULL;
		}
	}
	printf("비밀번호를 입력하세요:");
	scanf("%s",temp->pw);
	printf("이름을 입력하세요:");
	scanf("%s",temp->name);
	printf("주소를 입력하세요:");
	fflush(stdin);
	gets(temp->addr); 
	printf("전화번호를 입력하세요:"); 
	fflush(stdin);
	gets(temp->phone);
	for(j=0;j<G_numOfClient;j++){
		if(strcmp(G_client[j]->phone,temp->phone)==0){
			printf("이미 존재하는 전화번호\n");
			return NULL;
		}
	}
	
	return temp;
}

/*------------------------------------
void top3(): top3 인기도서 출력함수 
-------------------------------------*/
void top3(){
	int i, j, k, count, temp;
	char **isbn, tempc[11];
	int *cnt;
	
	//isbn과 cnt에 대여수만큼 할당후 초기화 
	isbn = (char**)malloc(G_numOfBorrow*sizeof(char*));
	cnt = (int*)malloc(G_numOfBorrow*sizeof(int));
	for(i=0;i<G_numOfBorrow;i++){
		cnt[i] = 0;
	}
	for(i=0;i<G_numOfBorrow;i++){
		isbn[i] = (char*)malloc(11*sizeof(char));
		strcpy(isbn[i],"");
	}
	
	//isbn에 대여도서의 isbn을 담고 cnt와 위치를 맞춰 숫자 증가 
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
	//isbn과 cnt를 내림차순정렬 
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
	//상위 3개만 출력 
	for(i=0;i<3;i++){
		if(cnt[i]==0||i>G_numOfBorrow){
			break;
		}
		for(j=0;j<G_numOfBook;j++){
			if(strcmp(isbn[i],G_book[j]->isbn)==0){
				printf("대여수:%d ",cnt[i]);
				printf("책이름:%s ",G_book[j]->name);
				printf("출판사:%s ",G_book[j]->publ);
				printf("저자:%s ",G_book[j]->author);
				printf("ISBN:%s\n",G_book[j]->isbn);
				break;
			}
		}
	}
	free(isbn);
	free(cnt);
}

/*------------------------------------
void mostBorrow(): 최다대여 회원출력 함수 
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
			printf("학번:%s ",G_client[i]->num);
			printf("이름:%s ",G_client[i]->name);
			printf("대여수:%d \n",temp);
			break;
		}
	}	
}

/*------------------------------------
void earlyReturn(): 조기반납 회원출력 함수 
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
			printf("학번:%s ",G_borrow[i]->num);
			printf("이름:%s ",G_borrow[i]->returnNum);
			printf("책이름:%s\n",G_borrow[i]->bk->name); 
		}
	}
}

/*------------------------------------
void returnTerm(): 대여기간별 조회 함수 
-------------------------------------*/
void returnTerm(){

	int i, j, k, dat[3], dat2[3];
	double bor, ret, rent;
	
	printf("대여기간별 조회\n");
	printf("검색할 기간을 설정하세요\n");
	printf("시작년:");
	scanf("%d",&dat[0]);
	printf("시작월:");
	scanf("%d",&dat[1]);
	printf("시작일:");
	scanf("%d",&dat[2]);
	
	printf("종료년:");
	scanf("%d",&dat2[0]);
	printf("종료월:");
	scanf("%d",&dat2[1]);
	printf("종료일:");
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
			printf("대여자학번:%s ",G_borrow[i]->num);
			printf("대여도서:%s ",G_borrow[i]->bk->name);
			printf("대여일:");
			for(k=0;k<6;k++){
				printf("-%d",G_borrow[i]->rentDay[k]);
			}
			printf("\n"); 
		}
	}
}
















