#include "mainwindow.h"
#include "LoginWindow.h"

#include <QApplication>
#include <QStackedWidget>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    QStackedWidget stack;

    LoginWindow login;
    MainWindow mainWindow;

    login.setMinimumSize(730, 410);
    mainWindow.setMinimumSize(730, 410);

    stack.addWidget(&login);
    stack.addWidget(&mainWindow);

    QObject::connect(&login, &LoginWindow::loginSuccessful,
                    [&](const QString &username){
                        stack.setCurrentIndex(1);
                    });

    stack.show();
    return a.exec();
}
