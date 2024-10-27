#include "common.h" 
#include "function.h"
#include "admin.h"
#include "client.h"

typedef enum loginChoice{join=1,login,quit,topRate,borrowRate,EReturn,term} LOGINCHOICE;	//�޴����� enum 
typedef enum loginType{admin=1,user} LOGINTYPE;		//admin,user �α��� ���� enum 

CLIENT** G_client;			//�� 
int G_numOfClient = 0;		//���� 
BOOK** G_book; 				//���� 
int G_numOfBook = 0;		//������ 
BORROW** G_borrow;			//�뿩 
int G_numOfBorrow = 0;		//�뿩�� 
 
 
int main(int argc, char *argv[]) {
	LOGINCHOICE c;
	LOGINTYPE t;
	
	//���� �� �ʱ� ��,����,�뿩 ���������� �Ҵ�
	create();
	
	//printf("1.ȸ������ 2.�α��� 3.���α׷� ���� 4.�α⵵�� 5.�ִٴ뿩ȸ�� 6.����ݳ�ȸ�� 7.�뿩�Ⱓ�� ��ȸ");
	
	while((c=menu())!=quit){
		if(c==join){				//1.ȸ������
			Join();
		}else if(c==login){			//2.�α���
			Login();	
		}else if(c==topRate){		//4.�α⵵�� 
			top3();
		}else if(c==borrowRate){	//5.�ִٴ뿩ȸ�� 
			mostBorrow();
		}else if(c==EReturn){		//6.����ݳ�ȸ�� 
			earlyReturn();
		}else if(c==term){			//7.�뿩�Ⱓ�� ��ȸ 
			returnTerm();
		}else{						//�߸��� �Է� 
			printf("�ٽ��Է�\n");
		}
	}
	
	//���α׷� ���� �� ������� ���Ͽ� ��� 
	uploadFile();
	
	//�Ҵ��� ���������� �Ҵ� ���� 
	free(G_client);
	free(G_book);
	free(G_borrow);
	
	return 0;
}



