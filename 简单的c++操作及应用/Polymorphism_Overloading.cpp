#include <iostream>

using namespace std;

int func(int i)
{
	return i;
}

float func(float i,float j)
{
	return i+j;
}

int main(void)
{
	float i = func(5.5,5.9);
	cout << "i = " << i << endl;

	system("pause");
	return 0;
}