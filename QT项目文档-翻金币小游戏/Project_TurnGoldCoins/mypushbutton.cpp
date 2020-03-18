#include "mypushbutton.h"
#include <QPropertyAnimation>   //特效的头文件

//myPushButton::myPushButton(QWidget *parent) : QWidget(parent)
//{

//}

myPushButton::myPushButton(QString normalImg, QString pressImg)
{
    this->normalImagePath = normalImg;
    this->pressImagePath = pressImg;

    QPixmap pix;
    pix.load(normalImg);

    //设置图片固定大小
    this->setFixedSize(pix.width(),pix.height());

    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");

    //设置图标
    this->setIcon(pix);

    //设置图标大小
    this->setIconSize(QSize(pix.width()*0.75,pix.height()*0.75));

}

void myPushButton::zoom1()//下
{
    //创建动画的对象
    QPropertyAnimation* animation = new QPropertyAnimation(this,"geometry");
    //设置动画时间间隔
    animation->setDuration(200);

    //起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));

    //设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //开始执行动画
    animation->start();
}

void myPushButton::zoom2()//上
{
    //创建动画的对象
    QPropertyAnimation* animation = new QPropertyAnimation(this,"geometry");
    //设置动画时间间隔
    animation->setDuration(200);

    //起始位置
    animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));

    //设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //开始执行动画
    animation->start();
}

void myPushButton::mousePressEvent(QMouseEvent *e)
{
    if(this->pressImagePath != "")//传入按下图片不为空，所以要切换图片
    {
        QPixmap pix;
        pix.load(this->pressImagePath);

        //设置图片固定大小
        this->setFixedSize(pix.width(),pix.height());

        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px;}");

        //设置图标
        this->setIcon(pix);

        //设置图标大小
        this->setIconSize(QSize(pix.width()*0.75,pix.height()*0.75));
    }
    //让父类执行其他操作
    return QPushButton::mousePressEvent(e);
}

void myPushButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(this->pressImagePath != "")//传入按下图片不为空，所以要切换图片
    {
        QPixmap pix;
        pix.load(this->normalImagePath);

        //设置图片固定大小
        this->setFixedSize(pix.width(),pix.height());

        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px;}");

        //设置图标
        this->setIcon(pix);

        //设置图标大小
        this->setIconSize(QSize(pix.width()*0.75,pix.height()*0.75));
    }
    //让父类执行其他操作
    return QPushButton::mouseReleaseEvent(e);
}
