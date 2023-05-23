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

    ui->lineEdit_P->setText(QString::number(pValue));
    ui->lineEdit_I->setText(QString::number(iValue));
    ui->lineEdit_D->setText(QString::number(dValue));

    n_points_plot = 100;

    targetTemp = 50;
    ui->lineEdit_tergetTemp->setText(QString::number(targetTemp));

    for(int i = 0; i<500; i++)
        temper_list.push_back(sin(2*M_PI*i*0.05));

    drawLastNPoints(ui->customPlotTemper, temper_list, n_points_plot, pValue, iValue, dValue, targetTemp);

    connect(ui->lineEdit_P, &QLineEdit::textChanged, this, &MainWindow::updateKoeff);
    connect(ui->lineEdit_I, &QLineEdit::textChanged, this, &MainWindow::updateKoeff);
    connect(ui->lineEdit_D, &QLineEdit::textChanged, this, &MainWindow::updateKoeff);
    connect(ui->lineEdit_tergetTemp, &QLineEdit::textChanged, this, &MainWindow::updateKoeff);

    connect(serial_port_manager, &SerialPortManager::signal_ReadyRead, this, &MainWindow::processReceivedData);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete serial_port_manager;
}


void MainWindow::on_pushButton_startStopHeat_clicked()
{
    // Команда на запуск нагрева
    QString startHeatingCommand = START_HEATING;
    serial_port_manager->slot_WriteData(startHeatingCommand.toUtf8());
}


void MainWindow::on_pushButton_getTemp_clicked()
{
    // Считывание текущей температуры
    QString getCurrentTempCommand = GET_CURRENT_TEMP;
    serial_port_manager->slot_WriteData(getCurrentTempCommand.toUtf8());
}

void MainWindow::on_pushButton_saveScreen_clicked()
{
    // Создание QPixmap для сохранения скриншота
    QPixmap screenshot(ui->customPlotTemper->size());

    // Заполнение QPixmap содержимым графика
    screenshot = ui->customPlotTemper->toPixmap();

    // Отображение диалогового окна для выбора пути сохранения
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Screenshot", QString("Plot_P=%1_I=%2_D=%3.png").arg(pValue).arg(iValue).arg(dValue), "Images (*.png)");

    if (!filePath.isEmpty())
    {
        // Сохранение скриншота в файл
        screenshot.save(filePath);
    }
}


void MainWindow::on_pushButton_setCoeff_clicked()
{
    // Установка коэффициентов PID
    pValue = ui->lineEdit_P->text().toFloat();
    iValue = ui->lineEdit_I->text().toFloat();
    dValue = ui->lineEdit_D->text().toFloat();

   QString setPidCommand = SET_PID + split_sign + QString::number(pValue) + split_sign + QString::number(iValue) + split_sign+ QString::number(dValue);
   qDebug()<<"setPidCommand = "<< setPidCommand;
   serial_port_manager->slot_WriteData(setPidCommand.toUtf8());
}

void MainWindow::on_pushButton_getCoeff_clicked()
{

    // Запрос данных о коэффициентах PID
    QString getPidCommand = GET_PID;
    serial_port_manager->slot_WriteData(getPidCommand.toUtf8());
}

void MainWindow::on_pushButton_connect_clicked()
{
    serial_port_manager->slot_Connect(ui->comboBox_port->currentText());
}

void MainWindow::on_pushButton_updatePort_clicked()
{
    ui->comboBox_port->clear();
}

void MainWindow::updateKoeff()
{
    // Установка коэффициентов PID
    pValue = ui->lineEdit_P->text().toFloat();
    iValue = ui->lineEdit_I->text().toFloat();
    dValue = ui->lineEdit_D->text().toFloat();
    targetTemp = ui->lineEdit_tergetTemp->text().toFloat();

    drawLastNPoints(ui->customPlotTemper, temper_list, n_points_plot, pValue, iValue, dValue, targetTemp);
}

