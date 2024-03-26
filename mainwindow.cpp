#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_pauseTreatement_clicked()
{
    qDebug ("on_btn_pauseTreatement_clicked");
}


void MainWindow::on_btn_continueTreatment_clicked()
{
    qDebug ("continue Treatment");
}


void MainWindow::on_btn_stopTreatement_clicked()
{
    qDebug ("stop Treatment");
}


void MainWindow::on_btn_disconnectSite_clicked()
{
    qDebug ("disconnect Site");
}


void MainWindow::on_widget_menuOpts_itemActivated(QListWidgetItem *item)
{
    qDebug ("menuOpts_itemActivated");
}

