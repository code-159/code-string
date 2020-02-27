#include <iostream>
#include <vector>

using namespace std;
//用户输入到vector容器里并打印出来
int main(void)
{
	vector<int> v1;
	int input_num;

	for (int i = 1; i <= 10; i++)
	{
		cin >> input_num;
		if (input_num == 0)
			break;
		v1.push_back(input_num);
	}

	//v1.reserve(v1.size());

	vector<int>::iterator itr;
	for (itr = v1.begin(); itr != v1.end(); itr++)
	{
		cout << *itr << " ";
	}
	cout << endl;

	system("pause");
	return 0;
}

//从1~10的插入以及打印
/*int main(void)
{
	vector<int> v1;

	for (int i = 1; i <= 10; i++)
	{
		v1.push_back(i);
	}

	vector<int>::iterator itr;
	for (itr = v1.begin(); itr != v1.end(); itr++)
	{
		cout << *itr << " ";
	}
	cout << endl;

	system("pause");
	return 0;
}*/
