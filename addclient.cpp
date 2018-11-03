#include "addclient.h"
#include "ui_addclient.h"
#include"serverui.h"
#include<QPushButton>
AddClient::AddClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddClient)
{
    ui->setupUi(this);
    s=(ServerUI *)parent;
    connect(ui->buttonBox->button(QDialogButtonBox::Ok),&QPushButton::clicked,this,&AddClient::onAddClient);
}
void AddClient :: onAddClient()
{
s->curClient=this->ui->textEdit->toPlainText().toStdString();
s->onAddClient();
}

AddClient::~AddClient()
{
    delete ui;
}
