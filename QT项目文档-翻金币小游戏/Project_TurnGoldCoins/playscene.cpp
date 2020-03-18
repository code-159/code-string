#include "playscene.h"
#include <QDebug>
#include <QMenuBar>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QString>
#include "mycoin.h"
#include "mypushbutton.h"
#include "dataconfig.h"
#include <QPropertyAnimation>
#include <QSound>

//PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
//{

//}
PlayScene::PlayScene(int levelNum)
{
    QString str = QString("进入了第 %1 关").arg(levelNum);
    qDebug()<<str;
    this->levelIndex = levelNum;

    //设置固定的大小
    setFixedSize(320,588);

    //设置标题
    setWindowTitle("Turn Gold Coins Game");

    //设置图标
    setWindowIcon(QIcon(":/res/coin_PixMap.png"));
    this->setStyleSheet("QIcon{border:0px;}");

    //创建菜单栏
    QMenuBar* bar = menuBar();
    setMenuBar(bar);

    QMenu* startMenu = bar->addMenu("开始");
    QAction* quitAction = startMenu->addAction("退出");

    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //准备开始的音效
    QSound* backSound = new QSound(":/res/bb.wav",this);
    QSound* successSound = new QSound(":/res/win.wav",this);
    QSound* btnSound = new QSound(":/res/btn.wav");

    //back按钮实现
    //返回按钮
    myPushButton* backBtn = new myPushButton(":/res/back.png",":/res/back_1.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width()+15,this->height()-backBtn->height()+15);

    connect(backBtn,&myPushButton::clicked,[=](){
        qDebug()<<"点击了返回键";
        backSound->play();
        QTimer::singleShot(400,this,[=](){
            emit this->playSceneSignal();//发出信号，让主场景知道返回
        });
    });

    //显示当前关卡的字样
    QLabel* label = new QLabel(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str1 = QString("Level: %1").arg(this->levelIndex);

    //将字体设置到标签中
    label->setFont(font);
    label->setText(str1);
    //设置一个矩形框,
    label->setGeometry(10,this->height()-468,120,50);


    dataConfig config;
    //初始化每个关卡的二维数组
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4 ; j++)
        {
            //通过config mData中的数据 存取到二维数组中，后面再进行其他操作
            this->gameArray[i][j] = config.mData[this->levelIndex][i][j];//第几关的二维数组
        }
    }

    //胜利图片的定义声明，可供后面胜利时再投放出来
    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/success.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width() - tmpPix.width())*0.5 , -tmpPix.height());

    //显示金币边框
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            QLabel* label = new QLabel;
            label->setGeometry(0,0,50,50);
            label->setPixmap(QPixmap(":/res/biankuang.jpg"));
            label->setParent(this);
            label->move(57+i*50,200+j*50);

            //创建金币
            QString str;
            if(this->gameArray[i][j] == 1)
            {
                //显示
                str = ":/res/coin001.png";
            }
            else
            {
                str = ":/res/coin008.png";
            }

            MyCoin* coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(57+i*50,200+j*50);

            //给金币属性赋值
            coin->posX = i;
            coin->posY = j;
            coin->flag = this->gameArray[i][j];//1为正面，0为反面

            //将金币放入二维维护数组中
            this->coinBtn[i][j] = coin;

            //监听点击事件
            connect(coin,&MyCoin::clicked,[=](){
                btnSound->play();
                //点击后  禁用所有按钮
                for(int i = 0; i <4; i++)
                {
                    for(int j = 0; j < 4; j++)
                    {
                        this->coinBtn[i][j]->isWin = true;
                    }
                }

                coin->changeFlag();
                this->gameArray[i][j]=this->gameArray[i][j] == 0? 1 : 0;//更新二维数组的内容

                QTimer::singleShot(300,this,[=](){
                    //翻转周围的硬币
                    if(coin->posX+1 <= 3)//右
                    {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY] = this->gameArray[coin->posX+1][coin->posY] == 0? 1 : 0;
                    }

                    if(coin->posX-1 >= 0)//左
                    {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY] = this->gameArray[coin->posX-1][coin->posY] == 0? 1 : 0;
                    }

                    if(coin->posY+1 <= 3)//上
                    {
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY+1] = this->gameArray[coin->posX][coin->posY+1] == 0? 1 : 0;
                    }

                    if(coin->posY-1 >= 0)//下
                    {
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY-1] = this->gameArray[coin->posX][coin->posY-1] == 0? 1 : 0;
                    }

                    //翻完周围解开禁用
                    for(int i = 0; i <4; i++)
                    {
                        for(int j = 0; j < 4; j++)
                        {
                            this->coinBtn[i][j]->isWin = false;
                        }
                    }

                    //判断是否胜利
                    this->isWin = true;
                    for(int i = 0; i <4; i ++)
                    {
                        for(int j = 0; j<4; j++)
                        {
                            if(coinBtn[i][j]->flag == false)//是否有一个反面，如果有就失败，break
                            {
                                this->isWin = false;
                                break;
                            }
                        }
                    }

                    if(this->isWin == true)
                    {
                        //胜利了
                        qDebug()<<"游戏胜利!";
                        successSound->play();
                        //将全部按钮变为true,然后不再响应按钮事件
                        for(int i = 0; i <4; i ++)
                        {
                            for(int j = 0; j<4; j++)
                            {
                                coinBtn[i][j]->isWin = true;
                            }
                        }

                        //将胜利图片给放下来
                        QPropertyAnimation* animation = new QPropertyAnimation(winLabel,"geometry");
                        //设置时间间隔
                        animation->setDuration(1000);
                        //设置开始位置
                        animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        //设置结束位置
                        animation->setEndValue(QRect(winLabel->x(),winLabel->y()+520,winLabel->width(),winLabel->height()));
                        //设置缓和曲线
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        //执行动画
                        animation->start();
                    }
                });
            });
        }
    }
}

//重写绘图事件
void PlayScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/start_background.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);//设置为主界面的高度和宽度

    //画上背景的图标
    pix.load(":/res/pixMap_beijin.png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    this->setStyleSheet("QPixmap{border:0px;}");
    painter.drawPixmap(40,20,pix.width(),pix.height(),pix);
}
