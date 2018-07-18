#include "serverwidget.h"
#include "ui_serverwidget.h"
#include <QFileDialog>
#include <QDebug>
#include <QFileInfoList>
#include <QStringList>
#include <QMessageBox>
#include <QBuffer>
#include <QImageReader>
#include <controller/utility.h>
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
//    sfc = new ServerFileController();
//    sfc->tcpSocket = this->tcpSocket;

    //设置进度条
    ui->serverProgressBar->setMinimum(0); //最小值
    ui->serverProgressBar->setValue(0); //当前值

    this->blockSize = 0;
    pictureNumber = 0;
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

        socketLisetener();

    }
    );

//    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(ServerWidget::receivePictures()));
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
        receivePictures();
//                //取客户端的信息
//                QByteArray buf = tcpSocket->readAll();

//                if(true == isStart)
//                {//接收头
//                    isStart = false;
//                    //初始化文件名
//                    fileName = QString(buf).section("##", 0, 0);
//                    //文件大小
//                    fileSize = QString(buf).section("##", 1, 1).toInt();
//                    recvSize = 0;   //已经接收文件大小

//                    //设置进度条
//                    ui->serverProgressBar->setMinimum(0); //最小值
//                    ui->serverProgressBar->setMaximum(fileSize/(1024)); //最大值
//                    ui->serverProgressBar->setValue(0); //当前值
//                    //打开文件
//                    //关联文件名字
//                    file.setFileName(fileName);

//                    //只写方式，打开文件
//                    bool isOk = file.open(QIODevice::WriteOnly);
//                    if(false == isOk)
//                    {
//                        qDebug() << "WriteOnly error 49";
//                        tcpSocket->disconnectFromHost(); //断开连接
//                        tcpSocket->close(); //关闭套接字

//                        return; //如果打开文件失败，中断函数
//                    }

//                    //弹出对话框，显示接收文件的信息
//                    QString str = QString("接收的文件: [%1: %2kb]").arg(fileName).arg(fileSize/1024);
//                    //QMessageBox::information(this, "文件信息", str);
//                    ui->textEdit->append(str);
//                    ui->textEdit->append("正在接收文件……");
//                    //set progressBar
//                    ui->serverProgressBar->setMinimum(0); //最小值
//                    ui->serverProgressBar->setMaximum(fileSize/1024); //最大值
//                    ui->serverProgressBar->setValue(0); //当前值
//                }
//                else //文件信息
//                {
//                    qint64 len = file.write(buf);
//                    if(len >0) //接收数据大于0
//                    {
//                        recvSize += len; //累计接收大小
//                    }

//                    //更新进度条
//                    ui->serverProgressBar->setValue(recvSize/(1024));

//                    if(recvSize == fileSize) //文件接收完毕
//                    {
//                        //先给服务发送(接收文件完成的信息)
//                        tcpSocket->write("file done");
//                        QMessageBox::information(this, "完成", "文件接收完成");
//                        file.close(); //关闭文件
//                        //断开连接
//                        tcpSocket->disconnectFromHost();
//                        tcpSocket->close();

//                    }
//                }
        }

    );
}

void ServerWidget::sendInfo(QString info)
{

}



void ServerWidget::sendData()
{

}

void ServerWidget::on_selectFolder_clicked()
{
    QFileDialog * fileDialog=new QFileDialog(this);
    QString dir=fileDialog->getExistingDirectory(this,"Open Directory",
                  "D:\\image",QFileDialog::ShowDirsOnly);
    ui->serverFilePath->setText(dir);
    if(fileDialog)
        delete fileDialog;
//    sfc->filename = dir;
//    sfc->tcpSocket = this->tcpSocket;
}

void ServerWidget::receivePictures()
{
    qDebug()<<"in function receivePictures";
    while(tcpSocket->bytesAvailable()>0)
    {
        QDataStream in(tcpSocket);
        if(blockSize==0)
        {

            in.setVersion(QDataStream::Qt_5_3);
            if(tcpSocket->bytesAvailable()<sizeof(quint32))
                return;
            in>>blockSize;
        }

        if(tcpSocket->bytesAvailable()<blockSize)
            return;
        QString fname;
        in>>fname;
        QByteArray array = tcpSocket->read(blockSize);
        QBuffer buffer(&array);
        buffer.open(QIODevice::ReadOnly);

        QImageReader reader(&buffer,fname.right(3).toStdString().c_str());
        image = reader.read();
        blockSize=0;

        if(!image.isNull())
        {
            QString filename = ui->serverFilePath->text()+"/"+fname;
            ui->textEdit->append(filename);
            qDebug()<<fname;
            image.save(filename);
//            pix.load(filename);
//            pictureList.push_back(pix);
            pictureNumber++;
            qDebug()<<"pictureNumber:   "<<pictureNumber;
            blockSize=0;
        }
    }

    if(tcpSocket==NULL)
            return;

//    if(pictureNumber<=5){
//        QByteArray block;
//        QDataStream back(&block,QIODevice::WriteOnly);
//        back.setVersion(QDataStream::Qt_5_3);
//        back <<(int)0;
//        tcpSocket->write(block);
//    }
}
