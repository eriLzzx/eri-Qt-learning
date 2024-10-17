#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    digitNums = {{Qt::Key_0,ui->btnNum0},
               {Qt::Key_1,ui->btnNum1},
               {Qt::Key_2,ui->btnNum2},
               {Qt::Key_3,ui->btnNum3},
               {Qt::Key_4,ui->btnNum4},
               {Qt::Key_5,ui->btnNum5},
               {Qt::Key_6,ui->btnNum6},
               {Qt::Key_7,ui->btnNum7},
               {Qt::Key_8,ui->btnNum8},
               {Qt::Key_9,ui->btnNum9},};
    // 数字键1-9的映射
    foreach (auto btn,digitNums)
        connect(btn,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum0,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum1,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum2,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum3,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum4,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum5,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum6,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum7,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum8,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum9,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
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
    connect(ui->btnSign,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnClear,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation(bool *ok)
{
    double result = 0;

    // 检查是否有足够的操作数和操作符
    if (operands.size() >= 2 && opcodes.size() > 0) {
        double operand1 = operands.front().toDouble();
        operands.pop_front(); // 第一个操作数出栈
        double operand2 = operands.front().toDouble();
        operands.pop_front(); // 第二个操作数出栈
        QString op = opcodes.back(); // 获取最后一个操作符
        opcodes.pop_back(); // 移除最后一个操作符

        // 根据操作符执行相应的计算
        if (op == "+") {
            result = operand1 + operand2;
        } else if (op == "-") {
            result = operand1 - operand2;
        } else if (op == "×") {
            result = operand1 * operand2;
        } else if (op == "÷") {
            if (operand2 != 0) {
                result = operand1 / operand2;
            } else {
                return QString("除数不能为0");
            }
        }
        // 将计算结果重新添加到操作数列表中
        operands.push_back(QString::number(result));
        ui->statusbar->showMessage(QString("计算中：%1 操作符数：%2")
                                       .arg(operands.size()).arg(opcodes.size()));
    } else {
        ui->statusbar->showMessage(QString("操作数数量：%1 操作符数量：%2")
                                       .arg(operands.size()).arg(opcodes.size()));
    }

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
        else if(op == "±"){
            // 反转符号
            result = -result;
        }else if(op == "CE" || op =="C")
            //清空
            result = 0;
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
    QString opcode = qobject_cast<QPushButton *>(sender())->text();

    // 检查操作符并替换
    if (!operand.isEmpty()) {
        // 如果操作数已输入，添加当前操作数到操作数列表
        operands.push_back(operand);
        operand.clear(); // 清空当前操作数
    }
    // 如果存在操作符，替换最后的操作符
    if (!opcodes.empty()) {
        opcodes.back() = opcode; // 替换最后一个操作符
    } else {
        opcodes.push_back(opcode); // 如果没有操作符，则添加
    }
    // 只在有足够的操作数和操作符时进行计算
    if (operands.size() >= 2 && opcodes.size() > 0) {
        QString result = calculation();
        ui->display->setText(result);
    } else {
        ui->statusbar->showMessage("输入不完整，请继续输入");
    }
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach(auto btnKey , digitNums.keys()){
        if(event->key() == btnKey)
            digitNums[btnKey]->animateClick();
    }
}

