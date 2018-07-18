#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include <qtimer.h>

namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = 0);
    ~ClientWidget();

    void sendData(); //发送文件数据
private slots:
    void on_buttonConnect_clicked();


    void on_selectFileButton_clicked();

    void on_sendFileButton_clicked();

private:
    Ui::ClientWidget *ui;

    QTcpSocket *tcpSocket;

    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 recvSize; //已经接收文件的大小

    qint64 sendSize; //已经发送文件的大小

    bool isStart;   //标志位，是否为头部信息


    QTimer timer; //定时器

    void uiInit();
    void setsocketListener();
};

#endif // CLIENTWIDGET_H
