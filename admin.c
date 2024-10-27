#include "common.h"
#include "admin.h"
#include "function.h"

extern CLIENT** G_client;	 
extern int G_numOfClient;	 
extern BOOK** G_book;		 
extern int G_numOfBook;		 
extern BORROW** G_borrow;	 
extern int G_numOfBorrow;	

typedef enum searchType{Bname=1,Bpubl,Bisbn,Bauthor,Ball,Bexit} SEARCHTYPE;	//검색 타입 enum 

/*------------------------------------
void adminLogin(): 관리자로그인함수 
-------------------------------------*/ 
void adminLogin(){
	char loginPW[20];
	int choice;	
	
	//임시 문자열에 pw받음 
	printf("PW입력:");
	scanf("%s",loginPW);
	
	//관리자 로그인 PW인 "admin1"과 같은지 확인 
	if(strcmp(loginPW,"admin1")==0){
		printf("관리자님 로그인 성공\n");
		
		while((choice=adminMenu())!=7){//7.로그 아웃
			if(choice==1){			//1.도서등록
				registBook();
				sort();
				indexSort();
			}else if(choice==2){	//2.도서삭제
				deleteBook();
				indexSort();
			}else if(choice==3){	//3.도서 대여
				borrowBook();
				indexSort(); 
			}else if(choice==4){	//4.도서 반납
				returnBook();
			}else if(choice==5){	//5.도서 검색
				if(G_numOfBook>0){
					searchBook();
				}else{
					printf("현재 보관중인 도서가 없습니다.\n");
				}
			}else if(choice==6){	//6.회원 목록
				if(G_numOfClient>0){
					clientList();
				}else{
					printf("현재 회원이 없습니다.\n");
				}
			}else if(choice==8){	//8.프로그램 종료 
				uploadFile();
				exit(1);
			}else{
				printf("잘못된 입력입니다. 다시입력하세요.\n");
			}
		}
	}
}

/*------------------------------------
int adminMenu():관리자 메뉴 선택 함수
-------------------------------------*/
int adminMenu(){
	int choice;
	
	printf("\n1.도서 등록 2.도서 삭제\n3.도서 대여 4.도서 반납\n5.도서 검색 6.회원 목록\n7.로그 아웃 8.프로그램 종료\n");
	printf("\n번호를 입력하세요:");
	scanf("%d",&choice);
	
	return choice;
} 

/*------------------------------------
void registBook(): 도서 등록 함수 
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
BOOK* registInput(): 등록할 도서 정보 입력함수 
-------------------------------------*/
BOOK* registInput(){
	BOOK* temp;
	
	temp = (BOOK*)malloc(sizeof(BOOK));
	
	fflush(stdin);
	printf("도서명:");
	gets(temp->name);
	printf("출판사:");
	scanf("%s",temp->publ);
	printf("저자:");
	fflush(stdin);
	gets(temp->author);
	printf("ISBN:");
	scanf("%s",temp->isbn);
	printf("보관위치:");
	scanf("%s",temp->storage);
	temp->available = 'Y';
	
	return temp;
}

/*------------------------------------
void sort(): 도서 정렬 함수 
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
void indexSort(): 도서 인덱스 정렬함수 
-------------------------------------*/
void indexSort(){
	int i,cnt;
	for(i=0,cnt=1;i<G_numOfBook;i++,cnt++){
		G_book[i]->index = cnt;
	}
}

