#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <QTimer>

class Battery : public QObject {
    Q_OBJECT

public:
    Battery(QObject *parent = nullptr);
    ~Battery();

    void startBatteryConsumption();
    void stopBatteryConsumption();
    int getBatteryLevel() const;

signals:
    void lowBatteryWarning();
    void batteryLevelChanged(int newLevel);
    void batteryDepleted();

private slots:
    void consumePower();

private:
    int batteryLevel;
    QTimer *timer;
};

#endif
