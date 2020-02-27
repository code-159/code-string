#include <iostream>
#include <memory>

using namespace std;

class A {
private:
	int a;
public:
	A(int _a = 250) :a(_a)
	{
		cout << "constructor A succese!" << endl;
	}
	~A()
	{
		cout << "destructor A succese a = " << a << endl;
	}
	void print_a()
	{
		cout << "a = " << a << endl;
	}
};

int main(void)
{
	{	//指向类A的智能指针，且new了一个A的默认对象,
		//且当A释放时，智能指针会自动调用其析构函数来释放内存
		unique_ptr<A> p(new A(360));
		p->print_a();
		(*p).print_a();//两个操作相同，证明只能指针的两个符号与普通指针一样
	}
	cout << "___________________________________________" << endl;

	system("pause");
	return 0;
}