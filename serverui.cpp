#include "serverui.h"
#include "ui_serverui.h"
#include"distribution.h"
#include"addclient.h"
#include<QPushButton>
#include<QMessageBox>
#include<QUrl>
#include"downloader.h"
#include"dividerange.h"
ServerUI::ServerUI(QWidget *parent,Distribution *d) :
    QDialog(parent),serverStarted(false),
    changeMade(false),
    ui(new Ui::ServerUI),dist(d)
{
    updater=new ServerUIUpdater;
    ui->setupUi(this);
    ui->listWidget->setViewMode(QListView::ListMode);
    connect(ui->pushButton,&QPushButton::clicked,this,&ServerUI::onStart);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&ServerUI::onAddClientStart);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&ServerUI::onDelClient);
    connect(updater,&ServerUIUpdater::updateProgress,this,&ServerUI::onUpdate);
    connect(ui->pushButton_4,&QPushButton :: clicked,this,&ServerUI :: onRangeDivisionShow);
    connect(ui->plainTextEdit,&QPlainTextEdit :: textChanged,this,&ServerUI :: dataChanged);
    connect(ui->plainTextEdit_2,&QPlainTextEdit :: textChanged,this,&ServerUI :: dataChanged);
    connect(ui->plainTextEdit_3,&QPlainTextEdit :: textChanged,this,&ServerUI :: dataChanged);
    if(loadDistData())
    {
        ui->listWidget->clear();
        string *iNames=dist->getInfoDataNames();
        int ct=dist->getInfoDataCount();
        for(int i=0;i<ct;i++)
        {
            ranges[iNames[i]]=dist->findInfoData(iNames[i])->r;
            ui->listWidget->addItem(iNames[i].c_str());
        }
        onStart();
    }
}
void ServerUI :: dataChanged()
{
    changeMade=true;
}

void ServerUI :: onRangeDivisionShow()
{
    if(changeMade || ranges.size()==0)
    {
        changeMade=false;
    if((ui->plainTextEdit->toPlainText().size()==0) ||
    (ui->plainTextEdit_2->toPlainText().size()==0) ||
    (ui->plainTextEdit_3->toPlainText().size()==0) || ui->listWidget->count()==0)
    {
        QMessageBox *b=new QMessageBox(QMessageBox::NoIcon,"Group Downloader","Please Input Required Fields");
        b->show();
        return;
    }
    QString dn=ui->plainTextEdit->toPlainText(),url=ui->plainTextEdit_2->toPlainText();
    QString hdrs=ui->plainTextEdit_3->toPlainText();
    int len=ui->listWidget->count();
    Downloader d(url);
    d.setOnlyHeaders(true);
    d.start();
    int contentLength=d.getLength();
    qDebug() <<contentLength;
    vector<string> names;
    for(int i=0;i<len;i++)
    {
        int start=(int)(i*(double)contentLength/len);
        int end=0;
        if(i<len-1)
        end=(int)((i+1)*(double)contentLength/len)-1;
        else
           end=contentLength-1;
        string n=ui->listWidget->item(i)->text().toStdString();
        Range r;
        r.start=start;
            r.end=end;
        ranges[n]=r;
        names.push_back(n);
    }
    DivideRange *d1=new DivideRange(names,&ranges,contentLength,this);
    d1->show();
    }
    else
    {
        changeMade=false;
        vector<string> names;
int len=ui->listWidget->count();
for(int i=0;i<len;i++)
{
    string n=ui->listWidget->item(i)->text().toStdString();
names.push_back(n);
}
DivideRange *d1=new DivideRange(names,&ranges,0,this);
d1->show();
}
}

