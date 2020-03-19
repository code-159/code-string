#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    int a[15][15];
    int isWin(int, int);
    int f1(int, int);
    int f2(int, int);
    int f3(int, int);
    int f4(int, int);
    int player;
};


#endif // MAINWINDOW_H
