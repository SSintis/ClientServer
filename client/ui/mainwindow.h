#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <string>
#include "../include/ChatClient.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setChatClient(ChatClient* chatClient);
    void addNewUserButton(const QString &username);
    void clearUserButtons();

private slots:
    void onSendButtonClicked();
    void onReceiverButtonClicked();
    void onUserButoonClicked(QAbstractButton *button);
    void onMessageReceived(const QString &sender, const QString &message);
    void onConnectionError(const QString &error);
    void onHistoryHandler(const std::string &history);

private:
    Ui::MainWindow *ui;
    ChatClient *client;
    QButtonGroup *userButtonGroup;
    QVBoxLayout *usersLayout;
    QString currentReceiver;
};
#endif // MAINWINDOW_H
