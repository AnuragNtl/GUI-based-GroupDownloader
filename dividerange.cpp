#include "dividerange.h"
#include "ui_dividerange.h"
#include<QSlider>
#include<QLabel>
#include<QLineEdit>
#include<QDebug>
#include<cmath>
DivideRange::DivideRange(vector<string> names,map<string,Range> *ranges,int contentLength
                         ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DivideRange),
    ranges(ranges),maxVal(contentLength)
{
    this->names=names;
    ui->setupUi(this);
    addComponents();
}

void DivideRange :: addComponents()
{
    QList<QAbstractButton*> buttons=ui->buttonBox->buttons();
    connect(buttons.at(0),&QPushButton :: clicked,this,&DivideRange :: onOKClicked);
    connect(buttons.at(1),&QPushButton :: clicked,this,&DivideRange :: onCloseClicked);
    connect(ui->pushButton,&QPushButton :: clicked,this,&DivideRange :: onResetClicked);
    vector<string> :: iterator it1=names.begin();
    int sz=ranges->size();
    vector<string> :: iterator it2=names.end();
    it2--;
    int s=(*ranges)[*it2].end;
    int interval=s/sz;
    for(int i=0;it1!=names.end();i++)
    {
        string n=*it1;
        Range r=(*ranges)[*it1];
        itemPos.insert(pair<string,int>(n,i));
        QSlider *slider1=new QSlider;
        slider1->setMinimum(0);
        slider1->setMaximum(s);
        int value=r.end-r.start+1;
        slider1->setValue(value);
        prevVals.push_back(value);
        slider1->setTickInterval(interval);
        slider1->setTickPosition(QSlider::TicksAbove);
        slider1->setOrientation(Qt::Horizontal);
        QLabel *l1=new QLabel;
        QLineEdit *t1=new QLineEdit,*t2=new QLineEdit;
        t1->setText(QString :: number(r.start));
        t2->setText(QString :: number(r.end));
        QLabel *l2=new QLabel;
        l2->setText("-");
        l1->setText(QString(n.c_str()));
        t1->setEnabled(false);
        t2->setEnabled(false);
        ui->gridLayout->addWidget(l1,i,0);
        ui->gridLayout->addWidget(slider1,i,1);
        ui->gridLayout->addWidget(t1,i,2);
        ui->gridLayout->addWidget(l2,i,3);
        ui->gridLayout->addWidget(t2,i,4);
        connect(slider1,&QSlider::sliderReleased,this,&DivideRange::onSliderChange);
        //connect(slider1,&QSlider::valueChanged,this,&DivideRange::onSliderUpdate);
        it1++;
    }
}
void DivideRange :: onOKClicked()
{
    vector<string> :: iterator i1=names.begin();
    for(int i=0;i1!=names.end();i1++,i++)
    {
        QLineEdit *t1=(QLineEdit *)ui->gridLayout->itemAtPosition(i,2)->widget();
        QLineEdit *t2=(QLineEdit *)ui->gridLayout->itemAtPosition(i,4)->widget();
        Range r;
        r.start=t1->text().toInt();
        r.end=t2->text().toInt();
        (*ranges)[*i1]=r;
    }
}

void DivideRange :: onCloseClicked()
{
    this->close();
    this->deleteLater();
}
void DivideRange :: onResetClicked()
{
vector<string> :: iterator i1=names.begin();
for(int i=0;i1!=names.end();i++,i1++)
{
    QSlider *sl1=(QSlider *)ui->gridLayout->itemAtPosition(i,1)->widget();
    QLineEdit *t1=(QLineEdit *)ui->gridLayout->itemAtPosition(i,2)->widget();
    QLineEdit *t2=(QLineEdit *)ui->gridLayout->itemAtPosition(i,4)->widget();
    Range r=(*ranges)[*i1];
    sl1->setValue(r.end-r.start+1);
    t1->setText(QString :: number(r.start));
    t2->setText(QString :: number(r.end));
}
}

