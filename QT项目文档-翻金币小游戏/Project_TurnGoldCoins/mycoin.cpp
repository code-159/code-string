#include "mycoin.h"

//MyCoin::MyCoin(QWidget *parent) : QWidget(parent)
//{

//}


MyCoin::MyCoin(QString btnImg)
{
    QPixmap pix;
    pix.load(btnImg);

    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));

    //初始化定时器
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    //监听正面翻反面的信号，然后翻转金币
    connect(timer1,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString(":/res/coin00%1").arg(this->min++);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));

        //判断 翻完了 min至1，timer1 stop
        if(this->min > this->max)
        {
            this->min = 1;
            this->isAnimation = false;
            timer1->stop();
        }
    });

    //反面->正面
    connect(timer2,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString(":/res/coin00%1").arg(this->max--);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));

        //判断 翻完了 max至8，timer2 stop
        if(this->max < this->min)
        {
            this->max = 8;
            this->isAnimation = false;
            timer2->stop();
        }
    });
}

//重写鼠标按下函数
void MyCoin::mousePressEvent(QMouseEvent *e)
{
    if(this->isAnimation || this->isWin)
        return;
    else
        QPushButton::mousePressEvent(e);
}

//改变正反面的方法
void MyCoin::changeFlag()
{
    //如果是正面 ->反面
    if(this->flag)
    {
        timer1->start(30);
        this->isAnimation = true;//开始做动画
        this->flag = false;
    }
    else
    {
        timer2->start(30);
        this->isAnimation = true;
        this->flag = true;
    }
}