/*------------------------------------
void delteBook(): 도서삭제 함수 
-------------------------------------*/
void deleteBook(){
	int i, j, k, l, cnt, cnt2, choiceType;
	char temp[41];
	cnt = 0;
	cnt2 = 0;
	while(1){
		printf(">>도서 삭제<<\n");
		printf("1.도서명으로 삭제 2.ISBN으로 삭제");
		scanf("%d",&choiceType);
		if(choiceType==1){//1.도서명으로 삭제 
			printf("삭제할 도서명:");
			fflush(stdin);
			gets(temp);
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->name)==0){
					cnt++;
					printf("%d\n",cnt); 
					printf("도서번호:%d",G_book[i]->index);
					printf("도서명:%s ",G_book[i]->name);
					printf("출판사:%s ",G_book[i]->publ);
					printf("저자:%s ",G_book[i]->author);
					printf("ISBN:%s ",G_book[i]->isbn);
					printf("보관위치%s ",G_book[i]->storage);
					printf("대여가능:%c\n",G_book[i]->available);
				}
			}
			break;
		}else if(choiceType==2){//2.ISBN으로 삭제 
			printf("삭제할 도서의 ISBN:");
			fflush(stdin);
			gets(temp);
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->isbn)==0){
					cnt++;
					printf("%d\n",cnt); 
					printf("도서번호:%d",G_book[i]->index);
					printf("도서명:%s ",G_book[i]->name);
					printf("출판사:%s ",G_book[i]->publ);
					printf("저자:%s ",G_book[i]->author);
					printf("ISBN:%s ",G_book[i]->isbn);
					printf("보관위치%s ",G_book[i]->storage);
					printf("대여가능:%c\n",G_book[i]->available);
				}
			}
			break;
		}else{
			printf("잘못된 입력입니다.\n");
		}
	}
	
	if(cnt==0){
		printf("\"%s\" 가 없습니다.\n",temp);
	}else{
		printf("이 중 몇번째 도서를 삭제할까요?:");
		scanf("%d",&k);
		if(k<=0||k>cnt){
			printf("해당 데이터가 존재하지 않습니다.\n");
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
						printf("%s 삭제 완료.\n",temp);
						G_numOfBook--;
					}else{
						printf("아직 대여중인 도서입니다.");
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
						printf("%s 삭제 완료.\n",temp);
						G_numOfBook--;
					}else{
						printf("아직 대여중인 도서입니다.");
					}
					break;
				}
			}
		} 
	}
	
}

/*------------------------------------
void searchBook(): 도서검색 함수 
-------------------------------------*/
void searchBook(){
	int i,cnt;
	char temp[41];
	SEARCHTYPE sType;
	
	while((sType=BsearchType())!=6){
		cnt = 0;
		if(sType==Bname){
			printf("검색할 도서명:");
			fflush(stdin);
			gets(temp); 
			
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->name)==0){
					printf("도서번호:%d",G_book[i]->index);
					printf("도서명:%s ",G_book[i]->name);
					printf("출판사:%s ",G_book[i]->publ);
					printf("저자:%s ",G_book[i]->author);
					printf("ISBN:%s ",G_book[i]->isbn);
					printf("보관위치%s ",G_book[i]->storage);
					printf("대여가능:%c\n",G_book[i]->available);
					cnt++;
				}
			}
			break;
		}else if(sType==Bpubl){
			printf("검색할 도서의 출판사명:");
			fflush(stdin);
			gets(temp);
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->name)==0){
					printf("도서번호:%d",G_book[i]->index);
					printf("도서명:%s ",G_book[i]->name);
					printf("출판사:%s ",G_book[i]->publ);
					printf("저자:%s ",G_book[i]->author);
					printf("ISBN:%s ",G_book[i]->isbn);
					printf("보관위치%s ",G_book[i]->storage);
					printf("대여가능:%c\n",G_book[i]->available);
					cnt++;
				}
			}
			break;
		}else if(sType==Bisbn){
			printf("검색할 도서의 ISBN:");
			fflush(stdin);
			scanf("%s",temp); 
			
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->isbn)==0){
					printf("도서번호:%d",G_book[i]->index);
					printf("도서명:%s ",G_book[i]->name);
					printf("출판사:%s ",G_book[i]->publ);
					printf("저자:%s ",G_book[i]->author);
					printf("ISBN:%s ",G_book[i]->isbn);
					printf("보관위치%s ",G_book[i]->storage);
					printf("대여가능:%c\n",G_book[i]->available);
					cnt++;
				}
			}
			break;
		}else if(sType==Bauthor){
			printf("검색할 도서의 저자명:");
			fflush(stdin);
			gets(temp);
			for(i=0;i<G_numOfBook;i++){
				if(strcmp(temp,G_book[i]->author)==0){
					printf("도서번호:%d",G_book[i]->index);
					printf("도서명:%s ",G_book[i]->name);
					printf("출판사:%s ",G_book[i]->publ);
					printf("저자:%s ",G_book[i]->author);
					printf("ISBN:%s ",G_book[i]->isbn);
					printf("보관위치%s ",G_book[i]->storage);
					printf("대여가능:%c\n",G_book[i]->available);
					cnt++;
				}
			}
			break;
		}else if(sType==Ball){
			for(i=0;i<G_numOfBook;i++){
				printf("도서번호:%d",G_book[i]->index);
				printf("도서명:%s ",G_book[i]->name);
				printf("출판사:%s ",G_book[i]->publ);
				printf("저자:%s ",G_book[i]->author);
				printf("ISBN:%s ",G_book[i]->isbn);
				printf("보관위치%s ",G_book[i]->storage);
				printf("대여가능:%c\n",G_book[i]->available);
				cnt++;
			}
			break;
		}else if(sType==Bexit){
			break;
		}else{
			printf("잘못된 입력입니다.\n");
		}
	}

	if(cnt==0){
		printf("조건을 만족하는 도서가 없습니다.\n");
	}
}