void ServerUI :: onUpdate()
{
    qDebug() <<"OnUpdate";
    int pk=dist->getProgressCount();
    qDebug() <<"Count " <<pk;
    string *pNames=dist->getProgressNames();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(pk);
    for(int i=0;i<pk;i++)
    {
        qDebug() <<pNames[i].c_str();
        const Progress *p1=dist->getProgress(pNames[i]);
        bool connected=dist->isConnected(pNames[i]);
        Distribution::STATUS status=dist->getStatus(pNames[i]);
        Progress p;
        if(p1==NULL)
           continue;
        p=*p1;
        ui->tableWidget->insertRow(i);
        QTableWidgetItem *items=new QTableWidgetItem[6];
        items[0].setText(p.name.c_str());
        int uploadProgress=0;
        if(status==Distribution::UPLOADING)
        {
            uploadProgress=dist->getUploadProgress(pNames[i]);
            items[1].setText(QString :: number(uploadProgress));
        }
            else
        items[1].setText(QString :: number(p.progress));
        qDebug() <<"Progress " <<p.progress;
        items[2].setText(connected?"Yes":"No");
        items[3].setText(status==Distribution::DISCONNECTED?"DISCONNECTED":status==Distribution::CONNECTED?"CONNECTED":status==Distribution::DOWNLOADING?"DOWNLOADING":status==Distribution::RECEIVING_CLIENT_INFO?"RECEIVING CLIENT INFO":status==Distribution::UPLOADING?"UPLOADING":"SEND DOWNLOAD DATA");
        items[4].setText(QString :: number(ranges[pNames[i]].start)+"-"+QString :: number(ranges[pNames[i]].end));
        items[5].setText(status==Distribution::UPLOADING && uploadProgress==100?"Yes":"No");
        for(int k=0;k<6;k++)
        {
            items[k].setFlags(Qt::NoItemFlags);
        ui->tableWidget->setItem(i,k,&items[k]);
        }
    }
    qDebug() <<"___________";
}

void ServerUI :: onAddClient()
{
    qDebug() <<curClient.c_str() <<endl;
    changeMade=true;
    ui->listWidget->addItem(new QListWidgetItem(curClient.c_str(),ui->listWidget));
}
void ServerUI :: onAddClientStart()
{
    AddClient *k=new AddClient(this);
    k->show();
}
void ServerUI :: onDelClient()
{
    changeMade=true;
    QList<QListWidgetItem*> selected=ui->listWidget->selectedItems();
    if(selected.isEmpty())
        return;
    delete selected.first();
}
void ServerUI :: onStart()
{
    if(serverStarted)
    {
        serverStarted=false;
        ui->pushButton->setText("Start Server");
        dist->stop();
        return;
    }
    else
    {
        ui->pushButton->setText("Stop Server");
        serverStarted=true;
    }
    if((ui->plainTextEdit->toPlainText().size()==0) ||
    (ui->plainTextEdit_2->toPlainText().size()==0) ||
    (ui->plainTextEdit_3->toPlainText().size()==0))
    {
        QMessageBox *b=new QMessageBox(QMessageBox::NoIcon,"Group Downloader","Please Input Required Fields");
        b->show();
        return;
    }
    if(dist->getInfoDataCount()==0)
    {
    QString dn=ui->plainTextEdit->toPlainText(),url=ui->plainTextEdit_2->toPlainText();
    QString hdrs=ui->plainTextEdit_3->toPlainText();
    int len=ui->listWidget->count();
    Downloader d(url);
    d.setOnlyHeaders(true);
    d.start();
    int contentLength=d.getLength();
    qDebug() <<contentLength;
    for(int i=0;i<len;i++)
    {
        InfoData info;
        info.hdrs=hdrs.toStdString();
        info.downloadId=dn.toStdString();
        QUrl s(url);
        info.host=s.host().toStdString();
        info.path=s.path().toStdString();
        info.start=1;
        info.stop=0;
        info.sendMergeData=0;
        info.rtype="GET";
        info.r.start=(int)(i*(double)contentLength/len);
        qDebug() <<(info.r.start);
        if(i<len-1)
        info.r.end=(int)((i+1)*(double)contentLength/len)-1;
        else
            info.r.end=contentLength-1;
        string n=ui->listWidget->item(i)->text().toStdString();
        ranges[n]=info.r;
        dist->addInfoData(n,info);
    }
    }
    qDebug() <<"__\n";
    saveDistData();
    dist->start();
    updater->start();
    qDebug() <<"______________\n";
}

ServerUI::~ServerUI()
{
    delete ui;
}

void ServerUIUpdater :: run()
{
    while(1)
    {
        QThread::msleep(2000);
        emit updateProgress();
    }
}
void ServerUI :: saveDistData()
{
    dist->saveToFile("DistSaveData.dat");
}
bool ServerUI :: loadDistData()
{
return dist->loadFromFile("DistSaveData.dat");
}

