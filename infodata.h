#ifndef INFO_DATA
#define INFO_DATA
#include<string>
#include"Range.h"
#include<map>
#include<iostream>
#include<QtXml/QXmlInputSource>
#include<QtXml/QXmlSimpleReader>
#include<QtXml/QXmlDefaultHandler>
#include<QXmlStreamWriter>
using namespace std;
#define UNIV_BUF_SIZE 1024
#define DISTRIBUTION_LISTEN_PORT 9980
class MapFromSimpleXML : public QXmlDefaultHandler
{
private:
    map<string,string> *data;
    string toParse;
    string curElem;
    QIODevice *input;
    bool documentRead;
public:
    bool startElement(const QString& namespaceURI,const QString &localName,const QString &qName,const QXmlAttributes &atts);
    MapFromSimpleXML(string xml);
    MapFromSimpleXML(QIODevice *d);
    bool parse();
    bool parse(bool incr);
    map<string,string>* getData();
    static map<string,string>* getData(string xml);
    static map<string,string>* getData(QIODevice *d);
    bool characters(const QString &);
    bool endDocument();
};
struct InfoData
{
    InfoData();
string host,path,rtype,hdrs,fdata;
Range r;
string downloadId;
int start,stop;
int sendMergeData;
friend std::ostream& operator <<(std::ostream&,InfoData &id);
static InfoData* loadFrom(string src);
static InfoData* loadFrom(QIODevice* d);
string toXml();
private:
static InfoData* loadFrom(map<string,string>*);
friend class DistSaveData;
};
std::ostream& operator <<(std::ostream&,InfoData &id);
struct Progress
{
    Progress();
	string host,name;
short started;
short finished;
long progress;
short cancelled;
short error;
friend ostream& operator <<(ostream& out,Progress &p);
static Progress* loadFrom(string);
static Progress* loadFrom(QIODevice*);
string toXml();
operator string();
private:
static Progress* loadFrom(map<string,string>*);
friend class DistSaveData;
};
struct ClientInfo
{
string name,host,id;
friend ostream& operator <<(ostream&,ClientInfo&);
static ClientInfo* loadFrom(string);
static ClientInfo* loadFrom(QIODevice*);
string toXml();
private:
static ClientInfo* loadFrom(map<string,string>*);
};
struct DistSaveData
{
    InfoData id1;
    Progress p1;
    DistSaveData();
    operator string();
    string toXML();
    static DistSaveData* loadFrom(string);
    ~DistSaveData();
};

class MapToSimpleXML
{
private:
    map<string,string> &k;
public:
    MapToSimpleXML(map<string,string>&);
    string convert(string);
    static string convertFromMap(string,map<string,string>);
};

#endif
