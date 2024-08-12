#ifndef BLECONTROLLER_H
#define BLECONTROLLER_H

#include <QObject>
#include <QQmlEngine>

#include <QLowEnergyController>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyService>

class BLEController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(float distance MEMBER m_distance NOTIFY distanceChanged FINAL)
public:
    explicit BLEController(QObject *parent = nullptr);


public slots:
    void startBLE();
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void scanFinished();
    void deviceConnected();
    void deviceDisconnected();
    void serviceDiscovered(const QBluetoothUuid &newService);
    void serviceScanDone();
    void characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);

signals:
    void distanceChanged();

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QLowEnergyController *controller = nullptr;
    QLowEnergyService *service = nullptr;
    QLowEnergyCharacteristic characteristic;
    float m_distance = 0;
};

#endif // BLECONTROLLER_H
