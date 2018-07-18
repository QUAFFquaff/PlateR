//#include "connectcontroller.h"

//#include <QMessageBox>

//ConnectController::ConnectController(QTextEdit *textEdit)
//{
//    tcpSocket = new QTcpSocket();

//    connect(tcpSocket, &QTcpSocket::readyRead,
//    [=]()
//    {
//        //取出接收的内容
//        QByteArray buf = tcpSocket->readAll();

//        if(true == isStart)
//        {//接收头
//            isStart = false;
//            //解析头部信息 QString buf = "hello##1024"
//            //                    QString str = "hello##1024#mike";
//            //                            str.section("##", 0, 0)

//            //初始化
//            //文件名
//            fileName = QString(buf).section("##", 0, 0);
//            //文件大小
//            fileSize = QString(buf).section("##", 1, 1).toInt();
//            recvSize = 0;   //已经接收文件大小

//            //打开文件
//            //关联文件名字
//            file.setFileName(fileName);

//            //只写方式，打开文件
//            bool isOk = file.open(QIODevice::WriteOnly);
//            if(false == isOk)
//            {
//                qDebug() << "WriteOnly error 49";

//                tcpSocket->disconnectFromHost(); //断开连接
//                tcpSocket->close(); //关闭套接字

//                return; //如果打开文件失败，中断函数
//            }

//            //弹出对话框，显示接收文件的信息
//            QString str = QString("接收的文件: [%1: %2M]").arg(fileName).arg(fileSize/(1024*1024));
//            QMessageBox::information(NULL, "文件信息", str);

//            //设置进度条
////            ui->progressBar->setMinimum(0); //最小值
////            ui->progressBar->setMaximum(fileSize/(1024*1024)); //最大值
////            ui->progressBar->setValue(0); //当前值

//        }
//        else //文件信息
//        {
//            qint64 len = file.write(buf);
//            if(len >0) //接收数据大于0
//            {
//                recvSize += len; //累计接收大小
//                qDebug() << len;
//            }

//            //更新进度条
////            ui->progressBar->setValue(recvSize/(1024*1024));

//            if(recvSize == fileSize) //文件接收完毕
//            {

//                //先给服务发送(接收文件完成的信息)
//                tcpSocket->write("file done");

//                QMessageBox::information(NULL, "完成", "文件接收完成");
//                file.close(); //关闭文件
//                //断开连接
//                tcpSocket->disconnectFromHost();
//                tcpSocket->close();

//            }
//        }

//        }

//    );


//}

//ConnectController::~ConnectController()
//{

//}

//void ConnectController::sendData()
//{

//    qint64 len = 0;
//    do
//    {
//    //每次发送数据的大小
//    char buf[4*1024] = {0};
//    len = 0;

//    //往文件中读数据
//    len = file.read(buf, sizeof(buf));
//    //发送数据，读多少，发多少
//    len = tcpSocket->write(buf, len);

//    //发送的数据需要累积
//    sendSize += len;

//    }while(len > 0);


//    //是否发送文件完毕
//    if(sendSize == fileSize)
//    {
//     textEdit->append("文件发送完毕");
//     file.close();

//     //把客户端端口
//     tcpSocket->disconnectFromHost();
//     tcpSocket->close();
//    }

//}

//void ConnectController::setReadListener()
//{

//}

