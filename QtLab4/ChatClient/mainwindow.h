#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chatclient.h"
#include <QJsonObject>
#include <QJsonDocument>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_loginBtn_clicked();

    void on_sayButton_clicked();

    void on_logoutButton_clicked();

    void connectedToServer();

    void messageReceived(const QString &sender,const QString &text);

    void jsonReceived(const QJsonObject &docObj);

    void userJoined(const QString &user);

    void userLeft(const QString &user);

    void userListReceived(const QStringList &list);

    void onServerResponse(const QJsonObject &response);

    void refreshConnect();
private:
    Ui::MainWindow *ui;
    ChatClient *m_chatClient;
    bool m_canLogin = true;


};
#endif // MAINWINDOW_H
