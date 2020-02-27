#include <iostream>
#include <string>
using namespace std;

class student {
private:
	string name = "卢本伟";
	int age = 18;
public:
	student()//无参构造函数
	{
		cout << "姓名：" << name << endl;
		cout << "年龄：" << age << endl;
		cout << endl;
	}
	student(string _name, int _age)//有参构造函数
	{
		name = _name;
		age = _age;
		cout << "姓名：" << name << endl;
		cout << "年龄：" << age << endl;
		cout << endl;
	}
	void get_age()
	{
		cout <<"得到"<< name << "的年龄为：" << age << endl << endl;
	}
	void set_age(int _age)
	{
		age = _age;
	}
	~student()
	{
		cout << "该学生GG" << endl;
	}
};

int main(void)
{
	student s;
	student m("马飞飞", 20);

	s.set_age(666);
	s.get_age();

	m.set_age(888);
	m.get_age();

	system("pause");
	return 0;
}