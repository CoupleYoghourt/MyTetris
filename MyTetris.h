#include <Windows.h>
using namespace std;
//未完成函数：方块旋转，消行增行，按键获取，计时函数 
class Player;

class Game
{
public:
	void player_order();			
	void update_screen(Player &a,Player &b);     //暂时完成 
};

class Player //基本完成 
{
public:
	Player(string Name,int pos);    //暂时完成 
	void add_score(int x); //暂时完成 
	friend class Game;
	friend class Block;
private:
	string name;
	int score;
	int position;	
};


class BackGround
{
public:
	void Initial_screen(); //暂时完成 
};


class Block
{
public:
	void generate(Player &p);           //暂时完成    //以下的移动操作，返回1为成功，为0为未成功 
	bool try_fall();//尝试自然下落       //暂时完成         
	bool try_fast_fall();//尝试直接下落  //暂时完成 
	bool try_spin();//尝试旋转 
	bool try_left_or_right(int direction); //尝试左右移动     暂时完成 
	int clear_row();  //要返回消行数，方便加积分 
	bool is_legal(COORD test[4]); //暂时完成 
private:
	COORD faller[4];    //表示正在掉落的方块，每个方块对象的当前方块都有一个faller来存储 
						//这样就可以用此来区分玩家1和玩家2的方块 
};
