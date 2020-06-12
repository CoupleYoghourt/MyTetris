#include <Windows.h>
using namespace std;
 
class Player;
class Block;

typedef enum Shape_Type 		//方块种类 ，便于使用随机数产生新方块 
{
    O_Shape, J_Shape, L_Shape, T_Shape, I_Shape, S_Shape, Z_Shape
}Shape_Type;

class Game
{
public:
	void playing(Player &p1,Player &p2,Block &block,Block &block2);
	bool over(Player &p1,Player &p2);		
	int clear_row(Player &p);  //（加积分的工作在此处完成）并且这要返回消行数，以便给对方増行	
	void add_row(Player &p,int add_line); 	
	void update_screen(Player &a,Player &b);    
	void Initial_screen(); 
	ULONGLONG last_time; 	
	ULONGLONG now_time;
	ULONGLONG update_time; //过多久刷新一次屏幕 
};

class Player  
{
public:
	Player(string Name,int pos);   
	friend class Game;
	friend class Block;
private:
	string name;
	int score;
	int position;	
};


class Block
{
public:
	void generate(Player &p);                //以下的移动操作，返回1为成功，为0为未成功 
	bool try_fall();						//尝试自然下落               
	bool try_fast_fall();					//尝试直接下落  
	bool try_spin();						//尝试旋转 									
	bool try_left_or_right(int direction); //尝试左右移动    
	bool is_legal(COORD test[4]); 
	friend class Game;
	friend class Player;
private:
	COORD faller[4];    //表示正在掉落的方块，每个方块对象的当前方块都有一个faller来存储 
						//这样就可以用此来区分玩家1和玩家2的方块 
	Shape_Type now_shape; //使用generate（） 新产生一个方块时形状 
};
