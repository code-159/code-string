#include <iostream>
#include <map>
#include <string>
using namespace std;

int main(void)
{
	map<string, int> m;//�������ظ��ļ�
	//multimap<string,int> m   �����ظ��ļ�

	m["¬��ΰ"] = 666;
	m["��ɷ�"] = 233;
	m["�����"] = 888;
	m["YJJ"] = 123;

	map<string, int>::iterator itr;
	for (itr = m.begin(); itr != m.end(); itr++)
	{
		//itr�ɿ���ָ���ֵ�Ե�ָ��,*itrΪ��ֵ�Ե�ֵ
		//itr.firstΪ��,itr.secondΪֵ,���Դ�ӡ�����´�ӡ
		cout << (*itr).first << " : " << (*itr).second << endl;
	}

	system("pause");
	return 0;
}