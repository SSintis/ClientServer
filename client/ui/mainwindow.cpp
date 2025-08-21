#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

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

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::setChatClient(ChatClient *chatClient){
    client = chatClient;

    connect(client, &ChatClient::connection_error, this, &MainWindow::onConnectionError);
    connect(client, &ChatClient::message_received, this, &MainWindow::onMessageReceived);
}

void MainWindow::onSendButtonClicked(){
    QString message = ui->lineEdit->text();

    if(!message.isEmpty()){
        client->send_message(message.toStdString());

        ui->textBrowser->append("You: " + message);
        ui->lineEdit->clear();
    }
}

void MainWindow::onMessageReceived(const QString &sender, const QString &message){
    ui->textBrowser->append(sender + ": " + message);
}

void MainWindow::onConnectionError(const QString &error){
    //QMessageBox::critical(this, "Connection error", error);
    close();
}

void MainWindow::onReceiverButtonClicked(){
    QString new_receiver = ui->lineEdit_receiver->text();
    this->client->set_new_receiver(new_receiver.toStdString());
}
