#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <random>
#include <condition_variable>
//�߳�+������+��������
using namespace std;

class buffer {
private:
	list<int> l;
	mutex m;//m��������������Դ l
	condition_variable cv;//l �����ݵ��¼�
public:
	buffer()
	{}
	//������
	void add_data(int d)
	{
		unique_lock<mutex> lm(m);

		l.push_back(d);//��������

		cv.notify_one();//�������ݣ�������������
	}
	//ɾ����
	int delete_data()
	{
		unique_lock<mutex> lm(m);

		int d = l.front();
		l.pop_front();

		return d;
	}
	bool empty()
	{
		return l.empty();
	}

	void wait_data()
	{
		unique_lock<mutex> lm(m);
		//wait ��������������ȴ��¼�����
		cv.wait(lm);
	}
};

void producer(buffer& b)
{
	while (1)
	{
		//   ����data
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		b.add_data(888);
	}
}

void comsumer(buffer& b)
{
	while (1)
	{
		while (1)
		{
			b.wait_data();
		}
		int d = b.delete_data();
		
		cout << "d = " << d << endl;
	}
}

int main(void)
{
	buffer buf;

	thread t1(producer, std::ref(buf));
	thread t2(comsumer, std::ref(buf));
	t1.join();
	t2.join();

	system("pause");
	return 0;
}