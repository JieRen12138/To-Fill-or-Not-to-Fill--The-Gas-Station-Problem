#include "widget.h"
#include "ui_widget.h"


size_t l_flag = -1;
size_t flag = 0;
int stop = 0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pushButton->setGeometry(1000, 10, 200, 30);
    ui->pushButton_2->setGeometry(1000, 50, 200, 30);
    ui->pushButton_3->setGeometry(1000, 90, 200, 30);
    ui->lineEdit->setGeometry(1000, 130, 200, 30);
    ui->lineEdit->setDisabled(true);
    ui->textBrowser->setGeometry(1000, 170, 200, 600);
    nodeArr = new vector<NODE>;
    distanceArr = new vector<DISTANCE>;
    resArr = new vector<RESPATH>;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    readFile();
    getDistance();
    timer->start(1000);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(5);
    pen.setColor(QColor(14,9,234));
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
    size_t j = 0;

    for(size_t i=0;i<=nodeArr->size()-1;i++)
    {
        if (i == 0 || i == nodeArr->size()-1)
        {
            painter.setBrush (QBrush(Qt::green, Qt::SolidPattern));
        }
        else
        {
            painter.setBrush (QBrush(Qt::red, Qt::SolidPattern));
        }
        painter.drawEllipse(QPoint(nodeArr->at(i).x,nodeArr->at(i).y),10,10);
    }

    if (resArr->size() != 0 && stop == 1)
        if (l_flag < resArr->size()-1)
        {
            for (j = 0; j <= l_flag; j++)
            {
                painter.drawLine(QPoint(resArr->at(j).res.x,resArr->at(j).res.y), QPoint(resArr->at(j+1).res.x,resArr->at(j+1).res.y));
            }
        }
}

void Widget::readFile()
{
    QFile file("D:\\4905V1.4\\test.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);
            str.chop(1);
            QStringList list = str.split(",");
            NODE temp;
            temp.id = list[0];
            temp.price = list[1].toDouble();
            temp.x = list[2].toDouble();
            temp.y = list[3].toDouble();
            nodeArr->push_back(temp);
        }
        file.close();
    }
    else
    {
        qDebug() << "file does not exist!";
    }
}

void Widget::getDistance()
{
    double secX = 0.0;
    double secY = 0.0;
    double dis = 0.0;

    for(size_t i=0;i<nodeArr->size()-1;i++)
    {
        secX = qAbs(nodeArr->at(i+1).x - nodeArr->at(i).x);
        secY = qAbs(nodeArr->at(i+1).y - nodeArr->at(i).y);
        dis = sqrt(qPow(secX, 2) +  qPow(secY, 2));
        DISTANCE disTemp;
        disTemp.res.x = nodeArr->at(i+1).x;
        disTemp.res.y = nodeArr->at(i+1).y;
        disTemp.dis = dis;
        disTemp.res.id = nodeArr->at(i+1).id;
        disTemp.res.price = nodeArr->at(i+1).price;
        distanceArr->push_back(disTemp);
    }
}

void Widget::getResult1()
{
    resArr->clear();
    QString str;
    RESPATH mypath;
    mypath.res.x = nodeArr->at(0).x;
    mypath.res.y = nodeArr->at(0).y;
    mypath.capa = 0;
    mypath.res.id = nodeArr->at(0).id;
    resArr->push_back(mypath);
    double sumPrice = 0;
    double n = MAX_DISTANCE;
    int k = nodeArr->size()-2;
    double s = 0.0;

    for (int i = 0; i <= k; i++)
    {
        s += distanceArr->at(i).dis;

        if (s >= n)
        {
            mypath.price = s / (MAX_DISTANCE / MAX_CAPACITY) * distanceArr->at(i-1).res.price;
            s = distanceArr->at(i).dis;
            mypath.res.x = distanceArr->at(i-1).res.x;
            mypath.res.y = distanceArr->at(i-1).res.y;
            mypath.res.id = distanceArr->at(i-1).res.id;
            resArr->push_back(mypath);
        }
    }

    mypath.res.x = nodeArr->at(nodeArr->size()-1).x;
    mypath.res.y = nodeArr->at(nodeArr->size()-1).y;
    mypath.capa = 0;
    mypath.res.id = nodeArr->at(nodeArr->size()-1).id;
    resArr->push_back(mypath);

    ui->textBrowser->clear();
    for (size_t j = 0; j <= resArr->size() - 1; j++)
    {
        str = resArr->at(j).res.id + " (" + QString::number(resArr->at(j).res.x, 'f', 2)  + "," + QString::number(resArr->at(j).res.y, 'f', 2) + ")" + " " + QString::number(resArr->at(j).capa, 'f', 2);
        ui->textBrowser->append(str);
        sumPrice += resArr->at(j).price;
    }

    ui->textBrowser->append(QString::number(sumPrice, 'f', 2));
    ui->textBrowser->append(QString::number(resArr->size(), 10));
}

