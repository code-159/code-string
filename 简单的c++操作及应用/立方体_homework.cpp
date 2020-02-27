#include <iostream>
//设计一个立方体类，要求属性和方法：成员属性  长、宽、高,成员函数
using namespace std;

class Cube {
private:
	double length, width, high;
public:
	Cube(double _length,double _width,double _high):length(_length),width(_width),high(_high)//构造函数
	{}
	////设置长、获取长
	double set_get_length(double _length)
	{
		length = _length;
		return length;
	}
	//设置宽、获取宽
	double set_get_width(double _width)
	{
		width = _width;
		return width;
	}
	//设置高、获取高
	double set_get_high(double _high)
	{
		high = _high;
		return high;
	}
	//求立方体面积
	void area()
	{
		double _area = 2 * ( length * width  +  length * high + width * high );
		cout << "该立方体面积为：" << _area << endl;
	}
	//求立方体体积
	void volume()
	{
		double _volume = (length * width * high);
		cout << "该立方里体积为：" << _volume << endl;
	}
	//通过成员函数判断两个立方体是否相同，相对长宽高相对应相等就可证明相同
	void Judge_same(Cube &_c)
	{
		if ((length == _c.length && width == _c.width && high == _c.high) ||
			(length == _c.length && width == _c.high && high == _c.width) ||
			(length == _c.width && width == _c.high && high == _c.length) ||
			(length == _c.width && width == _c.length && high == _c.high) ||
			(length == _c.high && width == _c.length && high == _c.width) ||
			(length == _c.high && width == _c.width && high == _c.length))
		{
			cout << " 这两个立方体完全相同!!!" << endl;
		}
		else
			cout << "这两个立方体不同!!!" << endl;
	}
};

int main(void)
{
	Cube c1(10, 10, 10);
	Cube c2(10, 20, 10);
	c1.area();
	c1.volume();
	c1.set_get_high(20);
	c1.area();
	c1.volume();

	//c2.area();
	//c2.volume();
	//c2.set_get_high(30);
	//c2.area();
	//c2.volume();

	c1.Judge_same(c2);
	
	system("pause");
	return 0;
}