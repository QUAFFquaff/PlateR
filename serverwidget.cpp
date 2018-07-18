#include "serverwidget.h"
#include "ui_serverwidget.h"
#include <QFileDialog>
#include <QDebug>
#include <QFileInfoList>
#include <QStringList>
#include <QMessageBox>
ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);

    uiInit();

    serverListener();


}

ServerWidget::~ServerWidget()
{
    delete ui;
}

//选择文件的按钮
void ServerWidget::on_buttonFile_clicked(){


}


//发送文件按钮
void ServerWidget::on_buttonSend_clicked()
{

}

/**
 * @brief ServerWidget::uiInit
 * initialize UI form
 * 初始化界面，包括属性和按钮
 */
void ServerWidget::uiInit()
{
    //监听套接字
    tcpServer = new QTcpServer(this);

    //监听
    tcpServer->listen(QHostAddress::Any, 8888);
    setWindowTitle("服务器端口为：8888");

    //两个按钮都不能按
    ui->buttonFile->setEnabled(false);
    ui->buttonSend->setEnabled(false);

    ui->serverProgressBar->setValue(0);

    isStart = true;

    //设置进度条
    ui->serverProgressBar->setMinimum(0); //最小值
    ui->serverProgressBar->setValue(0); //当前值

}

/**
 * @brief ServerWidget::serverListener
 * set server listener
 * 设置服务器监听
 */
void ServerWidget::serverListener()
{
    //如果客户端成功和服务器连接
    //tcpServer会自动触发 newConnection()
    connect(tcpServer, &QTcpServer::newConnection,
    [=]()
    {
        //取出建立好连接的套接字
        tcpSocket = tcpServer->nextPendingConnection();
        //获取对方的ip和端口
        QString ip = tcpSocket->peerAddress().toString();
        quint16 port = tcpSocket->peerPort();

        QString str = QString("[%1:%2] 成功连接").arg(ip).arg(port);
        ui->textEdit->setText(str); //显示到编辑区

        //成功连接后，才能按选择文件
        ui->buttonFile->setEnabled(true);

        socketLisetener();

    }
    );
}

/**
 * @brief ServerWidget::socketLisetener
 * set socket listener
 * 设置套接字监听
 */
void ServerWidget::socketLisetener()
{
    connect(tcpSocket, &QTcpSocket::readyRead,
            [=]()
            {
                //取客户端的信息
                QByteArray buf = tcpSocket->readAll();

                if(true == isStart)
                {//接收头
                    isStart = false;
                    //初始化文件名
                    fileName = QString(buf).section("##", 0, 0);
                    //文件大小
                    fileSize = QString(buf).section("##", 1, 1).toInt();
                    recvSize = 0;   //已经接收文件大小

                    //设置进度条
                    ui->serverProgressBar->setMinimum(0); //最小值
                    ui->serverProgressBar->setMaximum(fileSize/(1024)); //最大值
                    ui->serverProgressBar->setValue(0); //当前值
                    //打开文件
                    //关联文件名字
                    file.setFileName(fileName);

                    //只写方式，打开文件
                    bool isOk = file.open(QIODevice::WriteOnly);
                    if(false == isOk)
                    {
                        qDebug() << "WriteOnly error 49";
                        tcpSocket->disconnectFromHost(); //断开连接
                        tcpSocket->close(); //关闭套接字

                        return; //如果打开文件失败，中断函数
                    }

                    //弹出对话框，显示接收文件的信息
                    QString str = QString("接收的文件: [%1: %2kb]").arg(fileName).arg(fileSize/1024);
                    //QMessageBox::information(this, "文件信息", str);
                    ui->textEdit->append(str);
                    ui->textEdit->append("正在接收文件……");
                    //set progressBar
                    ui->serverProgressBar->setMinimum(0); //最小值
                    ui->serverProgressBar->setMaximum(fileSize/1024); //最大值
                    ui->serverProgressBar->setValue(0); //当前值
                }
                else //文件信息
                {
                    qint64 len = file.write(buf);
                    if(len >0) //接收数据大于0
                    {
                        recvSize += len; //累计接收大小
                    }

                    //更新进度条
                    ui->serverProgressBar->setValue(recvSize/(1024));

                    if(recvSize == fileSize) //文件接收完毕
                    {
                        //先给服务发送(接收文件完成的信息)
                        tcpSocket->write("file done");
                        QMessageBox::information(this, "完成", "文件接收完成");
                        file.close(); //关闭文件
                        //断开连接
                        tcpSocket->disconnectFromHost();
                        tcpSocket->close();

                    }
                }
        }

    );
}

void ServerWidget::sendInfo(QString info)
{
    qint64 len = 0;
   //每次发送数据的大小
   len = 0;
   //往文件中读数据
   len = info.toLatin1().length();
   //发送数据，读多少，发多少
   len = tcpSocket->write(info.toLatin1(), len);


}



void ServerWidget::sendData()
{
    ui->textEdit->append("正在发送文件……");
     qint64 len = 0;
     do
     {
        //每次发送数据的大小
        char buf[4*1024] = {0};
        len = 0;
        //往文件中读数据
        len = file.read(buf, sizeof(buf));
        //发送数据，读多少，发多少
        len = tcpSocket->write(buf, len);
        //发送的数据需要累积
        recvSize += len;
     }while(len > 0);


     //是否发送文件完毕
     if(recvSize == fileSize)
     {
         ui->textEdit->append("文件接收完毕");
         file.close();

         //把客户端端口
         tcpSocket->disconnectFromHost();
         tcpSocket->close();
     }


}
