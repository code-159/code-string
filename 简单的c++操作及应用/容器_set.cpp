#include <iostream>
#include <set>
#include <random>
using namespace std;

int main(void)
{
	set<int> s;//����洢��ͬ��Ԫ��
	//multise<int> s1;   ��洢��ͬ��Ԫ��
	int i;
	
	while(1)
	{
		cin >> i;
		if (i == 0)
			break;
		s.insert(i);
	}

	set<int>::iterator itr;
	for (itr = s.begin(); itr != s.end(); itr++)
	{
		cout << *itr << " ";
	}
	cout << endl;

	system("pause");
	return 0;
}