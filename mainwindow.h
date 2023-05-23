#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <SerialPortManager.h>
#include <qcustomplot.h>

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
    Ui::MainWindow *ui;
    SerialPortManager *serial_port_manager;
    QList<double> temper_list;
    int n_points_plot = 100;
    double pValue;
    double iValue;
    double dValue;

    void drawLastNPoints(QCustomPlot* plot, const QList<double>& temper_list, int n_points, double pValue, double iValue, double dValue);



};
#endif // MAINWINDOW_H
