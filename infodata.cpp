#include"infodata.h"
#include<QDebug>

map<string,string>* MapFromSimpleXML :: getData()
{
    return data;
}
MapFromSimpleXML :: MapFromSimpleXML(string xml):input(NULL),documentRead(false)
{
    this->toParse=xml;
    data=new map<string,string>;
}
MapFromSimpleXML :: MapFromSimpleXML(QIODevice *d):input(d),documentRead(false)
{
    data=new map<string,string>;
}
map<string,string>* MapFromSimpleXML :: getData(string xml)
{
    MapFromSimpleXML mfsx(xml);
    if(mfsx.parse())
        return mfsx.getData();
    else
        return NULL;
}
map<string,string>* MapFromSimpleXML :: getData(QIODevice *d)
{
MapFromSimpleXML mfsx(d);
if(mfsx.parse(true))
    return mfsx.getData();
else
    return NULL;
}

bool MapFromSimpleXML :: parse()
{
    bool r=false;
    QXmlSimpleReader *xr=new QXmlSimpleReader;
    QXmlInputSource *src;
    if(input!=NULL)
        src=new QXmlInputSource(input);
    else
    {
        src=new QXmlInputSource;
    src->setData(QString(toParse.c_str()));
    }
    xr->setContentHandler(this);
    r=xr->parse(src,true);
    delete src;
    return r;
}
bool MapFromSimpleXML :: parse(bool incr)
{
    if(input==NULL)
        return false;
    if(!incr)
        return parse();
    else
    {
        if(input->bytesAvailable()<=0)
        input->waitForReadyRead(-1);
        QXmlStreamReader xr(input);
        xr.readNext();
        do
        {
            if(xr.isStartElement())
            {
                curElem=xr.name().toString().toStdString();
                xr.readNext();
            }
            else if(xr.isCharacters())
            {
                string s2=xr.text().toString().toStdString();
                data->insert(pair<string,string>(curElem,s2));
                xr.readNext();
            }
            else if(xr.isEndElement())
            {
                xr.readNext();
            }
            else if(xr.hasError())
            {
               QXmlStreamReader::Error e1;
                       e1=xr.error();
                       if(e1!=QXmlStreamReader::PrematureEndOfDocumentError)
                           break;
                       else
                       {
                           if(input->bytesAvailable()<=0)
                           input->waitForReadyRead(-1);
                           xr.readNext();
                       }
            }
            else
            {
                xr.readNext();
            }
        }
        while(!xr.atEnd() || xr.error()==QXmlStreamReader::PrematureEndOfDocumentError);
        QXmlStreamReader :: Error e1=xr.error();
       return (!(xr.hasError()));
    }
}

bool MapFromSimpleXML :: characters(const QString &s)
{
    if(curElem.size()==0)
        return false;
 data->insert(pair<string,string>(curElem,s.toStdString()));
         return true;
}
bool MapFromSimpleXML :: startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts)
{
curElem=localName.toStdString();
return true;
}
bool MapFromSimpleXML :: endDocument()
{
return true;
}
InfoData :: InfoData() : start(0),stop(0),sendMergeData(0)
{
    downloadId="00";
    fdata="formData";
    hdrs="______";
    host="______";
    path="______";
    r.start=0;
    r.end=0;
    rtype="GET";
}
Progress :: Progress() : started(0),finished(0),error(0),cancelled(0)
{
this->host="HOST";
    this->name="NAME";
    this->progress=0;
}

string InfoData :: toXml()
{
QString s;
QXmlStreamWriter sw(&s);
sw.writeStartDocument();
sw.writeStartElement("InfoData");
sw.writeStartElement("downloadId");
sw.writeCharacters(QString(downloadId.c_str()));
sw.writeEndElement();
sw.writeStartElement("host");
sw.writeCharacters(QString(host.c_str()));
sw.writeEndElement();
sw.writeStartElement("path");
sw.writeCharacters(QString(path.c_str()));
sw.writeEndElement();
sw.writeStartElement("reqType");
sw.writeCharacters(QString(rtype.c_str()));
sw.writeEndElement();
sw.writeStartElement("headers");
sw.writeCharacters(QString(hdrs.c_str()));
sw.writeEndElement();
sw.writeStartElement("formData");
sw.writeCharacters(QString(fdata.c_str()));
sw.writeEndElement();
sw.writeStartElement("rangeStart");
sw.writeCharacters(QString :: number(r.start));
sw.writeEndElement();
sw.writeStartElement("rangeEnd");
sw.writeCharacters(QString :: number(r.end));
sw.writeEndElement();
sw.writeStartElement("state");
int state=((start<<2)&4)|((stop<<1)&2)|(sendMergeData&1);
sw.writeCharacters(QString::number(state));
sw.writeEndElement();
sw.writeEndElement();
sw.writeEndDocument();
return s.toStdString();
}

