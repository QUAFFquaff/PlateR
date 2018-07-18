#include "clientfilecontroller.h"

#include <QBuffer>
#include <QByteArray>
#include <QPixmap>
#include <QTimer>
#include <qfileinfo.h>
using namespace std;
ClientFileController::ClientFileController()
{
    picIndex = 0;
}

ClientFileController::~ClientFileController()
{

}

/**
 * @brief ClientFileController::sendPictures    send pictures in one folder
 * @param tcpSocket used to send data
 * @param picturelist   a list of picture need to be send
 * @param pictureString_list    the name of the pictures
 */
void ClientFileController::sendPictures(QTcpSocket *tcpSocket, vector<QPixmap> picturelist,QStringList pictureString_list)
{
    qDebug()<<"in";
    if(tcpSocket==NULL)
        return;
    QByteArray block;
    QBuffer buffer;
    QString style;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    qDebug()<<"size:    "<<picturelist[picIndex].size();
    //get the type of file
    style = pictureString_list[picIndex].right(3);
    picturelist[picIndex].save(&buffer,style.toStdString().c_str());
    out<<(quint32)buffer.data().size();
    out<<pictureString_list[picIndex].split("/")[pictureString_list[picIndex].split("/").length() - 1];;
    picIndex++;
    block.append(buffer.data());
    tcpSocket->write(block);
    //发送真正的文件信息
    //防止TCP黏包
    //需要通过定时器延时 20 m
    QTimer timer; //定时器
    timer.start(20);
    if(picIndex>=picturelist.size())
        picIndex = 0;
    qDebug()<<"over";

}

/**
 * @brief ClientFileController::sendPicture
 * @param tcpSocket socket used to send data
 * @param picture   the picture we want to send
 * @param path  picture path
 * send one picture
 */
void ClientFileController::sendPicture(QTcpSocket *tcpSocket, QPixmap picture,QString path)
{
    if(tcpSocket==NULL)
        return;
    QByteArray block;
    QBuffer buffer;
    QString style;
    QFileInfo info(path);
    qint64 fileSize = info.size();//get file size
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    //get the type of file
    style = path.right(3);
    picture.save(&buffer,style.toStdString().c_str());
    out<<(quint32)buffer.data().size();
    out<<path.split("/")[path.split("/").length() - 1];
    block.append(buffer.data());
    tcpSocket->write(block);
    QTimer timer; //定时器
    timer.start(20);

    qDebug()<<"over";
}

