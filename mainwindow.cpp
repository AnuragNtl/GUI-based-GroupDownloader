#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent)
{
    cout <<"__::__\n";
    ui=new Ui::Dialog;
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(onStartServerClicked()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(onStartClientClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
