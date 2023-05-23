#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialPortManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortManager(QObject *parent = nullptr);
    ~SerialPortManager();

public slots:
    void slot_UpdatePort();
    void slot_Connect(QString port);
    void slot_WriteData(const QByteArray &data);

signals:
    void signal_AvailablePorts(QStringList ports);
    void signal_Connected();
    void signal_Disconnected();
    void signal_ReadyRead(QByteArray data);

private:
    QSerialPort *serialPort;

private slots:
    void onReadyRead();
    void onPortDisconnected();
};

#endif // SERIALPORTMANAGER_H
