#include <iostream>
#include <windows.h>
#include <conio.h> 
#include <time.h>
#include "MyTetris.h"
#define WIDTH 25//x j
#define HEIGHT 25//y i
//[y][x]  [HEIGHT][WIDTH]  [i][j]  
#define P1_Left 10  //玩家1的左边界   
#define P2_Right 15  //玩家2的有边界 

typedef enum map_situation  	//地图情况 
{
    empty,fallen,moving		 // empty表示地图上无方块，fallen表示已落下方块，moving表示当前正在移动的方块 
}map_situation;

typedef enum Shape_Type 		//方块种类 ，便于使用随机数产生新方块 
{
    O_Shape, J_Shape, L_Shape, T_Shape, I_Shape, S_Shape, Z_Shape
}Shape_Type;

HANDLE hdl;
char map[HEIGHT][WIDTH];	//显示区域为0,5到10,25 
Shape_Type now_shape; 		//使用generate（） 新产生一个方块时形状 

COORD next_faller[4];   //用于测试当前方块能否进行掉落的方块，可为所有方块类调用 
bool keyboard_flag;   //判断对于键盘按键反馈是否成功 

short o_shape[8] = { 4,3,5,3,4,4,5,4 }; 	//枚举方块坐标 
short j_shape[8] = { 4,4,5,4,6,4,4,3 };
short l_shape[8] = { 5,4,4,4,3,4,5,3 };
short t_shape[8] = { 5,4,4,4,6,4,5,3 };
short i_shape[8] = { 4,4,5,4,3,4,6,4 };
short s_shape[8] = { 4,3,5,3,4,2,5,4 };
short z_shape[8] = { 4,3,5,3,5,2,4,4 };

short shape[7][8] =
{
    { 4,3,5,3,4,4,5,4 },
    { 4,4,5,4,6,4,4,3 },
    { 5,4,4,4,3,4,5,3 },
    { 5,4,4,4,6,4,5,3 },
    { 4,4,5,4,3,4,6,4 },
    { 4,3,5,3,4,2,5,4 },
    { 4,3,5,3,5,2,4,4 },
};


Player::Player(string Name,int pos)
{
	name=Name;
	score=0;
	position=pos;    	//	记录玩家在左还是右，便于控制方块坐标  0为左 1为右 
}


void Player::add_score(int x) //这里传入的x 应是消行所返回的行数 
{
	score+=x; 
} 


void BackGround::Initial_screen() 
{
	//清除光标
	hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci = { 1,0 };
    SetConsoleCursorInfo(hdl, &cci);
    SetConsoleTitle("MyTetris");
    
    //随机数种子
    srand((unsigned)time(NULL));
    
    //地图初始化
    memset(map, 0, WIDTH * HEIGHT * sizeof(char));
}


void Game::update_screen(Player &p1,Player &p2) 
{
	//system("CLS");
    COORD pos_start= { 0,0 };
    SetConsoleCursorPosition(hdl, pos_start);
    
    for (int i = 0; i < HEIGHT; i++) 																				//这里等会记得改 
    {
        cout << "<|" ;
        for (int j = 0; j < P1_Left ; j++)
        {
            if (map[i][j] == empty) cout << ". " ;
            else cout << "■" ;
        }
        cout << "|>              " ;
        
        cout << "<|" ;
        for (int j = P2_Right ; j < WIDTH; j++)
        {
            if (map[i][j] == empty) cout << ". " ;
            else cout<< "■" ;
        }
        cout << "|>" << endl;
    }
    
    cout << "<---------------------->              <---------------------->" << endl ;  
    cout << "score:" << p1.score << "                               " << "score:" << p2.score << endl ;   //这里边框绘制还有点问题 ，暂时先这样 
    cout <<	p1.name << "                                   " << p2.name << endl ;
    cout << "操作方式：WASD" <<  "                        " << "操作方式：小键盘上下左右" ; 
}


void Block::generate(Player &p) //产生方块 
{
	now_shape = (Shape_Type) (rand() % 7);
	short shape_for_ope[8];
											//按照传入对象，改变方块坐标。
											//position为0就是左边的玩家，为1就是右边的玩家 
	for(int i = 0 ; i < 8 ; i++)
	{
		if( i % 2 == 0)                 //这里控制方块的x坐标，因为玩家1和玩家2的y坐标都一样，x坐标有跨度 
		shape_for_ope[i] = shape[now_shape][i] + p.position * 15;
		else 								//这里控制方块的y坐标 
		shape_for_ope[i] = shape[now_shape][i];		
	}
	
    memcpy(&faller, shape_for_ope , 4 * sizeof(COORD));
    for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = moving;
}

bool Block::try_fall() //自然下落 
{
	memcpy(next_faller, faller, sizeof(faller));
    for (int i = 0; i < 4; i++)
    {
        next_faller[i].Y++;
        if ( is_legal(next_faller)==0 ) return 0;
    }
    for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = empty;
    memcpy(faller, next_faller, sizeof(faller));
    for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = moving;
    return 1;
} 

bool Block::try_fast_fall() //直接下落 
{
	bool result = 0;
    while (try_fall() == 1)  //只要能够下落就一直下落，并且只要有下落一次就表示该函数成功调用 
	{
		result = 1;             
	}
    return result;
}

bool Block::try_left_or_right(int direction) //左右移动  传入1为右 -1为左 
{
	memcpy(next_faller, faller, sizeof(faller));  
    for (int i = 0; i < 4; i++)   //判断下一个移动的方块是否合法 
    {
        next_faller[i].X+=direction;
        if ( is_legal(next_faller)==0 ) return 0; //不合法就无法进行左右移动 
    }
    keyboard_flag = 1;
    for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = empty; //合法的话原来的位置就为空 
    memcpy(faller, next_faller, sizeof(faller));   						// 接下来的位置就是正在移动状态 
    for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = moving;
    return 1;
}

bool Block::is_legal(COORD test[4])  //判断下一步的移动是否合法 
{
	for (int i = 0; i < 4; i++)
    {
    	if (map[test[i].Y][test[i].X] == fallen ) return 0;
        if (test[i].X < 0 || test[i].X >= WIDTH || (test[i].X >= P1_Left && test[i].X < P2_Right) ) return 0;
        if (test[i].Y >= HEIGHT ) return 0;
    }
    return 1;
}


