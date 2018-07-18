#include "serverfilecontroller.h"

#include <QBuffer>
#include <QImageReader>

ServerFileController::ServerFileController()
{
    this->blockSize = 0;
    pictureNumber = 0;

}

ServerFileController::~ServerFileController()
{
}
/**
 * @brief ServerFileController::receivePictures
 * recevie pictures
 * input stream
 * output: save stream into file
 */
void ServerFileController::receivePictures()
{
    qDebug()<<"in";
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
        QString style;
        in>>style;
        QByteArray array = tcpSocket->read(blockSize);
        QBuffer buffer(&array);
        buffer.open(QIODevice::ReadOnly);

        QImageReader reader(&buffer,style.toStdString().c_str());
        image = reader.read();
        blockSize=0;

        if(!image.isNull())
        {
//  QDateTime time=QDateTime::currentDateTime();
//  QString str=time.toString("yyyy-MM-dd hh:mm:ss ddd");
//            QString filename = ui->picturesPathLineEdit->text()+"/"+QString("%1.").arg(pictureNumber)+style;
            image.save(this->filename);
//            pix.load(filename);
//            pictureList.push_back(pix);
            pictureNumber++;
            blockSize=0;
        }
//        pictureList[0]= pictureList[0].scaled(ui->showPictureLabel->size());
//        ui->showPictureLabel->setPixmap(pictureList[0]);
    }

    if(tcpSocket==NULL)
            return;

    if(pictureNumber<=5){
        QByteArray block;
        QDataStream back(&block,QIODevice::WriteOnly);
        back.setVersion(QDataStream::Qt_5_3);
        back <<(int)0;
        tcpSocket->write(block);
}
}

