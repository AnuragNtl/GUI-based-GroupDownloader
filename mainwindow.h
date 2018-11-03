#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include<QPushButton>
namespace Ui {
class Dialog;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::Dialog *ui;
private slots:
    void onStartServerClicked();
    void onStartClientClicked();
};

#endif // MAINWINDOW_H
