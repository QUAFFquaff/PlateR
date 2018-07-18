#ifndef CONNECTCONTROLLER_H
#define CONNECTCONTROLLER_H

#include <QFile>
#include <QTcpSocket>
#include <QTextEdit>
#include <QTimer>



class ConnectController
{
public:
    ConnectController(
            QTextEdit *textEdit);
    ~ConnectController();
    virtual void sendData();
    virtual void setReadListener();


    QTcpSocket *tcpSocket;
    QTextEdit *textEdit;
    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 recvSize; //已经接收文件的大小

    qint64 sendSize; //已经发送文件的大小

    bool isStart;   //标志位，是否为头部信息


    QTimer timer; //定时器
};

#endif // CONNECTCONTROLLER_H
