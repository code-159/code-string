#include <iostream>
#include <set>
#include <random>
using namespace std;

int main(void)
{
	set<int> s;//不会存储相同的元素
	//multise<int> s1;   会存储相同的元素
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