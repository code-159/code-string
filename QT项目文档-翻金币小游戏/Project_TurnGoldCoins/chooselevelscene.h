#ifndef CHOOSELEVELSCENE_H
#define CHOOSELEVELSCENE_H

#include <QMainWindow>
#include <playscene.h>

class ChooseLevelScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChooseLevelScene(QWidget *parent = 0);

    //重写绘图事件
    void paintEvent(QPaintEvent *);

    //游戏场景对象指针
    PlayScene* play = NULL;

signals:
    //自定义一个信号
    void chooseSceneBack();

public slots:
};

#endif // CHOOSELEVELSCENE_H
