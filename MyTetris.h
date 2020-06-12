#include <Windows.h>
using namespace std;
 
class Player;
class Block;

typedef enum Shape_Type 		//�������� ������ʹ������������·��� 
{
    O_Shape, J_Shape, L_Shape, T_Shape, I_Shape, S_Shape, Z_Shape
}Shape_Type;

class Game
{
public:
	void playing(Player &p1,Player &p2,Block &block,Block &block2);
	bool over(Player &p1,Player &p2);		
	int clear_row(Player &p);  //���ӻ��ֵĹ����ڴ˴���ɣ�������Ҫ�������������Ա���Է�����	
	void add_row(Player &p,int add_line); 	
	void update_screen(Player &a,Player &b);    
	void Initial_screen(); 
	ULONGLONG last_time; 	
	ULONGLONG now_time;
	ULONGLONG update_time; //�����ˢ��һ����Ļ 
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
	void generate(Player &p);                //���µ��ƶ�����������1Ϊ�ɹ���Ϊ0Ϊδ�ɹ� 
	bool try_fall();						//������Ȼ����               
	bool try_fast_fall();					//����ֱ������  
	bool try_spin();						//������ת 									
	bool try_left_or_right(int direction); //���������ƶ�    
	bool is_legal(COORD test[4]); 
	friend class Game;
	friend class Player;
private:
	COORD faller[4];    //��ʾ���ڵ���ķ��飬ÿ���������ĵ�ǰ���鶼��һ��faller���洢 
						//�����Ϳ����ô����������1�����2�ķ��� 
	Shape_Type now_shape; //ʹ��generate���� �²���һ������ʱ��״ 
};
