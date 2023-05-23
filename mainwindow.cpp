#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial_port_manager = new SerialPortManager();

    connect(serial_port_manager, &SerialPortManager::signal_AvailablePorts, ui->comboBox_port, &QComboBox::addItems);
    connect(ui->pushButton_updatePort, &QPushButton::clicked, serial_port_manager, &SerialPortManager::slot_UpdatePort);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete serial_port_manager;
}


void MainWindow::on_pushButton_startStopHeat_clicked()
{

}


void MainWindow::on_pushButton_getTemp_clicked()
{

}


void MainWindow::on_pushButton_saveScreen_clicked()
{

}


void MainWindow::on_pushButton_setCoeff_clicked()
{

}


void MainWindow::on_pushButton_getCoeff_clicked()
{

}


void MainWindow::on_pushButton_connect_clicked()
{
    serial_port_manager->slot_Connect(ui->comboBox_port->currentText());
}


void MainWindow::on_pushButton_updatePort_clicked()
{
    ui->comboBox_port->clear();
}

