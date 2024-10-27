#include "common.h" 
#include "function.h"
#include "admin.h"
#include "client.h"

typedef enum loginChoice{join=1,login,quit,topRate,borrowRate,EReturn,term} LOGINCHOICE;	//메뉴선택 enum 
typedef enum loginType{admin=1,user} LOGINTYPE;		//admin,user 로그인 구분 enum 

CLIENT** G_client;			//고객 
int G_numOfClient = 0;		//고객수 
BOOK** G_book; 				//도서 
int G_numOfBook = 0;		//도서수 
BORROW** G_borrow;			//대여 
int G_numOfBorrow = 0;		//대여수 
 
 
int main(int argc, char *argv[]) {
	LOGINCHOICE c;
	LOGINTYPE t;
	
	//실행 후 초기 고객,도서,대여 이중포인터 할당
	create();
	
	//printf("1.회원가입 2.로그인 3.프로그램 종료 4.인기도서 5.최다대여회원 6.조기반납회원 7.대여기간별 조회");
	
	while((c=menu())!=quit){
		if(c==join){				//1.회원가입
			Join();
		}else if(c==login){			//2.로그인
			Login();	
		}else if(c==topRate){		//4.인기도서 
			top3();
		}else if(c==borrowRate){	//5.최다대여회원 
			mostBorrow();
		}else if(c==EReturn){		//6.조기반납회원 
			earlyReturn();
		}else if(c==term){			//7.대여기간별 조회 
			returnTerm();
		}else{						//잘못된 입력 
			printf("다시입력\n");
		}
	}
	
	//프로그램 종료 전 변경사항 파일에 기록 
	uploadFile();
	
	//할당한 이중포인터 할당 해제 
	free(G_client);
	free(G_book);
	free(G_borrow);
	
	return 0;
}



