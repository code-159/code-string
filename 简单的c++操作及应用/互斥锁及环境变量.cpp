#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <random>
#include <condition_variable>
//线程+互斥锁+条件变量
using namespace std;

class buffer {
private:
	list<int> l;
	mutex m;//m用来保护共享资源 l
	condition_variable cv;//l 有数据的事件
public:
	buffer()
	{}
	//加数据
	void add_data(int d)
	{
		unique_lock<mutex> lm(m);

		l.push_back(d);//推入数据

		cv.notify_one();//产生数据，唤醒条件变量
	}
	//删数据
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
		//wait 这个条件变量，等待事件产生
		cv.wait(lm);
	}
};

void producer(buffer& b)
{
	while (1)
	{
		//   产生data
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