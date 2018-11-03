#ifndef SERVERUI_H
#define SERVERUI_H

#include <QDialog>
#include"distribution.h"
#include<iostream>
namespace Ui {
class ServerUI;
}
class ServerUIUpdater;
class ServerUI : public QDialog
{
    Q_OBJECT

private:
    Ui::ServerUI *ui;
    Distribution *dist;
    string curClient;
    ServerUIUpdater *updater;
    map<string,Range> ranges;
    bool serverStarted,changeMade;
    void saveDistData();
    bool loadDistData();
public:
 ServerUI(QWidget *parent,Distribution *d);
    ~ServerUI();
public slots:
    void onStart();
    void onAddClient();
    void onDelClient();
    void onAddClientStart();
    void onUpdate();
    void onRangeDivisionShow();
    void dataChanged();
    friend class AddClient;
};
class ServerUIUpdater : public QThread
{
    Q_OBJECT
public:
    void run();
signals:
    void updateProgress();
};

#endif // SERVERUI_H
