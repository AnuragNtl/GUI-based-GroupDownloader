#ifndef DIVIDERANGE_H
#define DIVIDERANGE_H

#include <QDialog>
#include<map>
#include"Range.h"
#include<QSlider>
using namespace std;
namespace Ui {
class DivideRange;
}

class DivideRange : public QDialog
{
    Q_OBJECT
private:
    Ui::DivideRange *ui;
    map<string,Range> *ranges;
    map<string,int> itemPos;
    vector<string> names;
    vector<int> prevVals;
    int maxVal;
    void sliderUpdate(QSlider *s);
public:
    explicit DivideRange(vector<string>,map<string,Range> *,int,QWidget *parent = 0);
    void addComponents();
    ~DivideRange();
public slots:
    void onSliderChange();
    void onResetClicked();
    void onOKClicked();
    void onCloseClicked();

};

#endif // DIVIDERANGE_H
