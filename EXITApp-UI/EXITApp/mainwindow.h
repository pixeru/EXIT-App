#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QtSerialPort/QSerialPort"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // void on_pushButton_clicked();

    // void on_pushButton_2_clicked();

    void on_turnOffButton_clicked();

    void on_turnOnButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;

    void turnOn();
    void turnOff();
};
#endif // MAINWINDOW_H
