#include <iostream>
#include <windows.h>
#include <conio.h> 
#include <time.h>
#include "MyTetris.h"
#define WIDTH 25//x j
#define HEIGHT 25//y i
//[y][x]  [HEIGHT][WIDTH]  [i][j]  
#define P1_Left 10  //���1����߽�   
#define P2_Right 15  //���2���б߽� 

typedef enum map_situation  	//��ͼ��� 
{
    empty,fallen,moving		 // empty��ʾ��ͼ���޷��飬fallen��ʾ�����·��飬moving��ʾ��ǰ�����ƶ��ķ��� 
}map_situation;

typedef enum Shape_Type 		//�������� ������ʹ������������·��� 
{
    O_Shape, J_Shape, L_Shape, T_Shape, I_Shape, S_Shape, Z_Shape
}Shape_Type;

HANDLE hdl;
char map[HEIGHT][WIDTH];	//��ʾ����Ϊ0,5��10,25 
Shape_Type now_shape; 		//ʹ��generate���� �²���һ������ʱ��״ 

COORD next_faller[4];   //���ڲ��Ե�ǰ�����ܷ���е���ķ��飬��Ϊ���з�������� 
bool keyboard_flag;   //�ж϶��ڼ��̰��������Ƿ�ɹ� 

short o_shape[8] = { 4,3,5,3,4,4,5,4 }; 	//ö�ٷ������� 
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
	position=pos;    	//	��¼����������ң����ڿ��Ʒ�������  0Ϊ�� 1Ϊ�� 
}


void Player::add_score(int x) //���ﴫ���x Ӧ�����������ص����� 
{
	score+=x; 
} 


void BackGround::Initial_screen() 
{
	//������
	hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci = { 1,0 };
    SetConsoleCursorInfo(hdl, &cci);
    SetConsoleTitle("MyTetris");
    
    //���������
    srand((unsigned)time(NULL));
    
    //��ͼ��ʼ��
    memset(map, 0, WIDTH * HEIGHT * sizeof(char));
}


void Game::update_screen(Player &p1,Player &p2) 
{
	//system("CLS");
    COORD pos_start= { 0,0 };
    SetConsoleCursorPosition(hdl, pos_start);
    
    for (int i = 0; i < HEIGHT; i++) 																				//����Ȼ�ǵø� 
    {
        cout << "<|" ;
        for (int j = 0; j < P1_Left ; j++)
        {
            if (map[i][j] == empty) cout << ". " ;
            else cout << "��" ;
        }
        cout << "|>              " ;
        
        cout << "<|" ;
        for (int j = P2_Right ; j < WIDTH; j++)
        {
            if (map[i][j] == empty) cout << ". " ;
            else cout<< "��" ;
        }
        cout << "|>" << endl;
    }
    
    cout << "<---------------------->              <---------------------->" << endl ;  
    cout << "score:" << p1.score << "                               " << "score:" << p2.score << endl ;   //����߿���ƻ��е����� ����ʱ������ 
    cout <<	p1.name << "                                   " << p2.name << endl ;
    cout << "������ʽ��WASD" <<  "                        " << "������ʽ��С������������" ; 
}


void Block::generate(Player &p) //�������� 
{
	now_shape = (Shape_Type) (rand() % 7);
	short shape_for_ope[8];
											//���մ�����󣬸ı䷽�����ꡣ
											//positionΪ0������ߵ���ң�Ϊ1�����ұߵ���� 
	for(int i = 0 ; i < 8 ; i++)
	{
		if( i % 2 == 0)                 //������Ʒ����x���꣬��Ϊ���1�����2��y���궼һ����x�����п�� 
		shape_for_ope[i] = shape[now_shape][i] + p.position * 15;
		else 								//������Ʒ����y���� 
		shape_for_ope[i] = shape[now_shape][i];		
	}
	
    memcpy(&faller, shape_for_ope , 4 * sizeof(COORD));
    for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = moving;
}

bool Block::try_fall() //��Ȼ���� 
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

bool Block::try_fast_fall() //ֱ������ 
{
	bool result = 0;
    while (try_fall() == 1)  //ֻҪ�ܹ������һֱ���䣬����ֻҪ������һ�ξͱ�ʾ�ú����ɹ����� 
	{
		result = 1;             
	}
    return result;
}

bool Block::try_left_or_right(int direction) //�����ƶ�  ����1Ϊ�� -1Ϊ�� 
{
	memcpy(next_faller, faller, sizeof(faller));  
    for (int i = 0; i < 4; i++)   //�ж���һ���ƶ��ķ����Ƿ�Ϸ� 
    {
        next_faller[i].X+=direction;
        if ( is_legal(next_faller)==0 ) return 0; //���Ϸ����޷����������ƶ� 
    }
    keyboard_flag = 1;
    for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = empty; //�Ϸ��Ļ�ԭ����λ�þ�Ϊ�� 
    memcpy(faller, next_faller, sizeof(faller));   						// ��������λ�þ��������ƶ�״̬ 
    for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = moving;
    return 1;
}

bool Block::is_legal(COORD test[4])  //�ж���һ�����ƶ��Ƿ�Ϸ� 
{
	for (int i = 0; i < 4; i++)
    {
    	if (map[test[i].Y][test[i].X] == fallen ) return 0;
        if (test[i].X < 0 || test[i].X >= WIDTH || (test[i].X >= P1_Left && test[i].X < P2_Right) ) return 0;
        if (test[i].Y >= HEIGHT ) return 0;
    }
    return 1;
}


