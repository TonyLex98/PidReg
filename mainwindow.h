#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <SerialPortManager.h>
#include <qcustomplot.h>

#define START_HEATING "START_HEATING"
#define SET_PID "SET_PID"
#define GET_PID "GET_PID"
#define SET_TARGET_TEMP "SET_TARGET_TEMP"
#define GET_CURRENT_TEMP "GET_CURRENT_TEMP"



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

    void updateKoeff();

    void on_pushButton_getTemp_2_clicked();

    void on_pushButton_setTargetTemp_clicked();

public slots:
    void processReceivedData(QByteArray data);

private:
    Ui::MainWindow *ui;
    SerialPortManager *serial_port_manager;
    QList<double> temper_list;
    int n_points_plot = 100;
    float pValue;
    float iValue;
    float dValue;
    QString split_sign = "\t";

    float targetTemp = 50;

    void drawLastNPoints(QCustomPlot* plot, const QList<double>& temper_list, int n_points, double pValue, double iValue, double dValue, float targetTemp);



};
#endif // MAINWINDOW_H
