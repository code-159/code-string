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
		//���������
		list<int>::iterator itr;
		for (itr = l.begin(); itr != l.end(); itr++)
		{
			//�������
			if (*itr > inp_num)
				break;
			/*//�������
			if(*itr <inp_num)
				break;
			*/
		}
		l.insert(itr, inp_num);
	}
	//���
	list<int>::iterator itr;
	for (itr = l.begin(); itr != l.end(); itr++)
	{
		//���԰������Ϊָ��
		cout << *itr << " ";
	}
	cout << endl;

	system("pause");
	return 0;
}