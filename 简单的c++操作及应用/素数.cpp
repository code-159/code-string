#include <iostream>

using namespace std;

int main(void)
{
	int i, j;
	for (i = 101; i <= 200; i++)
	{
		for (j = 2; j < i; j++)
		{
			if (i % j == 0)    //������
			{
				break;
			}
			else
			{
				if (j == (i - 1))       //�������һ�����󻹲�����ѭ�����Ϳ����жϸ���Ϊ������
				{
					cout << i << "Ϊ����" << endl;
				}
			}
		}
	}
	system("pause");
	return 0;
}