#include <iostream>
#include <string.h>

using namespace std;

//字符串逆序函数
char* inverse(char *str)
{
	int len = strlen(str);
	int i, n = len / 2;
	char tem;
	for (i = 0; i <n; i++) //i<=n会多交换一次中间的变成原样
	{
		tem = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = tem;
	}
	return str;

}

int main(void)
{
	char a[100];
	cout << "输入字符串：";
	cin >> a;
	char *ptr = inverse(a);

	char* &q = ptr;

	cout << "输出字符串：";
	cout << q << endl;

	system("pause");
	return 0;
}