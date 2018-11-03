#include<fstream>
#include<string>
#include<cstring>
#include"merge.h"
using namespace std;
char* conv(string s)
{
    const char *ch1=s.c_str();
    char *ch=new char[strlen(ch1)+1];
    strcpy(ch,ch1);
    return ch;
}
Merger :: Merger()
{
    file=NULL;
}
void Merger :: setFile(char *file)
{
    this->file=file;
}
Merger :: Merger(char *file)
{
    this->file=file;
}
Merger Merger :: operator+(char *file)
{
appendFile(file);
return *this;
}
void Merger :: createRandomName(char rName[],int len,char *appnd)
{
//randomize();
for(int i=0;i<len;i++)
{
    int n=rand();
    while(n>=10)
        n/=10;
    cout <<n <<endl;
    rName[i]=(char)(n+48);
}
rName[len]='\0';
strcat(rName,appnd);
}
void Merger :: prependFile(char *file)
{
    cout <<file <<" " <<(this->file) <<endl;
    char rn[24];
    createRandomName(rn,20,".dat");
    ofstream out(rn,ios :: out | ios :: binary);
    ifstream in(file,ios :: in | ios :: binary);
    ifstream in1(this->file,ios :: in | ios :: binary);
    addFile(&out,&in);
    addFile(&out,&in1);
    in.close();
    in1.close();
    out.close();
        QFile file1(this->file);
        file1.remove();
        QFile file2(file);
        file2.remove();
        QFile(rn).rename(this->file);
    }
void Merger :: addFile(ofstream *out,ifstream *in)
{
    int r=0;
    char *rd1=new char[BSIZE];
    do
    {
        if(r>0)
        {
out->write(rd1,r);
        }
    in->read(rd1,BSIZE);
    r=in->gcount();
    }
    while(r>0);
in->close();
delete rd1;
}
void Merger :: appendFile(char *file)
{
    ofstream out(this->file,ios :: out | ios :: binary | ios :: app);
    ifstream in(file,ios :: in | ios :: binary);
    addFile(&out,&in);
    in.close();
    out.close();
}
void Merger :: renameTo(char *name)
{
    QFile(file).rename(name);
}
DMerger :: 	DMerger()
{
    this->file=NULL;
}

DMerger :: DMerger(char *file)
{
    this->file=file;
}
void DMerger :: appendData(char *data,int l)
{
ofstream out(file,ios :: out | ios :: binary | ios :: app);
out.write(data,l);
out.close();
}
void DMerger :: prependData(char *data,int l)
{
    char rN[24];
    createRandomName(rN,20,".dat");
    cout <<rN <<endl;
ofstream out(rN,ios :: out | ios :: binary);
out.write(data,l);
ifstream in(file,ios :: in | ios :: binary);
addFile(&out,&in);
in.close();
out.close();
QFile(this->file).remove();
QFile(rN).rename(this->file);
}
Merger operator+(char *file,Merger m)
{
m.prependFile(file);
return m;
}
