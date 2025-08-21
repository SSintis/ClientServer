#include "ui/mainwindow.h"
#include "ui/LoginWindow.h"
#include "include/ChatClient.hpp"

#include <QApplication>
#include <QStackedWidget>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    QStackedWidget stack;

    LoginWindow login;
    MainWindow mainWindow;
    ChatClient chatClient;  

    login.setMinimumSize(730, 410);
    mainWindow.setMinimumSize(730, 410);

    stack.addWidget(&login);
    stack.addWidget(&mainWindow);

    QObject::connect(&login, &LoginWindow::loginSuccessful,
                    [&](const QString &username, const QString &password, const QString &receiver){
                      Auth::AuthData authData;
                      authData.login = username.toStdString();
                      authData.password = password.toStdString();
                      authData.receiver = receiver.toStdString();

                      if(chatClient.connect_to_server("10.28.188.60", 3425, authData)){
                        mainWindow.setChatClient(&chatClient);
                        stack.setCurrentIndex(1);
                      }
                    });

    stack.show();
    return a.exec();
}
