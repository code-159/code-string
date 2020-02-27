#include <iostream>

using namespace std;

int main(void)
{
	int i, j;
	for (i = 101; i <= 200; i++)
	{
		for (j = 2; j < i; j++)
		{
			if (i % j == 0)    //是素数
			{
				break;
			}
			else
			{
				if (j == (i - 1))       //除完最后一个数后还不跳出循环，就可以判断该数为素数。
				{
					cout << i << "为素数" << endl;
				}
			}
		}
	}
	system("pause");
	return 0;
}