#include <iostream>

using namespace std;
class B;//�ɼӿɲ��ӣ�ǰ������

class A {
private:
	int a;
public:
	A(int _a = 666):a(_a)
	{}
	friend class B;
};

class B {
private:
	A a1;
public:
	void print_a_v1()
	{
		cout << " a1.a_v1 = " << a1.a << endl;
	}
	void print_a_v2(A& a)
	{
		cout << " a1.a_v2 = " << a.a << endl;
	}
};
//��A��������ΪB�����Ԫ��ʱ����A�е����ݿ�������B���з���
int main(void)
{
	A a1(777777);
	B b1;
	b1.print_a_v1();
	b1.print_a_v2(a1);

	system("pause");
	return 0;
}