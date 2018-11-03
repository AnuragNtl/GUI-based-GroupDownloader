#include "clientui.h"
#include "ui_clientui.h"

ClientUI::ClientUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientUI)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton :: clicked,this,&ClientUI :: onStart);
}

ClientUI::~ClientUI()
{
    delete ui;
}
void ClientUI :: onStart()
{
    onStarted(ui->textEdit_2->toPlainText(),ui->textEdit->toPlainText());
}
void ClientUI :: updateProgress(int p)
{
ui->progressBar->setValue(p);
}
