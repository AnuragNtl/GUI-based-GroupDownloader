#ifndef DISTRIBUTION
#define DISTRIBUTION
#include<QObject>
#include<QTcpServer>
#include<map>
#include"infodata.h"
#include"distributionreceiver.h"
#include<QThread>
#include<QTcpSocket>
#include<QMutex>
#include<fstream>
#include<QEventLoop>
using namespace std;
class DistributionThread;
class Distribution : public QTcpServer
{
    Q_OBJECT
public:
    enum STATUS{DISCONNECTED,CONNECTED,RECEIVING_CLIENT_INFO,SEND_DOWNLOAD_DATA,DOWNLOADING,UPLOADING};
private:
    map<string,InfoData> infoDatas;
    DistributionReceiver *r;
    map<string,Progress> *prog;
    QMutex progressMutex;
    QMutex infoDataMutex;
    QMutex connectedListMutex;
    QMutex statusMutex;
    map<string,bool> connectedList;
    map<string,STATUS> status;
    QMutex uploadProgressMutex;
    map<string,int> uploadProgress;
    bool autoAccept;
    void setUploadProgress(string,int);
    string outputFileName;
    vector<DistributionThread*> connectedHosts;
public:
    Distribution();
    Distribution(string,InfoData);
    void addInfoData(string,InfoData);
    ~Distribution();
    void start();
    void stop();
    void setReceiver(DistributionReceiver *r);
    void incomingConnection(qintptr handle);
    void setProgress(string,Progress*);
    string* getInfoDataNames();
    int getInfoDataCount();
    InfoData* findInfoData(string name);
    const Progress* getProgress(string);
    string* getProgressNames();
    int getProgressCount();
    void setAutoAccept(bool);
    bool isConnected(string);
    void setConnected(string,bool);
    void setStatus(string,STATUS);
    STATUS getStatus(string);
    int getUploadProgress(string);
    void mergeDownloads();
    friend class DistributionThread;
    friend class MergerThread;
    bool loadFromFile(string file);
    void saveToFile(string file);
signals:
    void mergeCompleted();
void mergeProgress(int);
};
class DistributionThread : public QThread
{
Q_OBJECT
private:
    QMutex afMutex;
    Distribution *d;
    InfoData *curData;
    qintptr handle;
    QTcpSocket *con;
    void sendInfoData();
    bool acceptFile;
    bool closed;
    QEventLoop *e1;
    ofstream oFile;
    int sz;
    int recvd;
    string name;
    bool fileAcceptEnabled();
    protected:
        void run();
public:
    DistributionThread(Distribution*,qintptr handle);
    friend class Distribution;
    void enableAcceptFile();
    void closeDistribution();
public slots:
    void receiveFile();
    void onAboutToClose();
};

class MergerThread : public QThread
{
    Q_OBJECT
private:
    Distribution *d;
public:
    MergerThread(Distribution *d);
    void cancelMerge();
    void run();
};

#endif // DISTRIBUTION
