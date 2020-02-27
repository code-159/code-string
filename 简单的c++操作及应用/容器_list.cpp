#include <iostream>
#include <list>

using namespace std;

int main(void)
{
	list<int> l;

	for (int i = 1; i <= 10; i++)
	{
		l.push_back(i);
	}

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