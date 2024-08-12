#include "BLEController.h"
#include <QTimer>

// UUID usługi i charakterystyki
const QString TARGET_DEVICE_NAME = "Scyhte_ESP32";
const QString SERVICE_UUID = "a05fde7e-bacb-40b9-9856-efb85cdb8f66";
const QString CHARACTERISTIC_UUID = "eb99eb2b-048a-4fa7-a81f-4f62ca333f07";


BLEController::BLEController(QObject *parent)
    : QObject{parent}
{}

void BLEController::startBLE()
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BLEController::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BLEController::scanFinished);

    discoveryAgent->start();
}

void BLEController::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Discovered device:" << device.name() << device.address().toString();

    if (device.name() == TARGET_DEVICE_NAME) {
        qDebug() << "Target device found.";
        discoveryAgent->stop();

        controller = QLowEnergyController::createCentral(device, this);
        connect(controller, &QLowEnergyController::connected, this, &BLEController::deviceConnected);
        connect(controller, &QLowEnergyController::disconnected, this, &BLEController::deviceDisconnected);
        connect(controller, &QLowEnergyController::serviceDiscovered, this, &BLEController::serviceDiscovered);
        connect(controller, &QLowEnergyController::discoveryFinished, this, &BLEController::serviceScanDone);

        controller->connectToDevice();
    }
}

void BLEController::scanFinished()
{
    qDebug() << "Device scan finished.";
    if (!controller) {
        qDebug() << "Target device not found.";
    }
}

void BLEController::deviceConnected()
{
    qDebug() << "Connected to device.";
    controller->discoverServices();
}

void BLEController::deviceDisconnected()
{
    qDebug() << "Disconnected from device.";
}

void BLEController::serviceDiscovered(const QBluetoothUuid &newService)
{
    qDebug() << "Service discovered:" << newService.toString();
}

void BLEController::serviceScanDone()
{
    qDebug() << "Service scan done.";
    service = controller->createServiceObject(QBluetoothUuid(SERVICE_UUID), this);
    if (!service) {
        qDebug() << "Service not found.";
        return;
    }

    qDebug() << "Creating service object: " << service->serviceUuid();

    connect(service, &QLowEnergyService::stateChanged, this, [this](const QLowEnergyService::ServiceState &newState){
        qDebug() << "State changed to: " << newState;
        if (newState == QLowEnergyService::RemoteServiceDiscovered) {
            characteristic = service->characteristic(QBluetoothUuid(CHARACTERISTIC_UUID));
            qDebug() << "Characteristic: " << characteristic.uuid();
            if (!characteristic.isValid()) {
                qDebug() << "Characteristic not found.";
                return;
            }

            service->readCharacteristic(characteristic);
        }
    });

    connect(service, &QLowEnergyService::characteristicRead, this, &BLEController::characteristicChanged);
    connect(service, &QLowEnergyService::characteristicChanged, this, &BLEController::characteristicChanged);
    service->discoverDetails();
}

void BLEController::characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    if (characteristic.uuid() == QBluetoothUuid(CHARACTERISTIC_UUID)) {
        qDebug() << "Characteristic value changed:" << value;
        m_distance = value.toFloat();
        emit distanceChanged();
        service->readCharacteristic(characteristic);
    }
}
