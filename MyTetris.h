#include <Windows.h>
using namespace std;
//δ��ɺ�����������ת���������У�������ȡ����ʱ���� 
class Player;

class Game
{
public:
	void player_order();			
	void update_screen(Player &a,Player &b);     //��ʱ��� 
};

class Player //������� 
{
public:
	Player(string Name,int pos);    //��ʱ��� 
	void add_score(int x); //��ʱ��� 
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
	void Initial_screen(); //��ʱ��� 
};


class Block
{
public:
	void generate(Player &p);           //��ʱ���    //���µ��ƶ�����������1Ϊ�ɹ���Ϊ0Ϊδ�ɹ� 
	bool try_fall();//������Ȼ����       //��ʱ���         
	bool try_fast_fall();//����ֱ������  //��ʱ��� 
	bool try_spin();//������ת 
	bool try_left_or_right(int direction); //���������ƶ�     ��ʱ��� 
	int clear_row();  //Ҫ����������������ӻ��� 
	bool is_legal(COORD test[4]); //��ʱ��� 
private:
	COORD faller[4];    //��ʾ���ڵ���ķ��飬ÿ���������ĵ�ǰ���鶼��һ��faller���洢 
						//�����Ϳ����ô����������1�����2�ķ��� 
};
