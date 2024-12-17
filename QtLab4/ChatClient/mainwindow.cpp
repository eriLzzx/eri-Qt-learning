#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginBtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ChatPage);
}


void MainWindow::on_sayButton_clicked()
{

}


void MainWindow::on_logoutButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
}

