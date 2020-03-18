#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QTimer>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyCoin(QWidget *parent = 0);

    //参数代表传入金币还是银币
    MyCoin(QString btnImg);

    //金币的属性
    int posX;//金币X坐标位置
    int posY;//金币Y坐标位置
    bool flag;//金币正反的状态

    //改变标志的方法
    void changeFlag();
    QTimer* timer1;//正面翻反面的定时器
    QTimer* timer2;//反面翻正面的定时器
    int min = 1;
    int max = 8;

    //执行动画的标志
    bool isAnimation = false;

    //重写鼠标按下函数
    void mousePressEvent(QMouseEvent *);

    //是否胜利的标志
    bool isWin = false;

signals:

public slots:
};

#endif // MYCOIN_H
