#include"distribution.h"
#include<QHostAddress>
#include<QApplication>
#include<fstream>
#include<QFileInfo>
#include"Merge.h"
void Distribution :: addInfoData(string name,InfoData id)
{
infoDatas.insert(pair<string,InfoData>(name,id));
}
Distribution :: ~Distribution()
{

}
Distribution :: Distribution() : prog(new map<string,Progress>)
{

}
bool Distribution :: isConnected(string n)
{
    map<string,bool> :: iterator it1=connectedList.find(n);
    if(it1==connectedList.end())
        return false;
    else
        return connectedList[n];
}
void Distribution :: setConnected(string n,bool v)
{
    connectedListMutex.lock();
    connectedList[n]=v;
    connectedListMutex.unlock();
}

Distribution :: Distribution(string name,InfoData d) : prog(new map<string,Progress>)
{
    addInfoData(name,d);
}
void Distribution :: start()
{
    qDebug() <<"Listening\n";
this->listen(QHostAddress::Any,DISTRIBUTION_LISTEN_PORT);
    qDebug() <<"Listening Issued\n";
}
void Distribution :: setReceiver(DistributionReceiver *r)
{
    this->r=r;
}
DistributionThread :: DistributionThread(Distribution *s,qintptr h) : d(s),acceptFile(false),
    closed(false)
{
    handle=h;
}
void Distribution :: setAutoAccept(bool s)
{
    autoAccept=s;
}
void Distribution :: setUploadProgress(string n,int v)
{
    uploadProgressMutex.lock();
    uploadProgress[n]=v;
    uploadProgressMutex.unlock();
}
int Distribution :: getUploadProgress(string n)
{
    if(uploadProgress.find(n)!=uploadProgress.end())
    return uploadProgress[n];
    else
        return 0;
}

void DistributionThread :: run()
{
closed=false;
    con=new QTcpSocket;
con->setSocketDescriptor(handle);
ClientInfo *k=ClientInfo::loadFrom(con);
if(k==NULL)
    qDebug() <<"Error in Parsing\n";
else
qDebug() <<"Client Info Received " <<QString(k->toXml().c_str());
name=k->name;
d->setStatus(k->name,Distribution::CONNECTED);
InfoData *data1=d->findInfoData(k->name);
d->setStatus(k->name,Distribution::RECEIVING_CLIENT_INFO);
if(data1==NULL)
    return;
//qDebug() <<QString(data1->toXml().c_str()) <<endl;
d->setStatus(k->name,Distribution::SEND_DOWNLOAD_DATA);
InfoData d1=*data1;
qDebug() <<"Sending Info Data";
d1.fdata="frmData";
con->write(QByteArray(d1.toXml().c_str()));
con->flush();
qDebug() <<"Info Data Sent";
d->setConnected(k->name,true);
d->setStatus(k->name,Distribution::DOWNLOADING);
Progress *p=Progress::loadFrom(con);
while(con->isOpen())
{
qDebug() <<"Receiving Progress";
    if(p!=NULL)
    {
        qDebug() <<"Progress Receive Success";
    d->setProgress(p->name,p);
    if(p->finished && p->progress==100)
        break;
    qDebug() <<"Progress Received " <<p->toXml().c_str();
    }
    qDebug() <<"Done Receiving Progress";
p=Progress::loadFrom(con);
}
while(con->isOpen() && !acceptFile);
if(!con->isOpen())
{
    d->setConnected(k->name,false);
    return;
}
InfoData sendFileSignal;
sendFileSignal=d1;
sendFileSignal.start=0;
sendFileSignal.sendMergeData=1;
sendFileSignal.stop=1;
qDebug() <<"Signalling Send File";
con->write(sendFileSignal.toXml().c_str());
con->flush();
qDebug() <<"Send File Signalled";
d->setStatus(k->name,Distribution::UPLOADING);
oFile.open((p->name+d1.downloadId).c_str(),ios :: out | ios :: binary);
sz=d1.r.end-d1.r.start+1;
e1=new QEventLoop;
connect(con,&QTcpSocket :: readyRead,this,&DistributionThread :: receiveFile);
connect(con,&QTcpSocket :: aboutToClose,this,&DistributionThread :: onAboutToClose);
e1->exec();
oFile.close();
d->setConnected(k->name,false);
delete e1;
}
void Distribution :: setStatus(string n,STATUS v)
{
    statusMutex.lock();
    status[n]=v;
    statusMutex.unlock();
}
void Distribution :: mergeDownloads()
{
    MergerThread *thrd1=new MergerThread(this);
    thrd1->start();
}
MergerThread :: MergerThread(Distribution *d)
{
this->d=d;
}

void MergerThread :: run()
{
    map<string,InfoData> :: iterator it1=d->infoDatas.begin();
    if(it1==d->infoDatas.end())
    {
        return;
    }
    Merger merger(conv(it1->second.downloadId.c_str()));
    while(it1!=d->infoDatas.end())
    {
        InfoData d1=it1->second;
        string name=it1->first;
        string f=name+d1.downloadId;
        QFileInfo fInfo(f.c_str());
        if(fInfo.exists())
        {
            merger=merger+conv(f.c_str());
        }
        it1++;
    }

}

Distribution::STATUS Distribution :: getStatus(string n)
{
    return status[n];
}

void DistributionThread :: onAboutToClose()
{
    d->setConnected(name,false);
}

