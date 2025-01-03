#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
    m_chatClient = new ChatClient(this);

    connect(m_chatClient,&ChatClient::connected,this,&MainWindow::connectedToServer);
    // connect(m_chatClient,&ChatClient::messageReceived,this,&MainWindow::messageReceived);

    connect(m_chatClient,&ChatClient::jsonReceived,this,&MainWindow::jsonReceived);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginBtn_clicked()
{
  //如果连接上了服务器,通过TcpSocket发给ChatClient，传给connected，然后调用connectedToServer
    m_chatClient->connectToServer(QHostAddress(ui->serverEdit->text()),1967);
}


void MainWindow::on_sayButton_clicked()
{
    if(!ui->saylineEdit->text().isEmpty())
        m_chatClient->sendMessgae(ui->saylineEdit->text());
}


void MainWindow::on_logoutButton_clicked()
{
    m_chatClient->disconnectFromHost();
    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
    //在listwidget列表中寻找
    for(auto aItem : ui->userListWidget->findItems(ui->userNameEdit->text(),Qt::MatchExactly)){
        qDebug("remove");
        ui->userListWidget->removeItemWidget(aItem);
        delete aItem;
    }
}

void MainWindow::connectedToServer()
{
    ui->stackedWidget->setCurrentWidget(ui->ChatPage);
    m_chatClient->sendMessgae(ui->userNameEdit->text(),"login");
}

void MainWindow::messageReceived(const QString &sender,const QString &text)
{
    ui->roomTextEdit->append(QString("%1：%2").arg(sender).arg(text));
}

void MainWindow::jsonReceived(const QJsonObject &docObj)
{
    const QJsonValue typeVal = docObj.value("type");
    if(typeVal.isNull() || !typeVal.isString())
        return ;
    if(typeVal.toString().compare("message",Qt::CaseInsensitive) == 0){
        const QJsonValue textVal = docObj.value("text");
        const QJsonValue senderVal = docObj.value("sender");
        if(textVal.isNull() || !textVal.isString())
            return;
        if(senderVal.isNull() || !senderVal.isString())
            return;
        messageReceived(senderVal.toString(),textVal.toString());

    }else if(typeVal.toString().compare("newuser",Qt::CaseInsensitive) == 0 ){
        const QJsonValue usernameVal = docObj.value("username");
        if(usernameVal.isNull() || !usernameVal.isString())
            return;

        userJoined(usernameVal.toString());
    }else if(typeVal.toString().compare("userdisconnected",Qt::CaseInsensitive) == 0 ){
        const QJsonValue usernameVal = docObj.value("username");
        if(usernameVal.isNull() || !usernameVal.isString())
            return;

        userLeft(usernameVal.toString());
    }else if(typeVal.toString().compare("userlist",Qt::CaseInsensitive) == 0 ){
        const QJsonValue userlistVal = docObj.value("userlist");
        if(userlistVal.isNull() || !userlistVal.isArray())
            return;
        //更新成字符串列表
        userListReceived(userlistVal.toVariant().toStringList());
    }
}

void MainWindow::userJoined(const QString &user)
{
    ui->userListWidget->addItem(user);
}

void MainWindow::userLeft(const QString &user)
{
    //在listwidget列表中寻找
    for(auto aItem : ui->userListWidget->findItems(user,Qt::MatchExactly)){
        qDebug("remove");
        ui->userListWidget->removeItemWidget(aItem);
        delete aItem;
    }
}

void MainWindow::userListReceived(const QStringList &list)
{
    //清空原先的列表再添加新列表
    ui->userListWidget->clear();
    ui->userListWidget->addItems(list);
}

void MainWindow::refreshConnect()
{
    m_chatClient->connectToServer(QHostAddress(ui->serverEdit->text()), 1967);
    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
}

