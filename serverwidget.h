#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer> //监听套接字
#include <QTcpSocket> //通信套接字
#include <QFile>
#include <QTimer>

#include <controller/serverfilecontroller.h>

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = 0);
    ~ServerWidget();

    void sendData(); //发送文件数据

    quint32 getBlockSize() const;
    void setBlockSize(const quint32 &value);

    void receivePictures();
private slots:
    void on_buttonFile_clicked();

    void on_buttonSend_clicked();

    void on_selectFolder_clicked();

private:
    Ui::ServerWidget *ui;

    QTcpServer *tcpServer; //监听套接字
    QTcpSocket *tcpSocket; //通信套接字

    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 sendSize; //已经发送文件的大小
    qint64 recvSize; //已经接受文件的大小


    QTimer timer; //定时器
    bool isStart;   //标志位，是否为头部信息


    void uiInit();
    void serverListener();
    void socketLisetener();
    void sendInfo(QString info);

//    ServerFileController *sfc;
    quint32 blockSize;
    QImage image;
    int pictureNumber;
    QString filename;
    QPixmap pix;




};

#endif // SERVERWIDGET_H
