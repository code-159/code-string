#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <mycoin.h>

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    //explicit PlayScene(QWidget *parent = 0);

    PlayScene(int levelNum);

    int levelIndex;//内部成员函数，记录所选关卡

    //重写绘图事件
    void paintEvent(QPaintEvent *);


    int gameArray[4][4];//二维数组 维护没关的具体数据

    MyCoin* coinBtn[4][4];

    //是否胜利的标志
    bool isWin;

signals:
    //自定义一个信号
    void playSceneSignal();


public slots:
};

#endif // PLAYSCENE_H
