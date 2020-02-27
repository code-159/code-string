#include <iostream>
#include <string>

using namespace std;

class game_character{
private:
	string name, sex;
	int wu_g, fa_g, wu_f, fa_f;
public:
	static int equipment_num;
	static string which_game;

	game_character(string _name="007",string _sex="Man",int _wu_g = 0,int _fa_g=0,int _wu_f=0,int _fa_f=0)
					:name(_name),sex(_sex),wu_g(_wu_g),fa_g(_fa_g),wu_f(_wu_f),fa_f(_fa_f)
	{
		cout << "初始化成功!!!" << endl;
		cout << "名字：" << name << endl;
		cout << "性别：" << sex << endl;
		cout << "物攻：" << wu_g << endl;
		cout << "法功：" << fa_g << endl;
		cout << "物防：" << wu_f << endl;
		cout << "法防：" << fa_f << endl;
		if (wu_g == 40 && fa_g == 0)
			cout << "该英雄为近战英雄!" << endl;
		else if (wu_g == 0 && fa_g == 40)
			cout << "该英雄为法师英雄!" << endl;
		else
			cout << "该英雄为傻逼英雄!" << endl;
		cout << endl;
	}
	//加攻击力
	void add_Attack_power(int _wu_g = 0, int _fa_g = 0)
	{
		wu_g+= _wu_g;
		fa_g+= _fa_g;
		cout << "名字：" << name << endl;
		cout << "物攻增加至：" << wu_g << " , 法强增加至：" << fa_g << endl;
		cout << "性别：" << sex << endl;
		cout << "物攻：" << wu_g << endl;
		cout << "法功：" << fa_g << endl;
		cout << "物防：" << wu_f << endl;
		cout << "法防：" << fa_f << endl;
		cout << endl;
	}
	//加防御力
	void add_Defense_power(int _wu_f = 0, int _fa_f = 0)
	{
		wu_f += _wu_f;
		fa_f += _fa_f;
		cout << "名字：" << name << endl;
		cout << "物防增加至：" << wu_f << " , 法防增加至：" << fa_f << endl;
		cout << "性别：" << sex << endl;
		cout << "物攻：" << wu_g << endl;
		cout << "法功：" << fa_g << endl;
		cout << "物防：" << wu_f << endl;
		cout << "法防：" << fa_f << endl;
		cout << endl;
	}

	//静态成员函数，打印出所有静态成员：实现调用打印和类名方式打印
	static void print_static_member()
	{
		cout << "该英雄的装备栏有: " << equipment_num << " 个" << endl;
		cout << "该英雄属于: " << which_game << endl << endl;
	}

	~game_character()
	{
		cout << "英雄已退出游戏!!!" << endl;
	}
};

int game_character::equipment_num = 6;
string game_character::which_game = "LOL";

int main(void)
{
	game_character g1("卢本伟", "Man", 50, 50, 50, 50);
	game_character g2("马飞飞", "Man", 40, 0, 100, 100);

	g1.add_Attack_power(20, 40);
	g2.add_Defense_power(566, 566);

	g1.print_static_member();
	g2.print_static_member();

	system("pause");
	return 0;
}