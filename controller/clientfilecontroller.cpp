#include "clientfilecontroller.h"

#include <QBuffer>
#include <QByteArray>

ClientFileController::ClientFileController()
{

}

ClientFileController::~ClientFileController()
{

}

void ClientFileController::sendPictures(QTcpSocket *tcpSocket)
{
    if(tcpSocket==NULL)
        return;
    QByteArray block;
    QBuffer buffer;
    QString style;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    style = pictureString_list[picIndex].right(3);
    picturelist[picIndex].save(&buffer,style.toStdString().c_str());
    picIndex++;
    out<<(quint32)buffer.data().size();
    out<<style;
    block.append(buffer.data());
    tcpSocket->write(block);
    if(picIndex>=picturelist.size())
        picIndex = 0;
}

