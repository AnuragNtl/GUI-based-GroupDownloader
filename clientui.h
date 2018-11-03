#ifndef CLIENTUI_H
#define CLIENTUI_H

#include <QDialog>
#include<QString>
namespace Ui {
class ClientUI;
}

class ClientUI : public QDialog
{
    Q_OBJECT

public:
    explicit ClientUI(QWidget *parent = 0);
    ~ClientUI();
    void onStarted(QString host,QString name);
    void updateProgress(int p);
private:
    Ui::ClientUI *ui;
private slots:
    void onStart();
};

#endif // CLIENTUI_H
