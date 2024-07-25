    // Code Writen by Josh
    // 3/17/2024

    #include "mainwindow.h"
    #include "ui_mainwindow.h"

    #include <QtSerialPort/QSerialPort>



    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        serial(new QSerialPort(this)) // Initialize the QSerialPort object
    {
        ui->setupUi(this);

        serial->setPortName("COM3"); // Set the port name to your Arduino's port
        serial->setBaudRate(QSerialPort::Baud9600); // Set baud rate to 9600

        if(serial->open(QIODevice::ReadWrite)) {
            // Successfully opened the port
        } else {
            // Failed to open the port, handle error here
        }
    }

    MainWindow::~MainWindow()
    {
        if(serial->isOpen()) {
            serial->close();
        }
        delete ui;
    }



    //// UI Code ////
    void MainWindow::on_turnOnButton_clicked()
    {
        turnOn();
    }

    void MainWindow::on_turnOffButton_clicked()
    {
        turnOff();
    }



    //// Functions ////
    void MainWindow::turnOn()
    {
        if(serial->isOpen()) {
            serial->write("1"); // Sending '1' as a signal to turn on
        } else {
            // Print debug message
            qWarning() << "Serial Port isn't open!";
        }
    }

    void MainWindow::turnOff()
    {
        if(serial->isOpen()) {
            serial->write("0"); // Sending '0' as a signal to turn off
        } else {
            // Print debug message
            qWarning() << "Serial Port isn't open!";
        }
    }
