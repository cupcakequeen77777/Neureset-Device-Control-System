#include "mainwindow.h"
#include "ui_mainwindow.h"

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


void MainWindow::on_pushButton_clicked()
{

}


void MainWindow::on_btn_pauseTreatement_clicked()
{

}


void MainWindow::on_btn_continueTreatment_clicked()
{

}


void MainWindow::on_btn_stopTreatement_clicked()
{

}


void MainWindow::on_btn_disconnectSite_clicked()
{

}





void MainWindow::on_widget_menuOpts_itemClicked(QListWidgetItem *item)
{

}

