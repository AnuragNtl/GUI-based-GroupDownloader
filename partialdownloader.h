#ifndef PARTIALDOWNLOADER_H
#define PARTIALDOWNLOADER_H

#include <QObject>
#include"downloader.h"
struct Range;
class PartialDownloader : public Downloader
{
    Q_OBJECT
public:
    explicit PartialDownloader(QString);
    PartialDownloader(QString,QString);
    ~PartialDownloader();
    void setRange(Range *r);
public slots:
};

#endif // PARTIALDOWNLOADER_H
