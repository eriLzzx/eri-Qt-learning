#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QTcpServer>
#include "serverworker.h"

class ChatServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDesciptor) override;
    QVector<ServerWorker *> m_clients;//记录所有的连接

    void broadcast(const QJsonObject &message,ServerWorker *exclude);


public slots:
    void stopServer();
    void jsonReceived(ServerWorker *sender,const QJsonObject &docObj);
    //管理多个客户端
    void userDisconnected(ServerWorker *sender);

signals:
    void logMessage(const QString &message);
};

#endif // CHATSERVER_H
