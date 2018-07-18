#include "dialog.h"
#include "ui_dialog.h"
#include "recogize.h"
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    Recogize *r = new Recogize();
    QString path = "E://";
    QString filename = "test.jpg";
    if(r->rec(path,filename)){
        qDebug()<<"ok";
    }
    ui->textEdit->append(r->plateColor);
    ui->textEdit->append(r->plateNum);
    ui->textEdit->append(r->resultPath);
}
