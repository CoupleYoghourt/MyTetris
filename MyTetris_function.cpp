#include <iostream>
#include <Windows.h>
#include <conio.h> 
#include <time.h>
#include <iomanip>
#include "MyTetris.h"
#define WIDTH 25//x j
#define HEIGHT 25//y i
//[y][x]  [HEIGHT][WIDTH]  [i][j]  
#define P1_Left 10  //玩家1的左边界   
#define P2_Right 15  //玩家2的右边界 
  
HANDLE hdl;
char map[HEIGHT][WIDTH];	//显示区域为0,5到10,25
COORD next_faller[4];   //用于测试当前方块能否进行掉落的方块，可为所有方块类调用 
char key;
bool keyboard_flag;   //判断对于键盘按键反馈是否成功

typedef enum map_situation  	//地图情况 
{
    empty,fallen,moving	// empty表示地图上无方块，fallen表示已落下方块，moving表示当前正在移动的方块								
}map_situation;
map_situation rand_situation; //用于增加他人最后一行时，随机产生地图情况

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



void Game::playing(Player &p1,Player &p2,Block &block1,Block &block2)//需改动 
{
	now_time = GetTickCount();	//获取当前时间 
        
	if (keyboard_flag)	//按键反馈为 1，说明方块操作成功 
    {
        keyboard_flag = 0;
        update_screen(p1,p2);
    }
    if (now_time > last_time)
    {
        last_time += update_time;	//last_time 变为下一次要刷新屏幕的时间 
        if ( block1.try_fall() == 0 )	//不能尝试自然下落，说明要创建一个新的方块了 
        {
            for (int i = 0; i < 4; i++) map[block1.faller[i].Y][block1.faller[i].X] = fallen;	//原来的方块设为"已经下落状态" 			 
			add_row( p2, clear_row(p1) );
            block1.generate(p1);
        }
        if ( block2.try_fall() == 0 )
        {
            for (int i = 0; i < 4; i++) map[block2.faller[i].Y][block2.faller[i].X] = fallen;
			add_row( p1, clear_row(p2) );	
            block2.generate(p2);
        }
        update_screen(p1,p2);
    }
    while (kbhit())
    {
        key = getch();		
        switch (key)
        {
        case 'W':case 'w':
            if (block1.try_spin() == 1)keyboard_flag = 1;	//旋转 
            break;
        case 'A':case 'a':
        	if (block1.try_left_or_right(-1) == 1)keyboard_flag = 1;	//	左移 
        	break;
        case 'S':case 's':
            if (block1.try_fast_fall() == 1)keyboard_flag = 1;	//直接下落 
            break;
        case 'D':case 'd':
            if (block1.try_left_or_right(1) == 1)keyboard_flag = 1; //右移 
            break;
        case -32:
			//按上下左右键会返还两个字符，第一个字符是固定的，是-32，表示获得了一个上下左右字符；
        	key = getch();//第二个字符决定具体是哪个，上下左右分别对应的是72 80 75 77； 
        	switch (key)
        	{
        	case 72:
        		if (block2.try_spin() == 1)keyboard_flag = 1;	//旋转 
           		break;
        	case 80:
           		if (block2.try_fast_fall() == 1)keyboard_flag = 1;	//直接下落 
            	break;
        	case 75:
            	if (block2.try_left_or_right(-1) == 1)keyboard_flag = 1;	//	左移 
        		break;
        	case 77:
        		if (block2.try_left_or_right(1) == 1)keyboard_flag = 1; //右移 
            	break;
			}	
        }
    }
}

void Game::Initial_screen() 
{
	//清除光标
	hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci = { 1,0 };
    SetConsoleCursorInfo(hdl, &cci);
    SetConsoleTitle("MyTetris");
    
    //随机数种子
    srand((unsigned)time(NULL));
    
    //初始化时间 
    last_time = GetTickCount();
    
	//设置刷新屏幕的间隔 
	update_time = 500; 
	
    //初始化按键检测 
    keyboard_flag = 0;
    
    //地图初始化
    memset(map, 0, WIDTH * HEIGHT * sizeof(char));
}

void Game::update_screen(Player &p1,Player &p2) 
{
	//system("CLS");
    COORD pos_start= { 0,0 };
    SetConsoleCursorPosition(hdl, pos_start);
    
    for (int i = 5; i < HEIGHT; i++) 																			
    {
    	SetConsoleTextAttribute(hdl, FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);
        cout << "<|" ;
        for (int j = 0; j < P1_Left ; j++)
        {     	
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_BLUE);
            if (map[i][j] == empty) cout << ". " ;
            else cout << "■" ;
        }
        SetConsoleTextAttribute(hdl, FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);
        cout << "|>              " ;
        
        SetConsoleTextAttribute(hdl, FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);
        cout << "<|" ;
        for (int j = P2_Right ; j < WIDTH; j++)
        {    	
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_BLUE);
            if (map[i][j] == empty) cout << ". " ;
            else cout<< "■" ;
        }
        SetConsoleTextAttribute(hdl, FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);
        cout << "|>" << endl;
    }
    
    cout << "<---------------------->              <---------------------->" << endl ;  
    cout << "score:" << p1.score << setw(37) << setfill(' ') << "score:" << p2.score << endl ;   
    cout <<	"玩家1姓名:" << p1.name << setw(35) << setfill(' ') << "玩家2姓名:" <<p2.name << endl ;
    cout << "操作方式：WASD" << "                        " << "操作方式：小键盘上下左右" <<'\n'<<endl; 
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

