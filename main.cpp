#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

// Пример кода на Qt C++ для компьютера

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QCoreApplication>
#include <QDebug>

/*
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSerialPort serial;
    serial.setPortName("COM1"); // Укажите соответствующий порт
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (!serial.open(QIODevice::ReadWrite))
    {
        qDebug() << "Ошибка открытия порта:" << serial.errorString();
        return 1;
    }

    // Установка коэффициентов PID
    QString setPidCommand = "SET_PID 1.0 0.5 0.2";
    serial.write(setPidCommand.toUtf8());
    serial.waitForBytesWritten();

    // Запрос данных о коэффициентах PID
    QString getPidCommand = "GET_PID";
    serial.write(getPidCommand.toUtf8());
    serial.waitForBytesWritten();

    // Команда на запуск нагрева
    QString startHeatingCommand = "START_HEATING";
    serial.write(startHeatingCommand.toUtf8());
    serial.waitForBytesWritten();

    // Отправка целевой температуры
    QString setTargetTempCommand = "SET_TARGET_TEMP 50";
    serial.write(setTargetTempCommand.toUtf8());
    serial.waitForBytesWritten();

    // Считывание текущей температуры
    QString getCurrentTempCommand = "GET_CURRENT_TEMP";
    serial.write(getCurrentTempCommand.toUtf8());
    serial.waitForBytesWritten();

    // Чтение ответа от микроконтроллера
    QByteArray responseData = serial.readAll();
    qDebug() << "Ответ от микроконтроллера:" << responseData;

    serial.close();

    return 0;
}*/
