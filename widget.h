#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <vector>
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QtMath>
#include <QTimer>
#include <QTime>

using namespace std;

#define    MAX_DISTANCE    250
#define    MAX_CAPACITY    38

typedef struct node
{
    QString id;
    double price;
    double x;
    double y;
} NODE;

typedef struct distance
{
    NODE res;
    double dis;
} DISTANCE;

typedef struct resPath
{
    NODE res;
    double capa;
    double price;
} RESPATH;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void paintEvent (QPaintEvent *);
    void readFile();
    void getDistance();
    void getResult();
    void getResult1();
    void getResult2();

private slots:
    void timerUpDate();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;
    vector<NODE> *nodeArr;
    vector<DISTANCE> *distanceArr;
    vector<RESPATH> *resArr;
    QTimer *timer;
};

#endif // WIDGET_H
