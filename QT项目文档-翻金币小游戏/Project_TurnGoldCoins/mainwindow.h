#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <chooselevelscene.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //重写pointEvent事件  画背景图
    void paintEvent(QPaintEvent *);

    ChooseLevelScene* chooseScene = NULL;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
