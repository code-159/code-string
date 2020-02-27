#include <iostream>
#include <map>
#include <string>
using namespace std;

int main(void)
{
	map<string, int> m;//不允许重复的键
	//multimap<string,int> m   允许重复的键

	m["卢本伟"] = 666;
	m["马飞飞"] = 233;
	m["大官人"] = 888;
	m["YJJ"] = 123;

	map<string, int>::iterator itr;
	for (itr = m.begin(); itr != m.end(); itr++)
	{
		//itr可看成指向键值对的指针,*itr为键值对的值
		//itr.first为键,itr.second为值,所以打印可如下打印
		cout << (*itr).first << " : " << (*itr).second << endl;
	}

	system("pause");
	return 0;
}