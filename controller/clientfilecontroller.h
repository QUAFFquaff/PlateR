#ifndef CLIENTFILECONTROLLER_H
#define CLIENTFILECONTROLLER_H

#include "filecontroller.h"

#include <QTcpSocket>

using namespace std;

class ClientFileController : public FileController
{
public:
    ClientFileController();
    ~ClientFileController();

    int picIndex;
//    vector<QPixmap> picturelist;


    void sendPictures(QTcpSocket *tcpSocket, vector<QPixmap> picturelist, QStringList pictureString_list);
    void sendPicture(QTcpSocket *tcpSocket, QPixmap picture, QString path);

};

#endif // CLIENTFILECONTROLLER_H
