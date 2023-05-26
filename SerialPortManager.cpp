#include "SerialPortManager.h"

SerialPortManager::SerialPortManager(QObject *parent) : QObject(parent)
{
    serialPort = new QSerialPort(this);

    connect(serialPort, &QSerialPort::readyRead, this, &SerialPortManager::onReadyRead);
    connect(serialPort, &QSerialPort::errorOccurred, this, &SerialPortManager::onPortDisconnected);
    connect(serialPort, &QSerialPort::aboutToClose, this, &SerialPortManager::onPortDisconnected);
}

SerialPortManager::~SerialPortManager()
{
    if (serialPort->isOpen())
        serialPort->close();

    delete serialPort;
}

void SerialPortManager::slot_UpdatePort()
{
    QStringList ports;

    foreach (const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts())
    {
        ports.append(portInfo.portName());
    }

    emit signal_AvailablePorts(ports);
}

void SerialPortManager::slot_Connect(QString port)
{
    if (serialPort->isOpen())
        serialPort->close();

    serialPort->setPortName(port);
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite))
    {
        emit signal_Connected();
    }
    else
    {
        emit signal_Disconnected();
    }
}

void SerialPortManager::slot_WriteData(const QByteArray &data)
{
    if (serialPort->isOpen())
    {
        serialPort->write(data);
        //serialPort->waitForBytesWritten();
    }

}

void SerialPortManager::onReadyRead()
{
    QByteArray data = serialPort->readAll();
    qDebug()<<"onReadyRead data  = "<< data;
    emit signal_ReadyRead(data);
}

void SerialPortManager::onPortDisconnected()
{
    if (serialPort->isOpen())
        serialPort->close();

    emit signal_Disconnected();
}
