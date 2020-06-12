#include <iostream>
#include "MyTetris.h"
using namespace std;

int main()
{
	//玩家信息初始化
	int pos1=0,pos2=1;
	string name1,name2;
	cout<<"请输入玩家1昵称：";  cin>>name1;
	cout<<"请输入玩家2昵称：";  cin>>name2;
	Player p1(name1,pos1),p2(name2,pos2); 
	
	//背景、游戏初始化 
	Game game; 
	Block Bk1,Bk2;
	
	game.Initial_screen();
	
	//生成第一个方块 
	Bk1.generate(p1);
    Bk2.generate(p2);
	game.update_screen(p1,p2);
	
	while (1)//主循环
    {
		game.playing(p1,p2,Bk1,Bk2);
		if(game.over(p1,p2)) break; 
    }
	return 0;
}
