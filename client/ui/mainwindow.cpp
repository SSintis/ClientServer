#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked,
            this, &MainWindow::onSendButtonClicked);

    connect(ui->pushButton_receiver, &QPushButton::clicked,
            this, &MainWindow::onReceiverButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSendButtonClicked(){
    QString text = ui->lineEdit->text();

    if(!text.isEmpty()){
        ui->textBrowser->append(text);
        ui->lineEdit->clear();
    }
}

void MainWindow::onReceiverButtonClicked(){
    // Here we swap receiver
    QString new_receiver = ui->lineEdit_receiver->text();

    // client-server code...
}
