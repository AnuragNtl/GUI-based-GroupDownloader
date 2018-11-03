#ifndef DOWNLOADER
#define DOWNLOADER
#include<QString>
#include<QFile>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QUrl>
#include<QEventLoop>
#include<iostream>
#include<fstream>
#include<QThread>
#define DOWNLOAD_BUF_SIZE 1024
using namespace std;
class ProgressReceiver;
class Downloader : public QObject
{
    Q_OBJECT
protected:
    QEventLoop *e;
    QString url;
    int progress;
    char buf[DOWNLOAD_BUF_SIZE];
    ofstream dFile;
    QNetworkReply *reply;
    QNetworkRequest *request;
    bool onlyHeaders,completed;
    int length;
    bool aSync;
    ProgressReceiver *progressReceiver;
public:
    Downloader(QString);
    Downloader(QString host,QString path);
    void setHeader(QString name,QString value);
    void setFormData(QString frmData);
    bool start(QString downloadFile="");
    int getProgress();
    void stop();
    bool operator <(int);
    bool operator >(int);
    bool operator <=(int);
    bool operator >=(int);
    void close();
    void setOnlyHeaders(bool);
    int getLength();
    bool isCompleted();
    void setAsync(bool);
    void setProgressReceiver(ProgressReceiver*);
private slots:
    void onReadyRead();
    void onFinished(QNetworkReply*);
};
class ProgressReceiver
{
protected:
    Downloader *d;
public:
    ProgressReceiver(Downloader *d)
    {
        this->d=d;
    }
    virtual void operator ()(int progress){}
};

class DownloaderThread : public QThread
{
    Q_OBJECT
private:
    Downloader *d;
    QString file;
public:
    DownloaderThread(Downloader *d,QString file);
    void run();
};

#endif // DOWNLOADER

