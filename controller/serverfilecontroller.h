#ifndef SERVERFILECONTROLLER_H
#define SERVERFILECONTROLLER_H

#include "filecontroller.h"

#include <QTcpSocket>



class ServerFileController : public FileController
{
public:
    ServerFileController();
    ~ServerFileController();

    void receivePictures(QTcpSocket *tcpSocket);
};

#endif // SERVERFILECONTROLLER_H
