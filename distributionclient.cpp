#include"distributionclient.h"
#include<stdlib.h>
#include<QCryptographicHash>
#include<iostream>
#include<fstream>
using namespace std;
DistributionClient :: DistributionClient(string host,string name):connected(false)
{
this->host=host;
    this->name=name;
}
void DistributionClient :: connect()
{
con.connectToHost(QString(host.c_str()),DISTRIBUTION_LISTEN_PORT);
con.waitForConnected();
ClientInfo k;
k.host=host;
k.name=name;
QByteArray id=QCryptographicHash :: hash((QString::number(rand())+QString::number(rand())
                               +QString((k.host+k.name).c_str())+QString::number(rand())
        +QString::number(rand())).toLocal8Bit(),QCryptographicHash::Md5).toHex();
qDebug() <<id <<endl;
k.id=id.toStdString();
con.write(k.toXml().c_str());
qDebug() <<"Getting Info Data";
InfoData *infoDataT=InfoData :: loadFrom(&con);
qDebug() <<"Got Info Data";
if(infoDataT==NULL)
{
    qDebug() <<"::\n";
    return;
}
infoData=*infoDataT;
qDebug() <<QString(infoData.toXml().c_str()) <<endl;
delete infoDataT;
connected=true;
QObject :: connect(&con,&QTcpSocket::readyRead,this,&DistributionClient :: onDataRecv);
}
void DistributionClient :: setInfoDataReceiver(InfoDataReceiver *recv)
{
    this->receiver=recv;
}
void DistributionClient :: onDataRecv()
{
InfoData *d1=InfoData :: loadFrom(&con);
if(d1!=NULL)
    (*receiver)(*d1);
delete d1;
}

bool DistributionClient :: isConnected()
{
    return connected;
}

InfoData& DistributionClient :: getInfoData()
{
return infoData;
}
void DistributionClient :: sendProgress(Progress &p)
{
    string s=(string)p;
    if(con.isOpen())
    {
    con.write(s.c_str(),s.size());
    con.flush();
    }
    }
void DistributionClient :: sendFile(string file)
{
    if(!con.isOpen())
        return;
    char *buf=new char[1024];
    ifstream in(file.c_str(),ios :: in | ios :: binary);
    in.read(buf,1024);
    int rd1=in.gcount();
    while(rd1>0)
    {
        con.write(buf,rd1);
        con.flush();
        in.read(buf,1024);
        rd1=in.gcount();
    }
    in.close();
    delete[] buf;
}
