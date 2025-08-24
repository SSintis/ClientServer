#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    userButtonGroup = new QButtonGroup(this);
    userButtonGroup->setExclusive(true);

    usersLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    usersLayout->setAlignment(Qt::AlignTop);
    usersLayout->setSpacing(2);
    usersLayout->setContentsMargins(2, 2, 2, 2);

    ui->scrollAreaWidgetContents->setLayout(usersLayout);

    connect(ui->pushButton, &QPushButton::clicked,
            this, &MainWindow::onSendButtonClicked);

    connect(ui->pushButton_receiver, &QPushButton::clicked,
            this, &MainWindow::onReceiverButtonClicked);

    connect(userButtonGroup, &QButtonGroup::buttonClicked,
            this, &MainWindow::onUserButoonClicked);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::setChatClient(ChatClient *chatClient){
    client = chatClient;

    connect(client, &ChatClient::connection_error, this, &MainWindow::onConnectionError);
    connect(client, &ChatClient::message_received, this, &MainWindow::onMessageReceived);
    connect(client, &ChatClient::history_handler,  this, &MainWindow::onHistoryHandler);
}

void MainWindow::onSendButtonClicked(){
    QString message = ui->lineEdit->text();

    if(!message.isEmpty()){
        client->send_message(message.toStdString());

        ui->textBrowser->append("You: " + message);
        ui->lineEdit->clear();
    }
}

void MainWindow::onHistoryHandler(const std::string &history){
    ui->textBrowser->clear();
    ui->textBrowser->append(QString::fromStdString(history));
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
    addNewUserButton(new_receiver);
}

void MainWindow::onUserButoonClicked(QAbstractButton *button){
    currentReceiver = button->text();
    ui->lineEdit_receiver->setText(currentReceiver);
    client->set_new_receiver(currentReceiver.toStdString());
    client->get_history(currentReceiver.toStdString());
}

void MainWindow::addNewUserButton(const QString &username){
    for(auto button : userButtonGroup->buttons()){
        if(button->text() == username) return;
    }

    QPushButton *userButton = new QPushButton(username, ui->scrollAreaWidgetContents);
    userButton->setCheckable(true);
    userButton->setStyleSheet("QPushButton { text-align: left; padding: 5px; }"
                              "QPushButton:checked { background-color: #d0d0d0; }");

    userButtonGroup->addButton(userButton);
    usersLayout->addWidget(userButton);
}

void MainWindow::clearUserButtons(){
    for(auto button : userButtonGroup->buttons()){
        usersLayout->removeWidget(button);
        userButtonGroup->removeButton(button);
        delete button;
    }
}