/*------------------------------------
int BsearchType(): 도서검색타입 선택함수 
-------------------------------------*/
int BsearchType(){
	SEARCHTYPE t;
	
	printf("1.도서명 검색 2.출판사명 검색\n3.ISBN 검색   4.저자명 검색\n5.전체검색    6.나가기\n");
	printf("번호를 입력하세요:");
	scanf("%d",&t);
	if(G_numOfBook==0){
		t = -1;
	}
	return t;
}

/*------------------------------------
void borrowBook(): 도서대여함수 
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
		printf("대여처리 완료.\n");
	}
}

/*------------------------------------
BORROW* tempBorrow(): 대여정보 입력 함수 
-------------------------------------*/ 
BORROW* tempBorrow(){
	char Cnum[10], bookName[41];
	int i,bookIndex,cnt,cnt2,choice;
	cnt = 0;
	cnt2 = 0;
	BORROW *temp;
	temp = (BORROW*)malloc(sizeof(BORROW));
	
	printf("대여자의 학번입력:");
	scanf("%s",Cnum);
	for(i=0;i<G_numOfClient;i++){
		if(strcmp(Cnum,G_client[i]->num)==0){
			strcpy(temp->num,Cnum);
			break;
		}
	}
	if(i==G_numOfClient){
		printf("해당 학번의 회원이 존재하지 않습니다.\n");
		return NULL;
	}
	
	while(1){
		printf("1.도서명 검색 2.ISBN 검색\n");
		printf("번호를 입력하세요:");
		scanf("%d",&choice);
		if(choice==1||choice==2){
			break;	
		}
	}
	if(choice==1){
		printf("대여해줄 책 이름:");
		scanf("%s",bookName);
		for(i=0;i<G_numOfBook;i++){
			if(strcmp(bookName,G_book[i]->name)==0){
				cnt++;
				printf("%d\n",cnt); 
				printf("도서번호:%d",G_book[i]->index);
				printf("도서명:%s ",G_book[i]->name);
				printf("출판사:%s ",G_book[i]->publ);
				printf("저자:%s ",G_book[i]->author);
				printf("ISBN:%s ",G_book[i]->isbn);
				printf("보관위치%s ",G_book[i]->storage);
				printf("대여가능:%c\n",G_book[i]->available);
			}
		}
	}else{
		printf("대여해줄 책의 ISBN:");
		scanf("%s",bookName);
		for(i=0;i<G_numOfBook;i++){
			if(strcmp(bookName,G_book[i]->isbn)==0){
				cnt++;
				printf("%d\n",cnt);
				printf("도서번호:%d",G_book[i]->index);
				printf("도서명:%s ",G_book[i]->name);
				printf("출판사:%s ",G_book[i]->publ);
				printf("저자:%s ",G_book[i]->author);
				printf("ISBN:%s ",G_book[i]->isbn);
				printf("보관위치%s ",G_book[i]->storage);
				printf("대여가능:%c\n",G_book[i]->available);
			}
		}
	}
	if(cnt==0){
		printf("해당 도서가 존재하지 않습니다.\n");
		return NULL;
	}
	printf("몇번째 책을 대여할지 입력:");
	scanf("%d",&bookIndex);
	if(bookIndex<=0||bookIndex>cnt){
		printf("해당 데이터가 존재하지 않습니다.\n");
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
						
						printf("도서번호:%d ,",G_book[i]->index);
						printf("도서명:%s ,",G_book[i]->name);
						printf("ISBN:%s\n",G_book[i]->isbn);
						G_book[i]->available = 'N';
					}else{
						printf("이미 대여중인 도서입니다.\n");
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
						
						printf("도서번호:%d ,",G_book[i]->index);
						printf("도서명:%s ,",G_book[i]->name);
						printf("ISBN:%s\n",G_book[i]->isbn);
						G_book[i]->available = 'N';
					}else{
						printf("이미 대여중인 도서입니다.\n");
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
		printf("해당 index를 가진 도서가 존재하지 않습니다.\n");
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
void setDate(int *date): 날짜 설정 함수 
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
void returnBook(): 도서 반납 함수 
-------------------------------------*/ 
void returnBook(){
	int i, j, cnt, cnt2, numExist;
	char  Cnum[10];
	cnt = 0;
	cnt2 = 0;
	numExist = 0;
	fflush(stdin);
	printf(">>도서반납<<\n");
	printf("학번을 입력하세요:");
	scanf("%s",Cnum);
	for(i=0;i<G_numOfClient;i++){
		if(strcmp(Cnum,G_client[i]->num)==0){
			numExist = 1;
			break;
		}
	}
	if(numExist!=1){
		printf("해당학번을 가진 고객이 존재하지 않습니다.\n");
		return;
	}
	printf("%s님의 대여 목록\n",Cnum);
	
	for(i=0;i<G_numOfBorrow;i++){
		if(strcmp(G_borrow[i]->num,Cnum)==0 && strcmp(G_borrow[i]->returnNum,G_borrow[i]->num)!=0){
			cnt++;
			printf("%d\n",cnt);
			printf("|도서번호:%d ",G_borrow[i]->bk->index);
			printf("|도서명:%s\n",G_borrow[i]->bk->name);
			printf("대여일:");
			for(j=0;j<6;j++){
				printf("-%d-",G_borrow[i]->rentDay[j]);
			}
			printf("반납기일:");
			for(j=0;j<6;j++){
				printf("-%d-",G_borrow[i]->returnDay[j]);
			}
			printf("\n");
		}	
	}
	if(cnt==0){
		printf("현재 대여중인 도서가 없습니다.\n");
	}else{
		cnt = 0;
		printf("몇번째 도서를 반납하시겠습니까?");
		scanf("%d",&cnt2);
		for(i=0;i<G_numOfBorrow;i++){
			if(strcmp(G_borrow[i]->num,Cnum)==0){
				cnt++;
				if(cnt==cnt2){
					setDate(G_borrow[i]->returnDay);
					G_borrow[i]->bk->available = 'Y';
					strcpy(G_borrow[i]->returnNum,Cnum);
					printf("반납이 완료되었습니다.\n");
				}
			}
		}
	}
}

/*------------------------------------
void clientList(): 회원 목록 출력함수 
-------------------------------------*/
void clientList(){
	int i, choice;
	char Cname[9],Cnum[10];
	
	while(1){
		printf(">>회원 목록<<\n");
		printf("1.이름 검색 2.학번 검색\n3.전체 검색 4.이전 메뉴\n");
		printf("번호를 입력하세요:");
		scanf("%d",&choice);
		if(choice>0&&choice<=4){
			break;
		}
	}
	if(choice==1){
		printf("검색할 회원의 이름:");
		scanf("%s",Cname);
		for(i=0;i<G_numOfClient;i++){
			if(strcmp(Cname,G_client[i]->name)==0){
				printf("학번:%s ",G_client[i]->num);
				printf("이름:%s \n",G_client[i]->name);
				printf("주소:%s ",G_client[i]->addr);
				printf("전화번호:%s\n",G_client[i]->phone);
			}
		}
	}else if(choice==2){
		printf("검색할 회원의 학번:");
		scanf("%s",Cnum);
		for(i=0;i<G_numOfClient;i++){
			if(strcmp(Cnum,G_client[i]->num)==0){
				printf("학번:%s ",G_client[i]->num);
				printf("이름:%s \n",G_client[i]->name);
				printf("주소:%s",G_client[i]->addr);
				printf("전화번호:%s\n",G_client[i]->phone);
			}
		}
	}else if(choice==3){
		printf("전체 회원 목록\n");
		for(i=0;i<G_numOfClient;i++){
			printf("학번:%s ",G_client[i]->num);
			printf("이름:%s \n",G_client[i]->name);
			printf("주소:%s",G_client[i]->addr);
			printf("전화번호:%s\n\n",G_client[i]->phone);
		}
		printf("총 회원수:%d\n",G_numOfClient);
	}
	
}
