#include <iostream>
#include <string.h>

using namespace std;

//�ַ���������
char* inverse(char *str)
{
	int len = strlen(str);
	int i, n = len / 2;
	char tem;
	for (i = 0; i <n; i++) //i<=n��ཻ��һ���м�ı��ԭ��
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
	cout << "�����ַ�����";
	cin >> a;
	char *ptr = inverse(a);

	char* &q = ptr;

	cout << "����ַ�����";
	cout << q << endl;

	system("pause");
	return 0;
}