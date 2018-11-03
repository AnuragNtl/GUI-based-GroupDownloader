#ifndef ADDCLIENT_H
#define ADDCLIENT_H

#include <QDialog>
#include"serverui.h"
namespace Ui {
class AddClient;
}

class AddClient : public QDialog
{
    Q_OBJECT

public:
    explicit AddClient(QWidget *parent = 0);
    ~AddClient();

private:
    Ui::AddClient *ui;
    ServerUI *s;
public slots:
    void onAddClient();
};

#endif // ADDCLIENT_H
