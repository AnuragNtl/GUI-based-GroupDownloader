#include "mainwindow.h"
#include <QPushButton>
#include<QApplication>
#include<iostream>
#include<QWidget>
#include"clientui.h"
#include"serverui.h"
#include"Merge.h"
#include"distribution.h"
#include"infodata.h"
#include<QtXml/QXmlSimpleReader>
#include<QtXml/QXmlInputSource>
#include<QTXml>
#include"downloader.h"
#include"distributionclient.h"
#include"partialdownloader.h"
using namespace std;
void handleInfoDataRecv(InfoData);
ClientUI *clientUi;
ServerUI *serverUi;
MainWindow *w;
Distribution *d;
void MainWindow :: onStartServerClicked()
{
    d=new Distribution;
    d->setAutoAccept(true);
    serverUi=new ServerUI(NULL,d);
    serverUi->show();
    this->hide();
}
void MainWindow :: onStartClientClicked()
{

    clientUi=new ClientUI;
    clientUi->show();
    this->hide();
}
class ProgRecv1 : public ProgressReceiver
{
private:
    Progress prog;
    DistributionClient *client;
public:
    ProgRecv1(Downloader *d,string name,string host,DistributionClient *client) :
        ProgressReceiver(d)
    {
       prog.name=name;
       prog.started=1;
       prog.cancelled=0;
       prog.error=0;
       prog.finished=0;
       prog.host=host;
       prog.progress=0;
       this->client=client;
    }
    void operator()(int progress)
    {
        qDebug() <<prog.toXml().c_str();
        prog.progress=progress;
        clientUi->updateProgress(progress);
        if(progress<100)
        client->sendProgress(prog);
        else if(progress==100)
        {
            prog.finished=true;
            client->sendProgress(prog);
        }
    }
};
class IDataReceiver : public InfoDataReceiver
{
public:
    void operator()(InfoData d1)
    {
        qDebug() <<"Info Data Received " <<d1.toXml().c_str() <<endl;
        handleInfoDataRecv(d1);
    }
};

PartialDownloader *pDownloader;
DistributionClient *k;
void handleInfoDataRecv(InfoData d1)
{
if(d1.sendMergeData)
    k->sendFile(d1.downloadId.c_str());
}

void ClientUI :: onStarted(QString host,QString name)
{
   k=new DistributionClient(host.toStdString(),name.toStdString());
   k->setInfoDataReceiver(new IDataReceiver);
   qDebug() <<"Connecting";
   while(!k->isConnected())
   k->connect();
   qDebug() <<"Connected";
    InfoData &info=k->getInfoData();
    pDownloader=new PartialDownloader(info.host.c_str(),info.path.c_str());
    pDownloader->setProgressReceiver(new ProgRecv1(pDownloader,name.toStdString(),host.toStdString(),k));
    QString hdrs=info.hdrs.c_str();
    QStringList l1=hdrs.split("\n",QString::SkipEmptyParts);
    qDebug() <<l1.count();
    QStringList :: iterator it1=l1.begin();
    while(it1!=l1.end())
    {
        QString eHdr=*it1;
        qDebug() <<eHdr;
        QStringList l2=eHdr.split(":");
        qDebug() <<l2.count();
        if(l2.count()==2)
        pDownloader->setHeader(l2.at(0),l2.at(1));
        it1++;
    }
    qDebug() <<"___:::::::____";
    pDownloader->setRange(&(info.r));
    pDownloader->start(info.downloadId.c_str());
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w=new MainWindow;
    w->show();
    /*QXmlSimpleReader *xr=new QXmlSimpleReader;
    QXmlInputSource src;
    src.setData(QString("<name ee='abcd'><n1>Abcd</n1><n2>Efgh</n2></name>"));
    xr->setContentHandler(new Handler1);
    bool r=xr->parse(&src,true);
        cout <<r <<endl;
    map<string,string> *mp1=(MapFromSimpleXML :: getData("<name ee='abcd'><n1>Abcd</n1><n2>Efgh</n2></name>"));
    map<string,string> :: iterator t=mp1->begin();
    for(t=t;t!=mp1->end();t++)
        cout <<t->first <<" " <<t->second <<endl;*/
    /*Range r={10,20};
    InfoData d2={"Host","Path","Request Type","Headers","FormData",
                 r,"DownloadId",1,1,1};
    string s=d2.toXml();
    cout <<s <<endl;
    InfoData *d1;
    if(d1=InfoData::loadFrom(s))
    cout <<*d1 <<endl;
    else
        cout <<"::\n";*/
    /*Progress p={"Host","Name",1,0,2000,0,1};
    string s=p.toXml();
    cout <<s <<endl;
    Progress *p1;
    if(p1=Progress::loadFrom(s))
        cout <<*p1 <<endl;
    else
        cout <<"::\n";*/
    /*Downloader d("http://127.0.0.1/");
    d.start("T1.txt");
    cout <<"Downloaded\n";*/
    /*InfoData d1;
    cout <<d1 <<"abcd\n" <<endl;*/
    cout <<"Started\n";
    /*Distribution d;
    d.start();
    return a.exec();*/
    /*DistributionClient k("localhost","I1");
    k.connect();*/
    return a.exec();
}
