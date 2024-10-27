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
void clientLogin(int n): n번째 회원로그인 함수 
-------------------------------------*/
void clientLogin(int n){
	char loginPW[20];
	int i, choice, d;
	
	printf("PW입력:");
	scanf("%s",loginPW);
	
	if(strcmp(G_client[n]->pw,loginPW)==0){
		printf("%s님 로그인 성공\n",G_client[n]->name);
		while((choice=clientMenu())!=5){//5.로그 아웃 
			if(choice==1){//1.도서검색
				if(G_numOfBook==0){
					printf("현재 도서관에 도서가 없습니다.\n");
				}else{
					searchBook();
				}
			}else if(choice==2){//2.내 대여목록 
				borrowList(n);
			}else if(choice==3){//3.개인정보 수정 
				editInfo(n);
			}else if(choice==4){//4.회원 탈퇴 
				d = deleteAccount(n);
				if(d==1){
					break;
				}
			}else if(choice==6){//6.프로그램 종료 
				uploadFile();
				exit(1);
			}else{
				printf("잘못된 입력\n");
			}
		}
	}else{
		printf("id혹은 pw가 잘못되었습니다.\n");
	}
}

/*------------------------------------
int clientMenu(): 회원 메뉴 선택 함수 
-------------------------------------*/
int clientMenu(){
	int choice;
	
	printf("1.도서 검색       2.내 대여목록\n3.개인정보 수정   4.회원 탈퇴\n5.로그 아웃       6.프로그램 종료\n");
	printf("\n번호를 입력하세요:");
	scanf("%d",&choice);
	
	return choice;
}

/*------------------------------------
void borrowList(int n): n번째 회원의 대여 목록 함수 
-------------------------------------*/
void borrowList(int n){
	int i, j, cnt;
	cnt = 0;
	printf(">>%s님의 대여 목록<<\n",G_client[n]->name);
	for(i=0;i<G_numOfBorrow;i++){
		printf("%s\n",G_client[n]->num); 
		if(strcmp(G_client[n]->num,G_borrow[i]->num)==0 && strcmp(G_borrow[i]->returnNum,G_borrow[i]->num)!=0){
			printf("if문들어옴");
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
	}
	printf("\n"); 
}

/*------------------------------------
void editInfo(int n): n번째 회원 정보수정 함수 
-------------------------------------*/
void editInfo(int n){
	int i, cnt;
	CLIENT* temp;
	cnt = 0;
	temp = G_client[n];
	printf("--현재 %s님의 정보--\n",temp->name);
	printf("학번:%s ",temp->num);
	printf("비밀번호:%s\n",temp->pw);
	printf("이름:%s ",temp->name);
	printf("주소:%s\n",temp->addr);
	printf("전화번호:%s\n",temp->phone);
	printf("--정보 수정 입력--\n");
	printf("비밀번호:");
	scanf("%s",temp->pw);
	printf("주소:");
	fflush(stdin);
	gets(temp->addr);
	while(1){
		printf("전화번호:");
		scanf("%s",temp->phone);
		for(i=0;i<G_numOfClient;i++){
			if(strcmp(G_client[i]->phone,temp->phone)==0){
				cnt++;
				if(i==n){
					cnt--;
					continue;
				}
				printf("이미 존재하는 전화번호\n");
			}
		}
		if(cnt==0){
			break;
		}
	}
}

/*------------------------------------
int deleteAccount(int n):n번째 회원 탈퇴 함수 
-------------------------------------*/
int deleteAccount(int n){
	int i, j, choice, cnt, del;
	del = 0;
	cnt = 0;
	while(1){
		printf("정말 탈퇴하시겠습니까?\n");
		printf("1.네2.아니오\n");
		scanf("%d",&choice);
		if(choice==1){
			for(i=0;i<G_numOfBorrow;i++){
				if(strcmp(G_borrow[i]->num,G_client[n]->num)==0 && strcmp(G_borrow[i]->returnNum,G_borrow[i]->num)!=0){
					printf("count가 증가됨\n");
					cnt++;
				}
			}
			break;
		}else if(choice!=2){
			printf("잘못된 입력입니다.\n");
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
		printf("회원탈퇴가 완료되었습니다.\n");
		del = 1;
	}else{
		printf("현재 대여중인 도서가 있어서 탈퇴하실 수 없습니다.\n");
		printf("대여중인 도서들을 반납해주세요.\n");
	}
	
	return del;
}


























