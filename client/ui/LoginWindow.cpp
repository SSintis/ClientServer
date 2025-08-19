#include "LoginWindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent), ui(new Ui::LoginWindow){
    ui->setupUi(this);
}

LoginWindow::~LoginWindow(){
    delete ui;
}

void LoginWindow::on_loginButton_clicked(){
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    QString receiver = ui->receiverEdit->text();

    emit loginSuccessful(username, password, receiver);
}