void DistributionThread :: receiveFile()
{
    char *buf=new char[UNIV_BUF_SIZE];
    int rd1=con->read(buf,UNIV_BUF_SIZE);
    while(rd1>0)
    {
        recvd+=rd1;
        int prog=(int)((recvd*100.0)/sz);
        d->setUploadProgress(name,prog);
        if(recvd>=sz)
        {
            e1->quit();
            return;
        }
        oFile.write(buf,rd1);
        rd1=con->read(buf,UNIV_BUF_SIZE);
    }
}

bool DistributionThread :: fileAcceptEnabled()
{
afMutex.lock();
return acceptFile;
afMutex.unlock();
}
void DistributionThread :: enableAcceptFile()
{
    afMutex.lock();
    acceptFile=true;
    afMutex.unlock();
}

void Distribution :: incomingConnection(qintptr handle)
{
    cout <<"Connected\n";
    DistributionThread *dt=new DistributionThread(this,handle);
    connectedHosts.push_back(dt);
    if(autoAccept)
        dt->enableAcceptFile();
dt->start();
}
void Distribution :: setProgress(string name,Progress *p)
{
progressMutex.lock();
(*prog)[name]=*p;
progressMutex.unlock();
}
InfoData* Distribution :: findInfoData(string name)
{
infoDataMutex.lock();
map<string,InfoData> :: iterator i1=infoDatas.find(name);
if(i1==infoDatas.end())
{
    infoDataMutex.unlock();
    return NULL;
}
else
{
    InfoData *infoData=&(i1->second);
    infoDataMutex.unlock();
    return infoData;
}
}
int Distribution :: getProgressCount()
{
    int sz=0;
    progressMutex.lock();
    sz=prog->size();
    progressMutex.unlock();
    return sz;
}
const Progress* Distribution :: getProgress(string name)
{
    progressMutex.lock();
 map<string,Progress> :: iterator k=prog->find(name);
    if(k==prog->end())
    {
        progressMutex.unlock();
    return NULL;
    }
    else
    {
        progressMutex.unlock();
        return &(k->second);
    }
}
string* Distribution :: getProgressNames()
{
    int sz=getProgressCount();
    progressMutex.lock();
    string *r=new string[sz];
    map<string,Progress> :: iterator k=prog->begin();
    for(int i=0;k!=prog->end();i++,k++)
    {
        r[i]=k->first;
    }
    progressMutex.unlock();
    return r;
}
void Distribution :: stop()
{
int sz=connectedHosts.size();
for(int i=0;i<sz;i++)
{
    connectedHosts[i]->closeDistribution();
    connectedHosts[i]->deleteLater();
}
connectedHosts.erase(connectedHosts.begin(),connectedHosts.end());
this->close();
}
void DistributionThread :: closeDistribution()
{
con->close();
}
bool Distribution :: loadFromFile(string file)
{
    //char *data=new char[2048];
if(!QFile(file.c_str()).exists())
return false;
/*ifstream in(file.c_str());
while(!in.eof())
{
in.getline(data,2047);
string name=data;
if(in.eof())
    return false;
in.getline(data,2047);
string id1=data;
InfoData *idata=InfoData :: loadFrom(id1);
if(idata==NULL)
    return false;
infoDatas[name]=*idata;
}*/
QVector<QString> data;
QFile iFile(file.c_str());
iFile.open(QIODevice::ReadOnly);
QDataStream dataStream(&iFile);
dataStream >>data;
iFile.close();
int sz=data.size();
for(int i=0;i<sz;i++)
{
      DistSaveData *dsd=DistSaveData::loadFrom(data[i].toStdString());
      (*this->prog)[dsd->p1.name]=dsd->p1;
      this->infoDatas[dsd->p1.name]=dsd->id1;
      delete dsd;
}
return true;
}
void Distribution :: saveToFile(string file)
{
    /*ofstream out(file.c_str());

map<string,InfoData> :: iterator it1=infoDatas.begin();
while(it1!=infoDatas.end())
{
    string name=it1->first;
    InfoData id1=it1->second;
    out <<name.c_str() <<"\n" <<id1.toXml() <<"\n";
    it1++;
}
out.close();*/
    QVector<QString> w;
    map<string,InfoData> :: iterator it1=infoDatas.begin();
    while(it1!=infoDatas.end())
    {
        string name=it1->first;
        DistSaveData dsd;
        dsd.id1=it1->second;
        if(prog->find(name)!=prog->end())
        {
            dsd.p1=prog->find(name)->second;
        }
        else
        dsd.p1.name=name;
        w.push_back(dsd.toXML().c_str());
        it1++;
    }
    QFile oFile(file.c_str());
    oFile.open(QIODevice::WriteOnly);
    QDataStream dataStream(&oFile);
    dataStream <<w;
    oFile.close();
}
int Distribution :: getInfoDataCount()
{
    int ct=0;
    infoDataMutex.lock();
    ct=infoDatas.size();
    infoDataMutex.unlock();
    return ct;
}
string* Distribution :: getInfoDataNames()
{
    string *r=NULL;
    infoDataMutex.lock();
    r=new string[infoDatas.size()];
    map<string,InfoData> :: iterator i1=infoDatas.begin();
    for(int i=0;i1!=infoDatas.end();i++,i1++)
    {
        r[i]=i1->first;
    }
    infoDataMutex.unlock();
    return r;
}

