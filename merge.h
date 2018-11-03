#ifndef MERGER
#define MERGER
#include<iostream>
#include<fstream>
#include<cmath>
#include<cstring>
#include<cstdlib>
#include<QFile>
#define BSIZE 1024
using namespace std;

class Merger
{
protected:
	char *file;
	void createRandomName(char rName[],int len,char *appnd);
void addFile(ofstream *out,ifstream *in);
public:
Merger();
	Merger(char *file);
    void setFile(char *file);
    void renameTo(char *name);
	void prependFile(char *file);
	void appendFile(char *file);
	Merger operator+(char *file);
	char* getFile();
};
class DMerger : public Merger
{
public:
    DMerger();
	DMerger(char *file);
	void appendData(char *data,int l);
	void prependData(char *data,int l);
};
char* conv(string);
#endif