bool Block::try_spin() //顺时针旋转 
{
	if (now_shape == O_Shape) return 0; //o形方块也就是正方形方块的旋转无需操作，直接返回0
										 
    else if (now_shape == T_Shape)	//T字形方块以横竖交点的那个小方块为中心点进行旋转 
    {
    	for (int j = 0; j < 4; j++)
    	{
        	next_faller[j].X = faller[0].X + (faller[0].Y - faller[j].Y);
        	next_faller[j].Y = faller[0].Y + (faller[j].X - faller[0].X);
    	}
    	if (is_legal(next_faller))
    	{
        	for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = empty;
        	memcpy(faller, next_faller, sizeof(faller));
        	for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = moving;
        	return 1;
    	}
	} 
    else			//除了o形和T字形外的别的方块 
    {
    	for (int i = 0; i < 4; i++) 	//这个循环确定中心小方块 
        {	//i为中心方块 （存在可能以某个小方块为中心方块不能旋转，但以另一个小方块为中心方块又可以旋转的情况，所以要遍历每个小方块） 
            for (int j = 0; j < 4; j++) //这个循环确定以某个小方块为中心方块下，其他小方块的坐标变换 
            {	//j为所有小方块的遍历 
                next_faller[j].X = faller[i].X + (faller[i].Y - faller[j].Y);
                next_faller[j].Y = faller[i].Y + (faller[j].X - faller[i].X);
            }
            if (is_legal(next_faller))	//判断旋转后的方块是否合法 
            {
                for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = empty;	//如果合法，那么原来的位置的状态就是空的 
                memcpy(faller, next_faller, sizeof(faller));
                for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = moving;	//旋转后的位置设置为"正在移动状态" 
                return 1;
            }
        }
	}
    return 0; 	//	如果上述情况都不满足，那么说明这时候方块无法旋转，返回0 
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

int Game::clear_row(Player &p)
{
	int add_line = 0; //这个变量用来记录另一位玩家需要增加的行数
	int bianjie1 = 0 , bianjie2 = P1_Left;
		 
	if(p.position == 1)	//	判断是哪位玩家，然后更改边界，如果是玩家1就用默认的边界，玩家2才需更改 
	{
		bianjie1 = P2_Right;
		bianjie2 = WIDTH;
	}
	
    for (int i = HEIGHT - 1; i >= 5; i--)	//从最后一行开始，一行一行判断是否全是fallen状态 
    {	
    	int flag=1; 
    	
        for (int j = bianjie1; j < bianjie2; j++)
        {
            if (map[i][j] != fallen)
            {
                flag = 0;	//只要有一个不是fallen状态，flag就为0，否则说明那一行全是fallen状态，需要消除 
                break;
            }
        }
        
        if(flag)
        {
    	    p.score++;          //直接加分 
    	    add_line++;
			for(int k = i ; k >= 5; k--)
			{
                for(int j = bianjie1; j < bianjie2; j++)
				{
                	map[k][j]=map[k-1][j];
				}
			}
			i++; //这个i++十分重要，一行消完之后有可能下一行依然需要消，如果没有这个i++，就会直接跳到再下一行去 
    	}
    }
    return add_line; 
} 

void Game::add_row(Player &p,int add_line)
{
	int bianjie1 = 0 , bianjie2 = P1_Left;
		 
	if(p.position == 1)	//	判断是哪位玩家，然后更改边界，如果是玩家1就用默认的边界，玩家2才需更改 
	{
		bianjie1 = P2_Right;
		bianjie2 = WIDTH;
	}
	
    for(int l = 0; l < add_line; l++)
	{
	    char temp_map[HEIGHT][WIDTH];
    	memcpy(temp_map, map, HEIGHT * WIDTH * sizeof(char) );
    	
        for(int i = 5 ; i < HEIGHT-1; i++)	
     	{
            for(int j = bianjie1; j < bianjie2; j++)
     		{
     			//更新地图的时候，只有fallen和empty才需要拷贝过去 
     			if(map[i][j] != moving && map[i-1][j] != moving && map[i][j-1] != moving && map[i][j+1] != moving && map[i+1][j] != moving)
                map[i][j]=temp_map[i+1][j];	//当前这一行变为原来的下一行的情况，一行一行上推 
     		}
    	}
    	for(int j = bianjie1; j < bianjie2; j++)	//最后一行的随机生成单独写 
    	{
	    	rand_situation = (map_situation)(rand() % 2); //最后一行的随机増行只能是empty或者fallen类型的 
	    	map[HEIGHT-1][j] = rand_situation;
    	}
    }
}

bool Game::over(Player &p1,Player &p2)
{
	for(int i=0 ; i < WIDTH ; i++)
	{
		if( map[5][i] == fallen ) 
		{
			SetConsoleTextAttribute(hdl, FOREGROUND_INTENSITY|FOREGROUND_RED);
			cout<<"游戏结束！！！你我皆是赢家！！！"<<'\n'<<endl;
			SetConsoleTextAttribute(hdl,FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN);
			cout<<p1.name<<" 得分:"<<p1.score<<"   "<<p2.name<<" 得分:"<<p2.score<<'\n'<<endl;
			SetConsoleTextAttribute(hdl, FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);
			cout<<"MyTetris made by 陈泽宇,郑浩彬"<<endl; 
			return 1; 
		}
	}
	return 0;
}

