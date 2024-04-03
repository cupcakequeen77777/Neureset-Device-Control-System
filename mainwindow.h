#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <NeuresetController.h>
#include "Battery.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void disconnectSite(int eegId);


private slots:

    void on_btn_pauseTreatement_clicked();

    void on_btn_continueTreatment_clicked();

    void on_btn_stopTreatement_clicked();

    void on_btn_disconnectSite_clicked();

    void on_widget_menuOpts_itemActivated(QListWidgetItem *item);

    void on_btn_on_clicked();

    void on_btn_off_clicked();

    void on_btn_setDate_clicked();

    //slot for recieving signals from controller
    void updateTreatmentTime(const QString& time);

    // slot to update the UI based on battery level
    void updateBatteryLevel(int level);

    // slot to handle battery depletion
    void handleBatteryDepleted();


private:
    Ui::MainWindow *ui;
    NeuresetController* controller = NeuresetController::getInstance();

    Battery* batteryInstance;
    void initializeBatteryStuff();
};
#endif // MAINWINDOW_H
