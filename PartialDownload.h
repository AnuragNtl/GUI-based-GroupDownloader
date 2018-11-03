#include<string>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<fstream>
#ifndef INFO_DATA
#include"Range.h"
#endif
#include<QThread>
#include"downloaderthread.h"
using namespace std;
int getContentLength(string host,string path);

class Downloader
{
protected:
	string host,path;
	string file;
	string hdrs,frm,rtype;
	int flen,downloaded;
	DWORD thrdId;
	int rcode;
	int connected;
	string *data;
	int downloadToData;
	int completd;
	void setHostAndPath(string url);
public:
		static const int BUF_SIZE=1024;
	Downloader()
	{
	downloadToData=0;
	completd=0;
	flen=-1;
	downloaded=0;
	}
	Downloader(string url);
	Downloader(string host,string path);
	void setUrl(string url);
	void setHost(string host);
	void setPath(string path);
	int startDownload(string file);
	int startDownload(string *data);
	void setHeaders(string hdrs);
	void setFormData(string frm);
	void setRequestType(string rtype);
	int getResponseCode();
	int getProgress();
	int getLength()
	{
		return flen;
	}
	int isConnected()
	{
		return connected;
	}
	int operator<(int pr)
	{
		return (getProgress()<pr);
	}
	int operator>(int pr)
	{
		return (getProgress()>pr);
	}
	int isCompleted()
	{
		return completd;
	}
	string getHost()
	{
		return host;
	}
	string getPath()
	{
		return path;
	}
        friend class DownloadThread;
};
Downloader :: Downloader(string url)
{
	flen=-1;
	downloaded=0;
	downloadToData=0;
	completd=0;
	setHostAndPath(url);
}
Downloader :: Downloader(string host,string path)
{
	flen=-1;
	downloaded=0;
	downloadToData=0;
	completd=0;
	this->host=host;
	this->path=path;
}
void Downloader :: setUrl(string url)
{
	setHostAndPath(url);
}
void Downloader :: setHost(string h)
{
host=h;
}
void Downloader :: setPath(string p)
{
	path=p;
}
void Downloader :: setRequestType(string r)
{
	rtype=r;
}
void Downloader :: setHeaders(string h)
{
	hdrs=h;
}
void Downloader :: setFormData(string f)
{
	this->frm=f;
}
void Downloader :: setHostAndPath(string url)
{
	int l1=url.size();
	if(l1<=7)
		return;
			string s1;
			s1.insert(0,url,0,7);
			if(s1!="http://")
				return;
			string s4="";
			int hostset=0; 
			for(int i=7;i<l1;i++)
			{
				if((url[i]=='/') && !hostset)
				{
					host=s4;
					s4="";
					hostset=1;
				}
				else
				{
					s4=s4+url[i];
				}
			}
			if(hostset)
			path="/"+s4;
}
int Downloader :: startDownload(string file)
{
        DownloadThread dThrd(this);
        dThrd.start();
}


int Downloader :: getResponseCode()
{
	return rcode;
}
int Downloader :: getProgress()
{
	return (downloaded*100)/flen;
}
class PartialDownloader : public Downloader
{
protected:
	struct RangeS *r;
public:
	PartialDownloader(Range *r);
	PartialDownloader(string url);
	PartialDownloader(string host,string path) : Downloader(host,path)
	{	
		flen=-1;
	downloaded=0;
	}
	void setRange(Range *r)
	{
		this->r=r;
	}
	int startDownload(string file);
	int startDownload(string *data);
};
PartialDownloader :: PartialDownloader(Range *r)
{
this->r=r;
	flen=-1;
	downloaded=0;
}
PartialDownloader :: PartialDownloader(string url) : Downloader(url){}
int getContentLength(string host,string path,string hdrs,string fdata)
{
/*int cnttLen;
WSAData wd1;
SOCKET sc1=INVALID_SOCKET;
struct addrinfo *r=NULL,*p=NULL,hints;
int ir1=WSAStartup(MAKEWORD(2,2),&wd1);
if(ir1!=0)
	return -1;
ZeroMemory(&hints,sizeof(hints));
hints.ai_family=AF_UNSPEC;
hints.ai_socktype=SOCK_STREAM;
hints.ai_protocol=IPPROTO_TCP;
ir1=getaddrinfo(host.c_str(),"80",&hints,&r);
for(p=r;p!=NULL;p=p->ai_next)
{
	sc1=socket(p->ai_family,p->ai_socktype,p->ai_protocol);
	if(sc1==INVALID_SOCKET)
	{
		WSACleanup();
		return -1;
	}
	ir1=connect(sc1,p->ai_addr,(int)p->ai_addrlen);
	if(ir1==SOCKET_ERROR)
	{
		closesocket(sc1);
		sc1=INVALID_SOCKET;
		continue;
	}
	break;
}
if(sc1==INVALID_SOCKET)
{
	WSACleanup();
	return -1;
}
string sendHd="HEAD "+path+" HTTP/1.1\r\n"+hdrs+"\r\n\r\n"+fdata;
if(send(sc1,sendHd.c_str(),sendHd.size(),0)==SOCKET_ERROR)
{
	closesocket(sc1);
	WSACleanup();
	return -1;
}
char *rd1=new char[UNIV_BUF_SIZE+1];
ir1=recv(sc1,rd1,UNIV_BUF_SIZE,0);
if(ir1>0)
{
	rd1[ir1]='\0';
	string hdrsData=rd1;
	string cnttLenHdr="Content-Length: ";
	int hdrFind=hdrsData.find(cnttLenHdr);
	if(hdrFind>=0)
	{
		string hData;
		int lt=hdrsData.find("\r\n",hdrFind+cnttLenHdr.size());
		hData.insert(0,hdrsData,hdrFind+cnttLenHdr.size(),lt-hdrFind-cnttLenHdr.size());
		return atoi(hData.c_str());
	}
	else
	{
		closesocket(sc1);
		WSACleanup();
		return -1;
	}
}
else
{
closesocket(sc1);
WSACleanup();
return -1;
}*/
    return 0;
}
int PartialDownloader :: startDownload(string file)
{
	char st[10],e[10];
	ltoa(r->start,st,10);
	ltoa(r->end,e,10);
	setHeaders(string("Range: bytes=")+st+string("-")+e);
		completd=0;
	this->file=file;
//	HANDLE h1=CreateThread(NULL,0,start,(LPVOID)(this),0,&thrdId);
        return 0;//(h1?1:0);
}
int PartialDownloader :: startDownload(string *data)
{
		char st[10],e[10];
	ltoa(r->start,st,10);
	ltoa(r->end,e,10);
	setHeaders(string("Range: bytes=")+st+string("-")+e);
		completd=0;
	this->data=data;
	downloadToData=1;
//	HANDLE h1=CreateThread(NULL,0,start,(LPVOID)(this),0,&thrdId);
        return 0;//(h1?1:0);
}

void DownloadThread :: run()
{
//Q
}
