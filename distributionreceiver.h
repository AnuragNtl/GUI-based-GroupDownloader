#ifndef DISTRIBUTIONRECEIVER
#define DISTRIBUTIONRECEIVER
#include"infodata.h"
class DistributionReceiver
{
public:
    virtual void onReceiveProgress(Progress p)=0;
    virtual void onClientDisconnect(Progress p)=0;
    virtual void onReadFrom(Progress,string)=0;
};

#endif // DISTRIBUTIONRECEIVER