void DivideRange :: sliderUpdate(QSlider *sl1)
{
int indx=ui->gridLayout->indexOf(sl1);
int row,col,rs,cs;
//qDebug() <<"Slider Update";
ui->gridLayout->getItemPosition(indx,&row,&col,&rs,&cs);
QLineEdit *t1=(QLineEdit *)ui->gridLayout->itemAtPosition(row,2)->widget(),
        *t2=(QLineEdit *)ui->gridLayout->itemAtPosition(row,4)->widget();
int s=0;
if(row!=0)
{
        QLineEdit *prev=(QLineEdit *)ui->gridLayout->itemAtPosition(row-1,4)->widget();
        s=prev->text().toInt()+1;
}
//qDebug() <<"s at " <<row <<" " <<s;
t1->setText(QString::number(s));
t2->setText(QString::number(s+sl1->value()-1));
//qDebug() <<"Slider Updated";
}

void DivideRange :: onSliderChange()
{
    //qDebug() <<"Slider Changed";
QSlider *k=(QSlider *)sender();
int ct=prevVals.size();
int max=k->maximum();
int value=k->value();
int indx=ui->gridLayout->indexOf(k);
int row,col,rs,cs;
ui->gridLayout->getItemPosition(indx,&row,&col,&rs,&cs);
//qDebug() <<"Pos " <<row <<" ct " <<ct;
int change=prevVals[row]-value;
if(change==0 || (prevVals[row]>=max-ct+1 && change<0))
{
    k->setValue(prevVals[row]);
    return;
}
if(change>0 && value==0)
{
    value=1;
    k->setValue(1);
    change=prevVals[row]-value;
    if(change==0)
        return;
}
if(change<0 && max-value<ct-1)
{
    value=max-ct+1;
    k->setValue(value);
    change=prevVals[row]-value;
    if(change==0)
        return;
}
if(abs(change)<ct-1)
{
    ct=abs(change)+1;
}
int add=change/(ct-1);
bool d=false;
prevVals[row]=value;
int l=0;
int stp=0;
int achange=abs(change);
int remaining=0,diff;
for(int i=0;stp<achange;i=(i>=(ct-1)?0:i+1))
{
//    qDebug() <<stp <<" " <<change <<" " <<i <<(change<0);
    if(i==0)
{
        remaining=achange-stp;
        add=remaining/(ct-1);
        diff=remaining%(ct-1);
}
        QSlider *s=(QSlider *)ui->gridLayout->itemAtPosition(i,1)->widget();
       // qDebug() <<k  <<" " <<s <<" " <<(k==s);
    if(k==s)
    {
       // sliderUpdate(s);
        continue;
   }
        if(change<0)
   {
       int sub=add;
       int lval=prevVals[i]-1;
  //     qDebug() <<"Sub " <<sub <<" Lval " <<lval;
       if(sub>lval)
       {
           stp+=lval;
           prevVals[i]=1;
       }
       else
       {
           stp+=sub;
           prevVals[i]=prevVals[i]-sub;
       }
       lval=prevVals[i]-1;
       if(diff>lval)
       {
           stp+=lval;
           prevVals[i]=1;
           diff=diff-lval;
       }
       else
       {
           stp+=diff;
           prevVals[i]-=diff;
           diff=0;
       }
   }
   else
   {
       prevVals[i]+=add;
       stp+=add;
       if(diff+prevVals[i]<=max-ct-1)
        {
           prevVals[i]+=diff;
       stp+=diff;
           diff=0;
       }
   }
   s->setValue(prevVals[i]);
//   sliderUpdate(s);
}
//sliderUpdate(k);
for(int i=0;i<prevVals.size();i++)
{
    sliderUpdate((QSlider *)ui->gridLayout->itemAtPosition(i,1)->widget());
}
}

DivideRange::~DivideRange()
{
    delete ui;
}