void MainWindow::drawLastNPoints(QCustomPlot* plot, const QList<double>& temper_list, int n_points, double pValue, double iValue, double dValue, float targetTemp)
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
    plot->yAxis->setRange(qMin(targetTemp,(float)*std::min_element(temper_list.constBegin(), temper_list.constEnd())),
                         qMax(targetTemp,(float)*std::max_element(temper_list.constBegin(), temper_list.constEnd())));

    // Добавление значений коэффициентов P, I и D в подпись
    QString coefficients = QString("P=%1,\n I=%2,\n D=%3").arg(pValue).arg(iValue).arg(dValue);
    // Добавление подписей
    plot->graph(0)->setName("Temperature with Coefficients:\n " + coefficients);
    plot->xAxis->setLabel("Time (Sec)");
    plot->yAxis->setLabel("Temperature (C)");
    plot->legend->setVisible(true);


    // Создание второго графика
    plot->addGraph();

    // Заполнение данных для горизонтальной линии
    QVector<double> xTargetData(n_points);
    QVector<double> yTargetData(n_points, targetTemp);

    for (int i = startIndex; i < totalPoints; ++i)
    {
        // Заполнение x-координат данными (здесь просто увеличиваем индекс)
        xTargetData[i - startIndex] = i;
    }

    plot->graph(1)->setName("Target Temperature: " + QString::number(targetTemp));
    // Установка данных для горизонтальной линии
    plot->graph(1)->setData(xTargetData, yTargetData);

    // Установка цвета горизонтальной линии
    plot->graph(1)->setPen(QPen(Qt::red));

    // Перерисовка графика
    plot->replot();
}




void MainWindow::on_pushButton_getTemp_2_clicked()
{

}


void MainWindow::on_pushButton_setTargetTemp_clicked()
{
    targetTemp = ui->lineEdit_tergetTemp->text().toFloat();

    QString setTargetTempCommand = SET_TARGET_TEMP+ split_sign + QString::number(targetTemp);
    serial_port_manager->slot_WriteData(setTargetTempCommand.toUtf8());
}

void MainWindow::processReceivedData(QByteArray data)
{
    // Разбор команды
    QStringList data_list= QString::fromUtf8(data).split('\t');

    if(data_list.size()==0)
        return;

    QString command = data_list.at(0);

    if (command == SET_PID)
    {
        // Получение коэффициентов PID
        if (sscanf(data.constData(), "SET_PID\t%f\t%f\t%f", &pValue, &iValue, &dValue) == 3)
        {
            // Обработка команды SET_PID
            updateKoeff();
            qDebug() << "Received SET_PID command. pCoeff =" << pValue << "iCoeff =" << iValue << "dCoeff =" << dValue;
        }
        else
        {
            // Ошибка разбора команды SET_PID
            qDebug() << "Error parsing SET_PID command";
        }
    }
    else if (command == GET_PID)
    {
        // Обработка команды GET_PID
        qDebug() << "Received GET_PID command";
    }
    else if (command == START_HEATING)
    {
        // Обработка команды START_HEATING
        qDebug() << "Received START_HEATING command";
    }
    else if (command.startsWith(SET_TARGET_TEMP))
    {
        // Получение целевой температуры
        if (sscanf(data.constData(), "SET_TARGET_TEMP\t%f", &targetTemp) == 1)
        {
            // Обработка команды SET_TARGET_TEMP
            updateKoeff();
            qDebug() << "Received SET_TARGET_TEMP command. targetTemp =" << targetTemp;
        }
        else
        {
            // Ошибка разбора команды SET_TARGET_TEMP
            qDebug() << "Error parsing SET_TARGET_TEMP command";
        }
    }
    else if (command == GET_CURRENT_TEMP)
    {
        // Обработка команды GET_CURRENT_TEMP
        float current_temp;
        if (sscanf(data.constData(), "GET_CURRENT_TEMP\t%f", &current_temp) == 1)
        {
            // Обработка команды GET_CURRENT_TEMP
            temper_list.append(current_temp);
            updateKoeff();
            qDebug() << "Received GET_CURRENT_TEMP command. current_temp =" << current_temp;
        }
        else
        {
            // Ошибка разбора команды GET_CURRENT_TEMP
            qDebug() << "Error parsing GET_CURRENT_TEMP command";
        }
    }
    else
    {
        // Неизвестная команда
        qDebug() << "Unknown command:" << command;
    }
}

