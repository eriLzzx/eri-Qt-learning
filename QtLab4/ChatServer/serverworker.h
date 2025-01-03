#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>
#include <QTcpSocket>
class ServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit ServerWorker(QObject *parent = nullptr);
    virtual bool setSocketDescriptor(qintptr socketDescriptor);

    QString userName();
    void setUserName(QString user);
    // 静态变量用于维护所有已连接的用户名列表
    static QSet<QString> registeredUserNames;

signals:
    void logMessage(const QString &msg );
    void jsonReceived(ServerWorker *sender,const QJsonObject &docObj);
    void disconnectedFromClient();


private:
    QTcpSocket * m_serverSocket;
    QString m_userName;

public slots:
    void onReadyRead();
    void sendMessgae(const QString &text,const QString &type ="message");

    void sendJson(const QJsonObject &json);
};

#endif // SERVERWORKER_H
