#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QSound>
#include "mypushbutton.h"

ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    //配置选择关卡的场景
    this->setFixedSize(320,588);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/coin_PixMap.png"));

    //设置标题
    this->setWindowTitle("Choose Level");

    //创建菜单栏
    QMenuBar* bar = menuBar();
    setMenuBar(bar);

    QMenu* startMenu = bar->addMenu("开始");
    QAction* quitAction = startMenu->addAction("退出");

    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //准备开始的音效
    QSound* chooseSound = new QSound(":/res/choose.wav",this);
    QSound* backSound = new QSound(":/res/back.wav",this);

    //返回按钮
    myPushButton* backBtn = new myPushButton(":/res/back.png",":/res/back_1.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width()+15,this->height()-backBtn->height()+15);

    connect(backBtn,&myPushButton::clicked,[=](){
        backSound->play();
        qDebug()<<"点击了返回键";
        QTimer::singleShot(400,this,[=](){
            emit this->chooseSceneBack();//发出信号，让主场景知道返回
        });
    });

    //创建选择关卡的按钮
    for(int i = 0 ; i < 20 ; i ++)
    {
        myPushButton* menuBtn = new myPushButton(":/res/small_btn.png");
        menuBtn->setParent(this);
        menuBtn->move(31 + i%4 * 70 , 130 + i/4 * 70);

        //监听每个按钮的点击事件
        connect(menuBtn,&myPushButton::clicked,[=](){
            chooseSound->play();
            QString str = QString("选择了第 %1 关").arg(i+1);
            qDebug()<<str;

            //进入到游戏核心场景
            this->hide();
            play = new PlayScene(i+1);//场景游戏的场景
            //设置游戏场景初始的位置
            play->setGeometry(this->geometry());

            play->show();//显示游戏

            connect(play,&PlayScene::playSceneSignal,[=](){
                this->setGeometry(play->geometry());

                this->show();
                delete play;//这个是为了，让关卡退出后，关卡复位，然后显示选择关卡的界面
                play = NULL;
            });
        });

        //创建关卡数字label
        QLabel* label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(i+1));
        label->move(31 + i%4 * 70 , 130 + i/4 * 70);

        //设置label上的文字对齐方式,水平，垂直居中
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        //设置鼠标进行穿透   51号属性
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
}

void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/background_2.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);//设置为主界面的高度和宽度

    //画上背景的图标
    pix.load(":/res/pixMap_beijin.png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    this->setStyleSheet("QPixmap{border:0px;}");
    painter.drawPixmap(40,20,pix.width(),pix.height(),pix);
}
