#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>

class ChatClient : public QObject
{
    Q_OBJECT
public:
    explicit ChatClient(QObject *parent = nullptr);

private:
    QTcpSocket * m_clientSocket;

public slots:
    void onReadyRead();
    void sendMessgae(const QString &text,const QString &type ="message");
    void connectToServer(const QHostAddress &address,quint16 port);
    void disconnectFromHost();

signals:
    void connected();
    void messageReceived(const QString &text);
    void jsonReceived(const QJsonObject &docObj);
};

#endif // CHATCLIENT_H
