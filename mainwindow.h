#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <SerialPortManager.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_startStopHeat_clicked();

    void on_pushButton_getTemp_clicked();

    void on_pushButton_saveScreen_clicked();

    void on_pushButton_setCoeff_clicked();

    void on_pushButton_getCoeff_clicked();

    void on_pushButton_connect_clicked();

    void on_pushButton_updatePort_clicked();

private:
    SerialPortManager *serial_port_manager;



private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