void Widget::getResult()
{
    double avgPrice = MAX_DISTANCE / MAX_CAPACITY;
    double sum = 0;
    double gasRes = 0;
    double gasUsed = 0;
    double s1 = 0;
    double s = 0;
    resArr->clear();
    QString str;
    bool gFlag = false;
    RESPATH mypath;
    mypath.res.x = nodeArr->at(0).x;
    mypath.res.y = nodeArr->at(0).y;
    mypath.capa = 0;
    mypath.res.id = nodeArr->at(0).id;
    resArr->push_back(mypath);
    double sumPrice = 0;
    double n = MAX_CAPACITY;
    double n1 = MAX_DISTANCE;
    int k = nodeArr->size()-2;

    double gasAdd = 0;
    double gasAddPrice = 0;

    for (int i = 0; i <= k; i++)
    {
        s1 += distanceArr->at(i).dis;
        s = distanceArr->at(i).dis;
        gasUsed = s / avgPrice;
        gasRes = n - gasUsed;
        sum = gasRes * avgPrice;
        if (i+1 <= k)
        {
            if (s1 >= n1 && gFlag == false)
            {
                qDebug() << i;
                gFlag = true;
                mypath.res.x = distanceArr->at(i).res.x;
                mypath.res.y = distanceArr->at(i).res.y;
                gasAdd = (s1 - n1) / avgPrice;
                mypath.capa = gasAdd;
                mypath.price = gasAdd * distanceArr->at(i).res.price;
                mypath.res.id = distanceArr->at(i).res.id;
                resArr->push_back(mypath);
            }
            else
            {
                if (sum < distanceArr->at(i+1).dis)
                {
                    if (distanceArr->at(i+1).res.price > distanceArr->at(i).res.price)
                    {
                        gasAdd = gasUsed;
                    }
                    else
                    {
                        gasAdd = distanceArr->at(i+1).dis / avgPrice - gasRes;
                    }

                    gasAddPrice = gasAdd * avgPrice;
                    mypath.res.x = distanceArr->at(i).res.x;
                    mypath.res.y = distanceArr->at(i).res.y;
                    mypath.capa = gasAdd;
                    mypath.price = gasAdd * distanceArr->at(i).res.price;
                    mypath.res.id = distanceArr->at(i).res.id;
                    resArr->push_back(mypath);
                }
            }
            n = gasRes + gasAdd;
        }
    }

    mypath.res.x = nodeArr->at(nodeArr->size()-1).x;
    mypath.res.y = nodeArr->at(nodeArr->size()-1).y;
    mypath.capa = 0;
    mypath.price = 0;
    mypath.res.id = nodeArr->at(nodeArr->size()-1).id;
    resArr->push_back(mypath);

    ui->textBrowser->clear();
    for (size_t j = 0; j <= resArr->size() - 1; j++)
    {
        str = resArr->at(j).res.id + " (" + QString::number(resArr->at(j).res.x, 'f', 2)  + "," + QString::number(resArr->at(j).res.y, 'f', 2) + ")" + " " \
                + QString::number(resArr->at(j).capa, 'f', 2) + "," + QString::number(resArr->at(j).price, 'f', 2);
        ui->textBrowser->append(str);
        sumPrice += resArr->at(j).price;
        qDebug() << "(" << str << ")";
    }

    ui->textBrowser->append(QString::number(sumPrice, 'f', 2));
    ui->textBrowser->append(QString::number(resArr->size(), 10));
}

