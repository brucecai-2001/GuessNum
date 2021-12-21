#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QString>
#include <QMovie>

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

private:
    Ui::MyWidget *ui;
    int gametime=0;
    int gameTimerID;
    int overTimerID;
    int winTimerID;
    QString randStr;
    QString resultStr;//输入的结果
    QMovie winMovie;
    QMovie loseMovie;
private slots:
    void on_pushButtonStart_clicked();
    void on_pushButtonEnd_clicked();
    void on_pushButtonDel_clicked();
    void timerEvent(QTimerEvent *e);
    void dealNum();
};

#endif // MYWIDGET_H
