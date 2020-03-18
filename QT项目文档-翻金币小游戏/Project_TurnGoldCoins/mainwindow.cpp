#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mypushbutton.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QSound>//多媒体音效头文件

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置主场景

    //设置固定的大小
    setFixedSize(320,588);

    //设置标题
    setWindowTitle("Turn Gold Coins");

    //设置图标
    setWindowIcon(QIcon(":/res/coin_PixMap.png"));
    this->setStyleSheet("QIcon{border:0px;}");

    //退出按钮实现
    connect(ui->actionExit,&QAction::triggered,[=](){
        this->close();
    });

    //准备开始的音效
    QSound* startSound = new QSound(":/res/start.wav",this);

    //开始的按钮
    myPushButton* startBtn = new myPushButton(":/res/start1.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5-startBtn->width()*0.5,this->height()*0.4);

    //实例化关卡对象
    chooseScene = new ChooseLevelScene;

    //监听返回上一层
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,[=](){
        //设置this场景为chooseScene的位置
        this->setGeometry(chooseScene->geometry());

        chooseScene->hide();//隐藏
        this->show();//主场景show
    });

    connect(startBtn,&myPushButton::clicked,[=](){
        qDebug()<<"点击了开始键";
        //播放音效
        startSound->play();
        //特效
        startBtn->zoom1();
        startBtn->zoom2();
        //先进入延时，显示效果
        QTimer::singleShot(400,this,[=](){
            //设置chooseScene场景位置
            chooseScene->setGeometry(this->geometry());

            //再进入到选择关卡的场景中,自身隐藏，进入选择关卡
            this->hide();
            chooseScene->show();
        });
    });
}

//显示背景图
void MainWindow::paintEvent(QPaintEvent *)
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

MainWindow::~MainWindow()
{
    delete ui;
}
