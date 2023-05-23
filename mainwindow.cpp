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

    pValue = 2;
    iValue = 0.5;
    dValue = 0.1;

    n_points_plot = 100;

    for(int i = 0; i<50; i++)
        temper_list.push_back(sin(2*M_PI*i*0.05));

    drawLastNPoints(ui->customPlotTemper, temper_list, n_points_plot, pValue, iValue, dValue);

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

void MainWindow::drawLastNPoints(QCustomPlot* plot, const QList<double>& temper_list, int n_points, double pValue, double iValue, double dValue)
{
    // Очистка графика
    plot->clearGraphs();

    // Создание графика
    plot->addGraph();

    // Получение количества точек
    int totalPoints = temper_list.size();

    // Проверка, достаточно ли точек для отображения
    if (totalPoints < n_points)
        n_points = totalPoints;

    // Вычисление начального индекса для отображения последних n_points точек
    int startIndex = totalPoints - n_points;

    // Заполнение данных для графика
    QVector<double> xData(n_points);
    QVector<double> yData(n_points);

    for (int i = startIndex; i < totalPoints; ++i)
    {
        // Заполнение x-координат данными (здесь просто увеличиваем индекс)
        xData[i - startIndex] = i;

        // Заполнение y-координат данными из массива temper_list
        yData[i - startIndex] = temper_list[i];
    }

    // Установка данных для графика
    plot->graph(0)->setData(xData, yData);

    // Установка пределов осей графика
    plot->xAxis->setRange(startIndex, totalPoints - 1);
    plot->yAxis->setRange(*std::min_element(temper_list.constBegin(), temper_list.constEnd()),
                         *std::max_element(temper_list.constBegin(), temper_list.constEnd()));

    // Добавление значений коэффициентов P, I и D в подпись
    QString coefficients = QString("P=%1,\n I=%2,\n D=%3").arg(pValue).arg(iValue).arg(dValue);
    // Добавление подписей
    plot->graph(0)->setName("Temperature with Coefficients:\n " + coefficients);
    plot->xAxis->setLabel("Time (Sec)");
    plot->yAxis->setLabel("Temperature (C)");
    plot->legend->setVisible(true);


    // Перерисовка графика
    plot->replot();
}


