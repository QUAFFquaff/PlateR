#ifndef CLIENTFILECONTROLLER_H
#define CLIENTFILECONTROLLER_H

#include "filecontroller.h"

#include <QTcpSocket>



class ClientFileController : public FileController
{
public:
    ClientFileController();
    ~ClientFileController();


    void sendPictures(QTcpSocket *tcpSocket);

private:
    void sendPictures();
};

#endif // CLIENTFILECONTROLLER_H
