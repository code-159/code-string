#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class myPushButton : public QPushButton
{
    Q_OBJECT
public:
    //explicit myPushButton(QWidget *parent = 0);

    //参数1 正常显示的图片路径   2  按下后显示的图片路径
    myPushButton(QString normalImg, QString pressImg = "");

    //成员属性 保存用户传入的路径
    QString normalImagePath;
    QString pressImagePath;

    //弹跳效果函数
    void zoom1();//下
    void zoom2();//上

    //重写按钮按下，释放事件
    void mousePressEvent(QMouseEvent *e);

    void mouseReleaseEvent(QMouseEvent *e);

signals:

public slots:
};

#endif // MYPUSHBUTTON_H
