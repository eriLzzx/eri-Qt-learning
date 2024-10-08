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
    //操作符映射
    connect(ui->btnMulti,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked));
    connect(ui->btnPlus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked));
    connect(ui->btnDivide,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked));
    connect(ui->btnMinus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnNumClicked()
{
   //区别不同的按钮发射的信号，用sender来获取PushButton的文本信息
    QString digt = qobject_cast<QPushButton*>(sender())->text();
   if(digt == "0" && operand == "0")
        digt = "";
   if(digt !="0" && operand == "0")
       operand = "";
    operand += digt;


    ui->display->setText(operand);
}

void MainWindow::btnBinaryOperatorClicked()
{
    if(operand !="")
    {
        operands.push_back(operand);
        //将前一个数保存到堆栈中，并清空原数
        operand = "";
    }
}


void MainWindow::on_btnPeriod_clicked()
{
    if(!operand.contains("."))//判断条件，若当前数字已有小数点，则不添加小数点
         operand += qobject_cast<QPushButton*>(sender())->text();
    ui->display->setText(operand);
}


void MainWindow::on_btnDel_clicked()
{
    //退格 回到上一个字符串的长度-1处
    operand = operand.left(operand.length()-1);
    ui->display->setText(operand);
}


void MainWindow::on_btnClear_clicked()
{
    operand.clear();
    ui->display->setText(operand);
}


void MainWindow::on_btnEqual_clicked()
{
    if(operand !="")
    {
        operands.push_back(operand);
        //将前一个数保存到堆栈中，并清空原数
        operand = "";
    }

}

