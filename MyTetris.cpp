#include <iostream>
#include "MyTetris.h"
using namespace std;

int main()
{
	//�����Ϣ��ʼ��
	int pos1=0,pos2=1;
	string name1,name2;
	cout<<"���������1�ǳƣ�";  cin>>name1;
	cout<<"���������2�ǳƣ�";  cin>>name2;
	Player p1(name1,pos1),p2(name2,pos2); 
	
	//��������Ϸ��ʼ�� 
	Game game; 
	Block Bk1,Bk2;
	
	game.Initial_screen();
	
	//���ɵ�һ������ 
	Bk1.generate(p1);
    Bk2.generate(p2);
	game.update_screen(p1,p2);
	
	while (1)//��ѭ��
    {
		game.playing(p1,p2,Bk1,Bk2);
		if(game.over(p1,p2)) break; 
    }
	return 0;
}