InfoData* InfoData :: loadFrom(string src)
{
    map<string,string> *data=MapFromSimpleXML :: getData(src);
    if(data==NULL)
        return NULL;
    else
    return loadFrom(data);
}
InfoData* InfoData :: loadFrom(QIODevice *d)
{
map<string,string> *data=MapFromSimpleXML :: getData(d);
if(data==NULL)
    return NULL;
else
return loadFrom(data);
}

InfoData* InfoData :: loadFrom(map<string, string> *data)
{
    InfoData *r=new InfoData;
    map<string,string> :: iterator it1;
    it1=data->find("downloadId");
    if(it1==data->end())
        return NULL;
    r->downloadId=it1->second;
    it1=data->find("host");
    if(it1==data->end())
        return NULL;
    r->host=it1->second;
    it1=data->find("path");
    if(it1==data->end())
        return NULL;
    r->path=it1->second;
    it1=data->find("reqType");
    if(it1==data->end())
        return NULL;
    r->rtype=it1->second;
    it1=data->find("headers");
    if(it1==data->end())
        return NULL;
    r->hdrs=it1->second;
    it1=data->find("formData");
    if(it1==data->end())
        return NULL;
    r->fdata=it1->second;
    it1=data->find("rangeStart");
    if(it1==data->end())
        return NULL;
    (r->r).start=QString(it1->second.c_str()).toInt();
    it1=data->find("rangeEnd");
    if(it1==data->end())
        return NULL;
    (r->r).end=QString(it1->second.c_str()).toInt();
    it1=data->find("state");
    if(it1==data->end())
        return NULL;
    int state=QString((it1->second).c_str()).toInt();
    r->start=(4&state)>0;
    r->stop=(2&state)>0;
    r->sendMergeData=(1&state)>0;
    return r;
}

std::ostream& operator <<(std::ostream &out,InfoData &id)
{
out <<"Host=" <<id.host <<endl;
out <<"ReqType=" <<id.rtype <<endl;
out <<"Hdrs=" <<id.hdrs <<endl;
out <<"FormData=" <<id.fdata <<endl;
out <<"Range=" <<id.r.start <<"-" <<id.r.end <<endl;
out <<"DownloadId=" <<id.downloadId <<endl;
out <<"Flags(Start,Stop,SendMergeData) " <<id.start <<id.stop <<id.sendMergeData <<endl;
return out;
}
Progress* Progress :: loadFrom(string xml)
{
    map<string,string> *pData=MapFromSimpleXML :: getData(xml);
    if(pData==NULL)
        return NULL;
    else
    return loadFrom(pData);
}
Progress* Progress :: loadFrom(QIODevice *d)
{
    map<string,string> *pData=MapFromSimpleXML :: getData(d);
    if(pData==NULL)
        return NULL;
    else
    return loadFrom(pData);
}

Progress* Progress :: loadFrom(map<string,string> *pData)
{
    Progress *r=new Progress;
    map<string,string> :: iterator f=pData->find("host");
    if(f==pData->end())
        return NULL;
    r->host=f->second;
    f=pData->find("name");
    if(f==pData->end())
        return NULL;
    r->name=f->second;
    f=pData->find("prog");
    if(f==pData->end())
        return NULL;
    r->progress=QString(f->second.c_str()).toInt();
    f=pData->find("state");
    if(f==pData->end())
        return NULL;
    int state=QString(f->second.c_str()).toInt();
    r->started=(8&state)>0;
    r->finished=(4&state)>0;
    r->cancelled=(2&state)>0;
    r->error=(1&state)>0;
    return r;
}
string Progress :: toXml()
{
QString r="";
QXmlStreamWriter w1(&r);
w1.writeStartDocument();
QString tData[]={"host","name","prog","state"};
int state=((started<<3)&8)|((finished<<2)&4)|((cancelled<<1)&2)|(error&1);
QString tCntt[4]={QString(host.c_str()),QString(name.c_str()),QString :: number(progress),
                  QString :: number(state)};
w1.writeStartElement("Progress");
for(int i=0;i<4;i++)
{
w1.writeStartElement(tData[i]);
w1.writeCharacters(tCntt[i]);
w1.writeEndElement();
}
w1.writeEndElement();
w1.writeEndDocument();
return r.toStdString();
}

