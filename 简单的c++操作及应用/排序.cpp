#include <iostream>

using namespace std;

//排序函数(从小到大)
void paixu(int *p1, int *p2, int *p3)
{
	int temp;
	if (*p1>*p2)
	{ 
		temp = *p1; *p1 = *p2; *p2 = temp;
	}
	if (*p1>*p3) 
	{ 
		temp = *p1; *p1 = *p3; *p3 = temp; 
	}
	if (*p2>*p3) 
	{ 
		temp = *p2; *p2 = *p3; *p3 = temp;
	}
}
int main()
{
	int a, b, c;
	cout << "输入三个数：";
	cin >> a >> b >> c;

	//引用传参
	paixu(&a, &b, &c);

	cout << "输出为：";
	cout << a << " " << b << " " << c << endl;

	system("pause");
	return 0;
}