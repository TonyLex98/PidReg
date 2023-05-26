#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <SerialPortManager.h>
#include <qcustomplot.h>
#include <QTimer>

#define START_HEATING "START_HEATING"
#define STOP_HEATING "STOP_HEATING"
#define SET_PID "SET_PID"
#define GET_PID "GET_PID"
#define SET_TARGET_TEMP "SET_TARGET_TEMP"
#define GET_TARGET_TEMP "GET_TARGET_TEMP"
#define GET_CURRENT_TEMP "GET_CURRENT_TEMP"
#define PULSE "PULSE"



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

    void on_pushButton_setTargetTemp_clicked();

    void on_pushButton_setPlotPoints_clicked();

    void on_pushButton_autoKoeff_clicked();

    void on_pushButton_getTargetTemp_clicked();

    void on_pushButton_startStopTempMeas_clicked();

public slots:
    void processReceivedData(QByteArray data);

private:
    Ui::MainWindow *ui;
    SerialPortManager *serial_port_manager;
    QTimer *timerGetTemperature;

    QList<double> temper_list;
    int n_points_plot = 100;
    float pValue;
    float iValue;
    float dValue;
    QString split_sign = "\t";
    QString end_sign = "\r";

    float targetTemp = 50;
    float pulse = 0;

    bool isHeatStarted = false;
    bool isStartMeasTemp = false;

    bool isAutoKoeff = 0;

    int counter_current = 0;
    int counter_lim = 1000;

    float current_temp;

    void changeButtonColor(QPushButton* button, const QString& color) {
        button->setStyleSheet("background-color: " + color + ";");
    }

    void changeLineEditColor(QLineEdit* lineEdit, const QString& color) {
        lineEdit->setStyleSheet("background-color: " + color + ";");
    }

    void drawLastNPoints(QCustomPlot* plot, const QList<double>& temper_list, int n_points, double pValue, double iValue, double dValue, float targetTemp);



};
#endif // MAINWINDOW_H