ostream& operator <<(ostream& out,Progress &p)
{
    out <<"Host " <<p.host <<endl;
    out <<"Name " <<p.name <<endl;
    out <<"Progress " <<p.progress <<endl;
    out <<"Started " <<p.started <<endl;
    out <<"Finished " <<p.finished <<endl;
    out <<"Cancelled " <<p.cancelled <<endl;
    out <<"Error " <<p.error <<endl;
    return out;
}
ostream& operator <<(ostream &out,ClientInfo &k)
{
out <<"Host " <<k.host <<endl;
out <<"Name " <<k.name <<endl;
out <<"Id " <<k.id <<endl;
return out;
}
ClientInfo* ClientInfo :: loadFrom(map<string,string> *data)
{
ClientInfo *r=new ClientInfo;
map<string,string> :: iterator it1;
it1=data->find("host");
if(it1==data->end())
    return NULL;
r->host=it1->second;
it1=data->find("name");
if(it1==data->end())
    return NULL;
r->name=it1->second;
it1=data->find("id");
if(it1==data->end())
    return NULL;
r->id=it1->second;
return r;
}
ClientInfo* ClientInfo :: loadFrom(string src)
{
map<string,string> *data=MapFromSimpleXML :: getData(src);
if(data==NULL)
    return NULL;
else
return loadFrom(data);
}
ClientInfo* ClientInfo :: loadFrom(QIODevice *d)
{
map<string,string> *data=MapFromSimpleXML :: getData(d);
if(data==NULL)
    return NULL;
else
return loadFrom(data);
}
string ClientInfo :: toXml()
{
QString r="";
QXmlStreamWriter w1(&r);
w1.writeStartDocument();
QString n[]={"host","name","id"};
w1.writeStartElement("ClientInfo");
string p[]={host,name,id};
for(int i=0;i<3;i++)
{
    w1.writeStartElement(n[i]);
    w1.writeCharacters(QString(p[i].c_str()));
    w1.writeEndElement();
}
w1.writeEndElement();
w1.writeEndDocument();
return r.toStdString();
}
Progress :: operator string()
{
    return toXml();
}
DistSaveData :: DistSaveData(){}
DistSaveData :: ~DistSaveData(){}
DistSaveData* DistSaveData :: loadFrom(string s)
{
map<string,string> *data=MapFromSimpleXML::getData(s);
InfoData *id1=InfoData::loadFrom(data);
(*data)["host"]=(*data)["prog_host"];
(*data)["status"]=(*data)["prog_status"];
Progress *p1=Progress::loadFrom(data);
DistSaveData *r=new DistSaveData;
if(id1!=NULL && p1!=NULL)
{
r->id1=*id1;
r->p1=*p1;
}
delete data;
qDebug() <<(r->toXML().c_str());
return r;
}

string DistSaveData :: toXML()
{
    string infoDataS=id1.toXml();
    string progS=p1.toXml();
    qDebug() <<infoDataS.c_str();
    qDebug() <<progS.c_str();
    map<string,string> *data1=MapFromSimpleXML::getData(infoDataS);
    map<string,string> *data2=MapFromSimpleXML::getData(progS);
    for(map<string,string> :: iterator it1=data2->begin();it1!=data2->end();it1++)
        if(data1->find(it1->first)==data1->end())
        (*data1)[it1->first]=(*data2)[it1->first];
    else
            (*data1)["prog_"+it1->first]=(*data2)[it1->first];
    string r=MapToSimpleXML::convertFromMap("DistData",*data1);
    delete data1;
    delete data2;
    qDebug() <<r.c_str();
    return r;
}
MapToSimpleXML :: MapToSimpleXML(map<string,string> &k) : k(k){}
string MapToSimpleXML :: convert(string name)
{
    QString r="";
QXmlStreamWriter w1(&r);
w1.writeStartDocument();
w1.writeStartElement(name.c_str());
for(map<string,string> :: iterator it1=k.begin();it1!=k.end();it1++)
{

    w1.writeStartElement(it1->first.c_str());
    w1.writeCharacters(it1->second.c_str());
    w1.writeEndElement();
}
w1.writeEndElement();
    w1.writeEndDocument();
    return r.toStdString();
}
string MapToSimpleXML :: convertFromMap(string name,map<string,string> k)
{
    MapToSimpleXML mfsx(k);
    return mfsx.convert(name);
}
