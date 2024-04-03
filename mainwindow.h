#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <NeuresetController.h>
#include "defs.h"
#include <QTimer>
#include <QtDebug>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void contactLost(bool);

signals:



private slots:

    void on_btn_pauseTreatement_clicked();

    void on_btn_continueTreatment_clicked();

    void on_btn_stopTreatement_clicked();

    void on_btn_disconnectSite_clicked();

    void on_widget_menuOpts_itemActivated(QListWidgetItem *item);

    void on_btn_on_clicked();

    void on_btn_off_clicked();

    void on_btn_setDate_clicked();

    void on_btn_connectSites_clicked();

private:
    Ui::MainWindow *ui;
    NeuresetController* controller = NeuresetController::getInstance();
    void createChart();
    int waveformData[60];
};
#endif // MAINWINDOW_H
