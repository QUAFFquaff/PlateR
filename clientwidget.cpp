#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QFileDialog>
#include <QDebug>
#include <QFileInfoList>
#include <QMessageBox>
#include <QHostAddress>
#include <qfileinfo.h>
#include <controller/utility.h>
ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);

    uiInit();

//    connect(&timer, &QTimer::timeout,
//            [=]()
//            {
//                //关闭定时器
//                timer.stop();
//                //发送文件
//                sendData();
//            }
//            );
}

ClientWidget::~ClientWidget()
{
    delete ui;
}
/**
 * @brief ClientWidget::sendData
 * send data to server.
 */
void ClientWidget::sendData()
{
    ui->clientTextEdit->append("正在发送文件……");
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
        sendSize += len;
        //set progress bar
        ui->progressBar->setValue(sendSize/(1024));
     }while(len > 0);
     //是否发送文件完毕
     if(sendSize == fileSize)
     {
         ui->clientTextEdit->append("文件发送完毕");
         file.close();
         setsocketListener();
     }else{
         ui->clientTextEdit->append("文件发送未完成");
     }
}
/**
 * @brief ClientWidget::on_buttonConnect_clicked
 * when click connect button triger this function.
 * set connection with the server.
 */
void ClientWidget::on_buttonConnect_clicked()
{
    //获取服务器的ip和端口
    QString ip = ui->lineEditIP->text();
    quint16 port = ui->lineEditPort->text().toInt();

    //主动和服务器连接
    tcpSocket->connectToHost(QHostAddress(ip), port);

    isStart = true;

}

/**
 * @brief ClientWidget::on_selectFileButton_clicked
 * select file to send
 */
void ClientWidget::on_selectFileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "open", "../");
    if(false == filePath.isEmpty()) //如果选择文件路径有效
    {
        fileName.clear();
        fileSize = 0;

        //获取文件信息
        QFileInfo info(filePath);
        fileName = info.fileName(); //获取文件名字
        fileSize = info.size(); //获取文件大小
        sendSize = 0; //发送文件的大小
        //只读方式打开文件
        //指定文件的名字
        file.setFileName(filePath);
        //打开文件
        bool isOk = file.open(QIODevice::ReadOnly);
        if(false == isOk)
        {
            qDebug() << "只读方式打开文件失败 106";
        }
        //提示打开文件的路径
        ui->clientTextEdit->append(filePath);
        ui->clientFilePath->setText(filePath);

//        ui->selectFileButton->setEnabled(false);
        ui->sendFileButton->setEnabled(true);

    }
    else
    {
        qDebug() << "选择文件路径出错 118";
    }
}
/**
 * @brief ClientWidget::on_sendFileButton_clicked send one picture
 * when click the button, triger this function
 */
void ClientWidget::on_sendFileButton_clicked()
{

    QPixmap pix;
    QString dir = ui->clientFilePath->text();
    pix.load(dir);

    cfc->sendPicture(tcpSocket,pix,dir);


//    //先发送文件头信息  文件名##文件大小
//    QString head = QString("%1##%2").arg(fileName).arg(fileSize);
//    qDebug()<<head;
//    //发送头部信息
//    qint64 len = tcpSocket->write( head.toUtf8() );


//    ui->progressBar->setMinimum(0); //最小值
//    ui->progressBar->setMaximum(fileSize/(1024)); //最大值
//    ui->progressBar->setValue(0); //当前值
//    if(len > 0)//头部信息发送成功
//    {
//        //发送真正的文件信息
//        //防止TCP黏包
//        //需要通过定时器延时 20 ms
//        timer.start(20);
//    }
//    else
//    {
//        qDebug() << "头部信息发送失败 142";
//        file.close();
//        ui->sendFileButton->setEnabled(true);
//        ui->selectFileButton->setEnabled(false);
//    }
}

/**
 * @brief ClientWidget::uiInit
 * initialize ui form and basic class attributes
 */
void ClientWidget::uiInit()
{

    tcpSocket = new QTcpSocket(this);

    isStart = true;

    ui->progressBar->setValue(0); //当前值

    setWindowTitle("客户端");

    cfc = new ClientFileController;
}

/**
 * @brief ClientWidget::setsocketListener
 * listening to the response from the server.
 */
void ClientWidget::setsocketListener()
{

    connect(tcpSocket, &QTcpSocket::readyRead,
            [=]()
            {
                //取客户端的信息
                QByteArray buf = tcpSocket->readAll();
                if(QString(buf) == "file done")
                {//文件接收完毕
                     ui->clientTextEdit->append(buf.data());
                     qDebug()<<"sended";
                     file.close();

//                     //断开客户端端口
//                     tcpSocket->disconnectFromHost();
//                     tcpSocket->close();
                }

            }

            );

}

/**
 * @brief ClientWidget::on_selectFolder_clicked
 * select folder and add all pictures into the list
 */
void ClientWidget::on_selectFolder_clicked()
{
    fileSize = 0;
    QFileDialog *fileDialog = new QFileDialog(this);
    QString dir = fileDialog->getExistingDirectory(this,"Open Directory",
                  "D:\\Users\\lenovo\\Desktop",QFileDialog::ShowDirsOnly);
    ui->clientFilePath->setText(dir);
    if(dir==NULL)
        return;
    QString picturePath = ui->clientFilePath->text();
    GetFolderImages(picturePath,pictureString_list,false);
    for(int i=0;i<pictureString_list.size();i++)
    {
        QPixmap pix;
        pix.load(pictureString_list[i]);
        picturelist.push_back(pix);
    }
    if(fileDialog)
        delete fileDialog;
    qDebug()<<picturelist.size();
}

/**
 * @brief ClientWidget::on_sendPictures_clicked
 * when the button named "sendPictures" been clicked triger this function
 * this function used to send pictures in one folder.
 */
void ClientWidget::on_sendPictures_clicked()
{
    qDebug()<<picturelist.size();
    for (int i = 0;i < picturelist.size(); i++){

        cfc->sendPictures(tcpSocket,picturelist,pictureString_list);
    }
    ui->clientTextEdit->append("send over");
}
