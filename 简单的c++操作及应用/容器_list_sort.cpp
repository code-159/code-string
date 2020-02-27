#include <iostream>
#include <list>

using namespace std;

int main(void)
{
	list<int> l;
	int inp_num;

	while (1)
	{
		cin >> inp_num;
		if (inp_num == 0)
			break;
		//再排序输出
		list<int>::iterator itr;
		for (itr = l.begin(); itr != l.end(); itr++)
		{
			//升序插入
			if (*itr > inp_num)
				break;
			/*//逆序插入
			if(*itr <inp_num)
				break;
			*/
		}
		l.insert(itr, inp_num);
	}
	//输出
	list<int>::iterator itr;
	for (itr = l.begin(); itr != l.end(); itr++)
	{
		//可以把它理解为指针
		cout << *itr << " ";
	}
	cout << endl;

	system("pause");
	return 0;
}