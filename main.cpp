#include "mainwindow.h"

#include <QApplication>

#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
    //seed for random number generating
    srand(time(0));
    cout <<"Hello World!";
    cout <<"LeeAnne Here";
    cout <<endl<<"Sara Here"<<endl;
    cout << "Emrehan test!" << endl;
    cout << "Knox's saying hello to everyone!" << endl;
    
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
