#include "filecontroller.h"

#include <QFileDialog>

FileController::FileController()
{

}

FileController::~FileController()
{

}

QString FileController::showDirectory()
{
    QFileDialog *fileDialog;
    QString dir=fileDialog->getExistingDirectory(NULL,"Open Directory",
                  "D:\\image",QFileDialog::ShowDirsOnly);
    if(fileDialog)
        delete fileDialog;
    return dir;
}

void FileController::receivePictures()
{

}

