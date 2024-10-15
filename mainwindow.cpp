#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

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
    connect(ui->btnMulti,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnPlus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivide,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMinus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    //
    connect(ui->btnPercentage,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnInverse,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation(bool *ok)
{
    double result = 0;
    if(operands.size()==2 && opcodes.size()>0){
        //取操作数
        double operand1 = operands.front().toDouble();
        operands.pop_front();
        double operand2 = operands.front().toDouble();
        operands.pop_front();

        //取操作符
        QString op = opcodes.front();
        opcodes.pop_front();

        if(op == "+"){
            result = operand1 + operand2;
        }else if(op == "-"){
            result =operand1 - operand2;
        }else if(op == "×"){
            result =operand1 * operand2;
        }else if(op == "÷"){
            result =operand1 / operand2;
        }
        operands.push_back(QString::number(result));
        ui->statusbar->showMessage(QString ("the calculation is in progress is %1,opcode is %2").arg(operands.size()).arg
                                   (opcodes.size()));
    }
    else
        ui->statusbar->showMessage(QString ("the operands is %1,opcode is %2").arg(operands.size()).arg
                                   (opcodes.size()));
    return QString::number(result);
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

void MainWindow::btnUnaryOperatorClicked()
{
    if(operand != ""){
        double result = operand.toDouble();
        operand = "";

        QString op = qobject_cast<QPushButton *>(sender())->text();
        if(op == "%")
            result /=100.0;
        else if(op =="1/x")
            result =1/result;
        else if(op == "x^2")
            result *=result;
        else if(op == "²√x")
            result = sqrt(result);
        ui->display->setText(QString::number(result));
    }
}



void MainWindow::on_btnPeriod_clicked()
{
    if(!operand.contains("."))
        operand += qobject_cast<QPushButton *>(sender())->text();
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

void MainWindow::btnBinaryOperatorClicked()
{
    // ui->statusbar->showMessage("last operand" + operand);
    QString opcode = qobject_cast<QPushButton *>(sender())->text();
    if(operand !="")
    {
        operands.push_back(operand);
        //将前一个数保存到堆栈中，并清空原数
        operand = "";
        //将先前的操作符保存到堆栈中
        opcodes.push_back(opcode);
    }
    QString result = calculation();
    ui->display->setText(result);
}

void MainWindow::on_btnEqual_clicked()
{
    if(operand !="")
    {
        operands.push_back(operand);
        //将前一个数保存到堆栈中，并清空原数
        operand = "";
    }
    QString result = calculation();
    ui->display->setText(result);
}

