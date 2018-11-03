#include "partialdownloader.h"
#include"Range.h"
#include<QString>
using namespace std;
PartialDownloader :: PartialDownloader(QString s) : Downloader(s)
{

}
PartialDownloader :: PartialDownloader(QString host,QString path) : Downloader(host,path)
{

}

PartialDownloader::~PartialDownloader()
{

}

void PartialDownloader :: setRange(Range *r)
{
    setHeader("Range",QString("bytes=")+QString::number(r->start)+"-"+QString::number(r->end));
}
