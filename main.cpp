#include "mainwindow.h"

#include <QApplication>

#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
    cout <<"Hello World!";
    cout <<"LeeAnne Here";
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
