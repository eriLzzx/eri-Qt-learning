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
    QVector<ServerWorker *> m_clients;

public slots:
    void stopServer();
    //管理多个客户端


signals:
    void logMessage(const QString &message);
};

#endif // CHATSERVER_H
