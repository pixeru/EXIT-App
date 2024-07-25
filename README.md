
## Overview
Here, it contains 2 code: UI and Cmd (Command Line) version.
The UI version is created with Qt, and it needs many files, whereas cmd, only need a file, that is the cmd itself.

## Qt Code (Compile the .exe and use it in Windows PC)

```

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

  

// Input Password Textbox

void MainWindow::on_pushButton_clicked()

{

if(ui->lineEdit->text() == "123") {

if(serial->isOpen()) {

serial->write("1"); // Sending '1' as a signal to turn on

ui->label_2->setText("Correct!"); // Set text to "Correct!" on label_2

}

} else {

ui->label_2->setText(""); // Optionally blank text if the condition is not met

}

}

  

// "Turn Off" Button

void MainWindow::on_pushButton_2_clicked()

{

if(serial->isOpen()) {

serial->write("0"); // Sending '0' as a signal to turn off

}

}

  
  

```

  

## Arduino Code (Upload to Arduino)

```

const int PINNUM = 12;

// Code Writen by Josh

// 3/13/2024

  

void turnOn() {

digitalWrite(PINNUM, HIGH);

}

  

void turnOff() {

digitalWrite(PINNUM, LOW);

}

  

void beepBoop() {

digitalWrite(12, HIGH); // Turn on the LED

delay(1000); // Wait for 1 second

digitalWrite(12, LOW); // Turn off the LED

delay(1000); // Wait for 1 second

}

  

void setup() {

Serial.begin(9600); // Start serial communication at 9600 baud rate

pinMode(12, OUTPUT); // Set pin 12 as an output

}

  

void loop() {

if (Serial.available() > 0) {

int inByte = Serial.read();

if (inByte == '1') {

turnOn();

} else if (inByte == '0') {

turnOff();

}

}

}

  

// void setup() {

// pinMode(12, OUTPUT); // Set pin 12 as an output

// turnOff();

// //delay(3000);

// //turnOff();

// }

  

// // Loop Here

// void loop() {

// }

```