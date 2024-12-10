#include "idatabase.h"

void IDatabase::ininDatabase()
{
    database =QSqlDatabase::addDatabase("QSQLITE"); //添加SQL数据库驱动
    QString aFile ="D:/QtLearning/eri-Qt-learning/QtLab3/Lab4.db";
    database.setDatabaseName(aFile);

    if(!database.open()){//打开数据库
        qDebug()<<"failed to open database";
    }else
        qDebug()<<"open database is ok"<<database.connectionName();
}

QString IDatabase::userLogin(QString userName, QString password)
{
    // return "loginOK";
    QSqlQuery query;
    query.prepare("select username,password from user where username = :USER");
    query.bindValue(":USER",userName);
    query.exec();
    qDebug()<<query.lastQuery()<<query.first();

    if(query.first() && query.value("username").isValid()){
        QString passwd = query.value("password").toString();
        if(passwd == password){
            return "loginOK";
        }else{
            qDebug()<<"wrong password";
            return "wrongPassword";
        }
    }else{
        qDebug()<<"no such user";
        return "wrongUsername";
    }
}

IDatabase::IDatabase(QObject *parent) : QObject{parent}
{
    ininDatabase();
}
