#include<QString>
#include<QFile>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QUrl>
#include<iostream>
#include<fstream>
#include<QEventLoop>
#include"downloader.h"
#define DOWNLOAD_BUF_SIZE 1024
using namespace std;

Downloader :: Downloader(QString url):aSync(true),progressReceiver(NULL),onlyHeaders(false)
  ,length(0)
{
    this->url=url;
    progress=0;
    request=new QNetworkRequest(QUrl(url));
}
bool Downloader :: operator <(int p)
{
    return progress<p;
}
bool Downloader :: operator >(int p)
{
    return progress>p;
}
bool Downloader :: operator >=(int p)
{
    return progress>=p;
}
bool Downloader :: operator <=(int p)
{
    return progress<=p;
}
Downloader :: Downloader(QString host,QString path):aSync(true),progressReceiver(NULL),
    onlyHeaders(false),length(0)
{
url="http://"+host+"/"+path;
progress=0;
request=new QNetworkRequest(QUrl(url));
}
bool Downloader :: start(QString downloadFile)
{
    if(!aSync)
    e=new QEventLoop;
url=url.trimmed();
if(url.isEmpty())
    return false;
QNetworkAccessManager *manager=new QNetworkAccessManager(this);
connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
if(onlyHeaders)
    reply=manager->head(*request);
else
{
    dFile.open(downloadFile.toStdString().c_str(),ios :: out | ios :: binary);
reply=manager->get(*request);
}
QObject::connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
if(!aSync)
{
e->exec();
delete e;
delete manager;
}
return true;
}
void Downloader :: onReadyRead()
{
    if(length==0)
        length=reply->header(QNetworkRequest::ContentLengthHeader).toInt();
    qDebug() <<"RR" <<length;
    QByteArray r=reply->read(DOWNLOAD_BUF_SIZE);
    while(!reply->atEnd())
    {
    if(!onlyHeaders)
    dFile.write(r.data(),r.size());
    qDebug() <<r.size();
progress+=r.size()*100/length;
if(progressReceiver)
    (*progressReceiver)(progress);
r=reply->read(DOWNLOAD_BUF_SIZE);
    }
    if(!onlyHeaders)
    dFile.write(r.data(),r.size());
    qDebug() <<r.size();
progress+=r.size()*100/length;
if(progressReceiver)
    (*progressReceiver)(progress);
}
int Downloader :: getLength()
{
return length;
}

void Downloader :: close()
{
    dFile.close();
}
int Downloader :: getProgress()
{
    return progress;
}
void Downloader :: onFinished(QNetworkReply *)
{
    completed=true;
    progress=100;
    qDebug() <<(reply->header(QNetworkRequest::ContentLengthHeader)==QVariant::Invalid);
    length=reply->header(QNetworkRequest::ContentLengthHeader).toInt();
    qDebug() <<length;
    if(!onlyHeaders)
        dFile.close();
    if(!aSync)
    {
    e->exit(0);
    }
    if(progressReceiver)
        (*progressReceiver)(progress);
}
void Downloader :: setHeader(QString name,QString value)
{
request->setRawHeader(QByteArray::fromStdString(name.toStdString()),
                      QByteArray::fromStdString(value.toStdString()));
}
bool Downloader :: isCompleted()
{
    return completed;
}
void Downloader :: setOnlyHeaders(bool hdrs)
{
    onlyHeaders=hdrs;
    setAsync(false);
}
void Downloader :: setAsync(bool aSync)
{
    this->aSync=aSync;
}
void Downloader :: setProgressReceiver(ProgressReceiver *progressReceiver)
{
    this->progressReceiver=progressReceiver;
}

void Downloader :: setFormData(QString frmData)
{

}
DownloaderThread :: DownloaderThread(Downloader *d,QString file):d(d)
{
this->file=file;
}

void DownloaderThread :: run()
{
d->start(file);
}
