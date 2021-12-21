#include "mywidget.h"
#include "ui_mywidget.h"
#include <QTime>
#include <QString>
#include <QMessageBox>
#include <QPushButton>
#include <QFont>
#include <QDebug>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    //设置第一页
    ui->stackedWidget->setCurrentWidget(ui->pageStart);
    QFont font("Microsoft YaHei",20,75);
    ui->label_5->setFont(font);
    //初始化数据
    //胜利动画
    winMovie.setFileName(":/Resource/Rick.gif");
    ui->label_4->setMovie(&winMovie);//设置标签
    ui->label_4->setScaledContents(true);//自适应
    this->setWindowTitle("GUESS");

    //失败动画
    loseMovie.setFileName(":/Resource/zhilang.gif");
    ui->label_3->setMovie(&loseMovie);//设置标签
    ui->label_3->setScaledContents(true);//自适应
    this->setWindowTitle("GUESS");

    //信号连接
    connect(ui->pushButtonStart,&QPushButton::clicked,this,&MyWidget::on_pushButtonStart_clicked);
    connect(ui->pushButtonEnd,&QPushButton::clicked,this,&MyWidget::on_pushButtonEnd_clicked);
    connect(ui->pushButtonDel,&QPushButton::clicked,this,&MyWidget::on_pushButtonDel_clicked);
    connect(ui->pushButton0,&QPushButton::clicked,this,&MyWidget::dealNum);
    connect(ui->pushButton1,&QPushButton::clicked,this,&MyWidget::dealNum);
    connect(ui->pushButton2,&QPushButton::clicked,this,&MyWidget::dealNum);
    connect(ui->pushButton3,&QPushButton::clicked,this,&MyWidget::dealNum);
    connect(ui->pushButton4,&QPushButton::clicked,this,&MyWidget::dealNum);
    connect(ui->pushButton5,&QPushButton::clicked,this,&MyWidget::dealNum);
    connect(ui->pushButton6,&QPushButton::clicked,this,&MyWidget::dealNum);
    connect(ui->pushButton7,&QPushButton::clicked,this,&MyWidget::dealNum);
    connect(ui->pushButton8,&QPushButton::clicked,this,&MyWidget::dealNum);
    connect(ui->pushButton9,&QPushButton::clicked,this,&MyWidget::dealNum);

}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_pushButtonStart_clicked()
{
    //获取游戏时间
    gametime=ui->comboBox->currentText().toInt();
    //切换界面
    ui->stackedWidget->setCurrentWidget(ui->pageGame);

    int num;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));//设置时间种子
    while( (num=qrand()%10000)<999);
    qDebug()<<num;
    randStr=QString::number(num);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(gametime);
    ui->progressBar->setValue(gametime);

    //开始记时
    gameTimerID=startTimer(1000);//一秒一个间隔

    resultStr.clear();
    ui->textEdit->clear();
}

void MyWidget::on_pushButtonEnd_clicked()
{
    this->close();
}

//退格
void MyWidget::on_pushButtonDel_clicked()
{
    if(resultStr.size()==1){
        resultStr.clear();
        ui->textEdit->clear();
    }
    else{
        resultStr.chop(1);//截取最后一个字符
        ui->textEdit->setText(resultStr);
    }
}

void MyWidget::timerEvent(QTimerEvent *e)
{
    if(e->timerId()==gameTimerID){
        gametime--;

        ui->progressBar->setValue(gametime);
        //时间到了，失败
        if(gametime==0){
            killTimer(gameTimerID);
            QMessageBox::information(this,"失败","时间到了啊 kuso");
            ui->stackedWidget->setCurrentWidget(ui->pageOver);
            loseMovie.start();
            overTimerID=startTimer(5000);//5秒后关闭
        }
    }
    else if(e->timerId()==overTimerID){
        loseMovie.stop();
        killTimer(overTimerID);
        ui->stackedWidget->setCurrentWidget(ui->pageStart);
    }
    else if(e->timerId()==winTimerID){
        winMovie.stop();
        killTimer(winTimerID);
        ui->stackedWidget->setCurrentWidget(ui->pageStart);
    }

}

void MyWidget::dealNum()
{
    //获取信号接收者
    QObject *mySender=sender();
    QPushButton *p=(QPushButton *)mySender;
    if(p!=NULL){
        QString numStr=p->text();
        resultStr+=numStr;
        if(resultStr.size()==0 && resultStr=="0") resultStr.clear();//不可以以0开头
        if(resultStr.size()<=4){
            qDebug()<<resultStr;
            ui->textEdit->setText(resultStr);//数字到第四位，判断是不是正确的数字
            if(resultStr.size()==4){
                if(resultStr>randStr) ui->textEdit->append("Bigger !!!!!!!");
                else if(resultStr<randStr) ui->textEdit->append("Smaller !!!!!!");
                else{
                    ui->textEdit->append(" WELL DONE, it is correct");

                    //关闭计时器
                    killTimer(gameTimerID);
                    QMessageBox::information(this,"胜利","赢麻了");
                    ui->stackedWidget->setCurrentWidget(ui->pageWin);
                    winMovie.start();//开启胜利动画
                    winTimerID=startTimer(5000);

                }
                resultStr.clear();
            }
        }
    }
}
