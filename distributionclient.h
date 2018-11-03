#ifndef DISTRIBUTIONCLIENT
#define DISTRIBUTIONCLIENT
#include<iostream>
#include<infodata.h>
#include<QTcpSocket>
#include<QThread>
using namespace std;
class InfoDataReceiver
{
public:
virtual void operator()(InfoData id1)=0;
};
class DistributionClient : public QObject
{
    Q_OBJECT
private:
    string host,name;
    InfoData infoData;
    QTcpSocket con;
    bool connected;
    InfoDataReceiver *receiver;
public:
    DistributionClient(string host,string name);
    void connect();
    bool isConnected();
    InfoData& getInfoData();
    void sendProgress(Progress &);
    void sendFile(string file);
    void setInfoDataReceiver(InfoDataReceiver *);
public slots:
    void onDataRecv();
};

#endif // DISTRIBUTIONCLIENT

