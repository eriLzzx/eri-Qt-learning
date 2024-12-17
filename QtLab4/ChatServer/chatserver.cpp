#include "chatserver.h"
#include "serverworker.h"

ChatServer::ChatServer(QObject *parent):
    QTcpServer(parent) {

}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    ServerWorker *worker = new ServerWorker(this);
    if(!worker->setSocketDescriptor(socketDescriptor)){
        //创建失败，删除
        worker->deleteLater();
        return ;
    }
    //创建成功，放入列表
    m_clients.append(worker);
    emit logMessage("新的用户已连接");
}

void ChatServer::stopServer()
{
    close();
}
