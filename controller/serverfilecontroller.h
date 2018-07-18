#ifndef SERVERFILECONTROLLER_H
#define SERVERFILECONTROLLER_H

#include "filecontroller.h"

#include <QImage>
#include <QTcpSocket>



class ServerFileController : public FileController
{
public:
    ServerFileController();
    ~ServerFileController();

    void receivePictures();

    quint32 blockSize;
    QImage image;
    int pictureNumber;
    QString filename;
    QTcpSocket *tcpSocket;
};

#endif // SERVERFILECONTROLLER_H
