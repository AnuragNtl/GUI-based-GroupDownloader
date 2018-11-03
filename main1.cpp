#include "mainwindow.h"
#include <QPushButton>
#include<QApplication>
#include<iostream>
#include<QtNetwork/QHostAddress>
#include<QtNetwork/QTcpServer>
#include<QtNetwork/QTcpSocket>
#include"t1.h"
#include<QWidget>
#include<QSignalMapper>
using namespace std;
QTcpServer *q;
QTcpSocket *s;
RecvHandler :: RecvHandler(QWidget *q){}
RecvHandler :: ~RecvHandler(){}
void RecvHandler :: onRecv()
{
    cout <<"Connected\n";
cout <<q->hasPendingConnections();
    s=q->nextPendingConnection();
if(s==NULL)
{
    cout <<"::\n";
    return;
}
QObject :: connect(s,SIGNAL(readyRead()),this,SLOT(startRead()));
//(QApplication::instance())->quit();
}
void RecvHandler :: startRead()
{
    char *rd=new char[5];

    for(int i=0;i<20;i++)
    {
        int r=s->read(rd,4);
        cout <<r <<"\n";
    rd[r]='\0';
    cout <<rd <<endl;
    }
}
int main(int argc, char *argv[])
{
    cout <<"Started\n";
    QApplication a(argc, argv);
    //MainWindow w;
   // w.show();
    q=new QTcpServer;
    bool r=q->listen(QHostAddress::Any,8082);
    RecvHandler rh(NULL);
    QObject::connect(q,SIGNAL(newConnection()),&rh,SLOT(onRecv()));
    return a.exec();
}
