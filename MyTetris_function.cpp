#include <iostream>
#include <Windows.h>
#include <conio.h> 
#include <time.h>
#include <iomanip>
#include "MyTetris.h"
#define WIDTH 25//x j
#define HEIGHT 25//y i
//[y][x]  [HEIGHT][WIDTH]  [i][j]  
#define P1_Left 10  //���1����߽�   
#define P2_Right 15  //���2���ұ߽� 
  
HANDLE hdl;
char map[HEIGHT][WIDTH];	//��ʾ����Ϊ0,5��10,25
COORD next_faller[4];   //���ڲ��Ե�ǰ�����ܷ���е���ķ��飬��Ϊ���з�������� 
char key;
bool keyboard_flag;   //�ж϶��ڼ��̰��������Ƿ�ɹ�

typedef enum map_situation  	//��ͼ��� 
{
    empty,fallen,moving	// empty��ʾ��ͼ���޷��飬fallen��ʾ�����·��飬moving��ʾ��ǰ�����ƶ��ķ���								
}map_situation;
map_situation rand_situation; //���������������һ��ʱ�����������ͼ���

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



void Game::playing(Player &p1,Player &p2,Block &block1,Block &block2)//��Ķ� 
{
	now_time = GetTickCount();	//��ȡ��ǰʱ�� 
        
	if (keyboard_flag)	//��������Ϊ 1��˵����������ɹ� 
    {
        keyboard_flag = 0;
        update_screen(p1,p2);
    }
    if (now_time > last_time)
    {
        last_time += update_time;	//last_time ��Ϊ��һ��Ҫˢ����Ļ��ʱ�� 
        if ( block1.try_fall() == 0 )	//���ܳ�����Ȼ���䣬˵��Ҫ����һ���µķ����� 
        {
            for (int i = 0; i < 4; i++) map[block1.faller[i].Y][block1.faller[i].X] = fallen;	//ԭ���ķ�����Ϊ"�Ѿ�����״̬" 			 
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
            if (block1.try_spin() == 1)keyboard_flag = 1;	//��ת 
            break;
        case 'A':case 'a':
        	if (block1.try_left_or_right(-1) == 1)keyboard_flag = 1;	//	���� 
        	break;
        case 'S':case 's':
            if (block1.try_fast_fall() == 1)keyboard_flag = 1;	//ֱ������ 
            break;
        case 'D':case 'd':
            if (block1.try_left_or_right(1) == 1)keyboard_flag = 1; //���� 
            break;
        case -32:
			//���������Ҽ��᷵�������ַ�����һ���ַ��ǹ̶��ģ���-32����ʾ�����һ�����������ַ���
        	key = getch();//�ڶ����ַ������������ĸ����������ҷֱ��Ӧ����72 80 75 77�� 
        	switch (key)
        	{
        	case 72:
        		if (block2.try_spin() == 1)keyboard_flag = 1;	//��ת 
           		break;
        	case 80:
           		if (block2.try_fast_fall() == 1)keyboard_flag = 1;	//ֱ������ 
            	break;
        	case 75:
            	if (block2.try_left_or_right(-1) == 1)keyboard_flag = 1;	//	���� 
        		break;
        	case 77:
        		if (block2.try_left_or_right(1) == 1)keyboard_flag = 1; //���� 
            	break;
			}	
        }
    }
}

void Game::Initial_screen() 
{
	//������
	hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci = { 1,0 };
    SetConsoleCursorInfo(hdl, &cci);
    SetConsoleTitle("MyTetris");
    
    //���������
    srand((unsigned)time(NULL));
    
    //��ʼ��ʱ�� 
    last_time = GetTickCount();
    
	//����ˢ����Ļ�ļ�� 
	update_time = 500; 
	
    //��ʼ��������� 
    keyboard_flag = 0;
    
    //��ͼ��ʼ��
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
            else cout << "��" ;
        }
        SetConsoleTextAttribute(hdl, FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);
        cout << "|>              " ;
        
        SetConsoleTextAttribute(hdl, FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);
        cout << "<|" ;
        for (int j = P2_Right ; j < WIDTH; j++)
        {    	
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_BLUE);
            if (map[i][j] == empty) cout << ". " ;
            else cout<< "��" ;
        }
        SetConsoleTextAttribute(hdl, FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);
        cout << "|>" << endl;
    }
    
    cout << "<---------------------->              <---------------------->" << endl ;  
    cout << "score:" << p1.score << setw(37) << setfill(' ') << "score:" << p2.score << endl ;   
    cout <<	"���1����:" << p1.name << setw(35) << setfill(' ') << "���2����:" <<p2.name << endl ;
    cout << "������ʽ��WASD" << "                        " << "������ʽ��С������������" <<'\n'<<endl; 
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