void Widget::getResult2()
{
    resArr->clear();
    ui->textBrowser->clear();
    //1. get the low price and least node fill in array
    double dis = 0.0;
    double dis1 = 0.0;
    double secX = 0.0;
    double secY = 0.0;
    double needGas = 0.0;
    double gasAdd = 0.0;
    double gasPrice = 0.0;
    double sumPrice = 0.0;
    size_t j = 0;
    size_t i = 0;
    double avgGas = MAX_DISTANCE / MAX_CAPACITY;
    vector <int> index;
    index.push_back(0);
    RESPATH mypath;
    mypath.res.x = nodeArr->at(0).x;
    mypath.res.y = nodeArr->at(0).y;
    mypath.capa = 0;
    mypath.price = 0;
    mypath.res.id = nodeArr->at(0).id;
    resArr->push_back(mypath);

    i = 0;
    while(1)
    {
        for(j=i+1;j<=nodeArr->size()-1;j++)
        {
            secX = qAbs(nodeArr->at(j).x - nodeArr->at(i).x);
            secY = qAbs(nodeArr->at(j).y - nodeArr->at(i).y);
            dis = sqrt(qPow(secX, 2) +  qPow(secY, 2));
            if (dis > MAX_DISTANCE)
            {
                index.push_back(j-1);
                i = j - 1;
                break;
            }
        }
        if (i != j - 1)
            i++;
        secX = qAbs(nodeArr->at(i).x - nodeArr->at(nodeArr->size()-1).x);
        secY = qAbs(nodeArr->at(i).y - nodeArr->at(nodeArr->size()-1).y);
        dis = sqrt(qPow(secX, 2) +  qPow(secY, 2));
        qDebug() << "i = " << i << "dis = " << dis;
        if (dis <= MAX_DISTANCE)
        {
            break;
        }
    }

    //2. go and consider add the counter of gas
    qDebug() << index.size() << "aa";
    for(i=0;i<index.size()-1;i++)
    {
        //qDebug() << index.at(i);
        secX = qAbs(nodeArr->at(index.at(i+1)).x - nodeArr->at(index.at(i)).x);
        secY = qAbs(nodeArr->at(index.at(i+1)).y - nodeArr->at(index.at(i)).y);
        dis = sqrt(qPow(secX, 2) +  qPow(secY, 2));
        //MAX_CAPACITY - dis / avgPrice

        if (i+2 <= index.size()-1)
        {
            secX = qAbs(nodeArr->at(index.at(i+2)).x - nodeArr->at(index.at(i+1)).x);
            secY = qAbs(nodeArr->at(index.at(i+2)).y - nodeArr->at(index.at(i+1)).y);
            dis1 = sqrt(qPow(secX, 2) +  qPow(secY, 2));
            needGas = dis1 / avgGas;

            if (nodeArr->at(index.at(i+2)).price > nodeArr->at(index.at(i+1)).price)
            {
                gasAdd = needGas - (MAX_CAPACITY - dis / avgGas);
            }
            else
            {
                gasAdd = dis / avgGas;
            }

            gasPrice = gasAdd * nodeArr->at(index.at(i+1)).price;

            mypath.res.x = nodeArr->at(index.at(i+1)).x;
            mypath.res.y = nodeArr->at(index.at(i+1)).y;
            mypath.capa = gasAdd;
            mypath.price = gasPrice;
            mypath.res.id = nodeArr->at(index.at(i+1)).id;
            resArr->push_back(mypath);
        }
    }

    mypath.res.x = nodeArr->at(nodeArr->size()-1).x;
    mypath.res.y = nodeArr->at(nodeArr->size()-1).y;
    mypath.capa = 0;
    mypath.price = 0;
    mypath.res.id = nodeArr->at(nodeArr->size()-1).id;
    resArr->push_back(mypath);

    for (size_t j = 0; j <= resArr->size() - 1; j++)
    {
        QString str = resArr->at(j).res.id + " (" + QString::number(resArr->at(j).res.x, 'f', 2)  + "," + QString::number(resArr->at(j).res.y, 'f', 2) + ")" + " " \
                + QString::number(resArr->at(j).capa, 'f', 2) + "," + QString::number(resArr->at(j).price, 'f', 2);
        ui->textBrowser->append(str);
        sumPrice += resArr->at(j).price;
    }

    ui->textBrowser->append(QString::number(sumPrice, 'f', 2));
    ui->textBrowser->append(QString::number(resArr->size(), 10));

}

void Widget::on_pushButton_clicked()
{
    QTime time;

    time.start();
    getResult();
    QString str = QString("%1").arg(time.elapsed()/1000.0) + "s";
    ui->lineEdit->setText(str);
    l_flag = -1;
    stop = 1;
}

void Widget::on_pushButton_2_clicked()
{
    QTime time;

    time.start();
    getResult1();
    QString str = QString("%1").arg(time.elapsed()/1000.0) + "s";
    ui->lineEdit->setText(str);
    l_flag = -1;
    stop = 1;
}

void Widget::timerUpDate()
{
    if (stop == 1)
    {
        l_flag++;
        if (l_flag < resArr->size()-1)
        {
            update();
        }
    }
}

void Widget::on_pushButton_3_clicked()
{
    QTime time;

    time.start();
    getResult2();
    QString str = QString("%1").arg(time.elapsed()/1000.0) + "s";
    ui->lineEdit->setText(str);
    l_flag = -1;
    stop = 1;
}
