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
	{	//ָ����A������ָ�룬��new��һ��A��Ĭ�϶���,
		//�ҵ�A�ͷ�ʱ������ָ����Զ������������������ͷ��ڴ�
		unique_ptr<A> p(new A(360));
		p->print_a();
		(*p).print_a();//����������ͬ��֤��ֻ��ָ���������������ָͨ��һ��
	}
	cout << "___________________________________________" << endl;

	system("pause");
	return 0;
}