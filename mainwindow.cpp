#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 数字键1-9的映射
    connect(ui->btnNum0,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum1,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum2,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum3,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum4,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum5,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum6,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum7,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum8,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum9,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnNumClicked()
{
   //区别不同的按钮发射的信号，用sender来获取PushButton的文本信息
    QString str = ui->display->text();
    str += qobject_cast<QPushButton*>(sender())->text();
    ui->display->setText(str);
    ui->statusbar->showMessage(qobject_cast<QPushButton*>(sender())->text()+ "btn clicked");

}


void MainWindow::on_btnPeriod_clicked()
{
    QString str = ui->display->text();
    if(!str.contains("."))//判断条件，若当前数字已有小数点，则不添加
         str += qobject_cast<QPushButton*>(sender())->text();
    ui->display->setText(str);
}


void MainWindow::on_btnDel_clicked()
{
    //退格 回到上一个字符串的长度-1处
    QString str = ui->display->text();
    str = str.left(str.length()-1);
    ui->display->setText(str);
}