bool Block::try_spin() //˳ʱ����ת 
{
	if (now_shape == O_Shape) return 0; //o�η���Ҳ���������η������ת���������ֱ�ӷ���0
										 
    else if (now_shape == T_Shape)	//T���η����Ժ���������Ǹ�С����Ϊ���ĵ������ת 
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
    else			//����o�κ�T������ı�ķ��� 
    {
    	for (int i = 0; i < 4; i++) 	//���ѭ��ȷ������С���� 
        {	//iΪ���ķ��� �����ڿ�����ĳ��С����Ϊ���ķ��鲻����ת��������һ��С����Ϊ���ķ����ֿ�����ת�����������Ҫ����ÿ��С���飩 
            for (int j = 0; j < 4; j++) //���ѭ��ȷ����ĳ��С����Ϊ���ķ����£�����С���������任 
            {	//jΪ����С����ı��� 
                next_faller[j].X = faller[i].X + (faller[i].Y - faller[j].Y);
                next_faller[j].Y = faller[i].Y + (faller[j].X - faller[i].X);
            }
            if (is_legal(next_faller))	//�ж���ת��ķ����Ƿ�Ϸ� 
            {
                for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = empty;	//����Ϸ�����ôԭ����λ�õ�״̬���ǿյ� 
                memcpy(faller, next_faller, sizeof(faller));
                for (int i = 0; i < 4; i++) map[faller[i].Y][faller[i].X] = moving;	//��ת���λ������Ϊ"�����ƶ�״̬" 
                return 1;
            }
        }
	}
    return 0; 	//	�����������������㣬��ô˵����ʱ�򷽿��޷���ת������0 
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

int Game::clear_row(Player &p)
{
	int add_line = 0; //�������������¼��һλ�����Ҫ���ӵ�����
	int bianjie1 = 0 , bianjie2 = P1_Left;
		 
	if(p.position == 1)	//	�ж�����λ��ң�Ȼ����ı߽磬��������1����Ĭ�ϵı߽磬���2������� 
	{
		bianjie1 = P2_Right;
		bianjie2 = WIDTH;
	}
	
    for (int i = HEIGHT - 1; i >= 5; i--)	//�����һ�п�ʼ��һ��һ���ж��Ƿ�ȫ��fallen״̬ 
    {	
    	int flag=1; 
    	
        for (int j = bianjie1; j < bianjie2; j++)
        {
            if (map[i][j] != fallen)
            {
                flag = 0;	//ֻҪ��һ������fallen״̬��flag��Ϊ0������˵����һ��ȫ��fallen״̬����Ҫ���� 
                break;
            }
        }
        
        if(flag)
        {
    	    p.score++;          //ֱ�Ӽӷ� 
    	    add_line++;
			for(int k = i ; k >= 5; k--)
			{
                for(int j = bianjie1; j < bianjie2; j++)
				{
                	map[k][j]=map[k-1][j];
				}
			}
			i++; //���i++ʮ����Ҫ��һ������֮���п�����һ����Ȼ��Ҫ�������û�����i++���ͻ�ֱ����������һ��ȥ 
    	}
    }
    return add_line; 
} 

void Game::add_row(Player &p,int add_line)
{
	int bianjie1 = 0 , bianjie2 = P1_Left;
		 
	if(p.position == 1)	//	�ж�����λ��ң�Ȼ����ı߽磬��������1����Ĭ�ϵı߽磬���2������� 
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
     			//���µ�ͼ��ʱ��ֻ��fallen��empty����Ҫ������ȥ 
     			if(map[i][j] != moving && map[i-1][j] != moving && map[i][j-1] != moving && map[i][j+1] != moving && map[i+1][j] != moving)
                map[i][j]=temp_map[i+1][j];	//��ǰ��һ�б�Ϊԭ������һ�е������һ��һ������ 
     		}
    	}
    	for(int j = bianjie1; j < bianjie2; j++)	//���һ�е�������ɵ���д 
    	{
	    	rand_situation = (map_situation)(rand() % 2); //���һ�е��������ֻ����empty����fallen���͵� 
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
			cout<<"��Ϸ�������������ҽ���Ӯ�ң�����"<<'\n'<<endl;
			SetConsoleTextAttribute(hdl,FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN);
			cout<<p1.name<<" �÷�:"<<p1.score<<"   "<<p2.name<<" �÷�:"<<p2.score<<'\n'<<endl;
			SetConsoleTextAttribute(hdl, FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);
			cout<<"MyTetris made by ������,֣�Ʊ�"<<endl; 
			return 1; 
		}
	}
	return 0;
}

