#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private slots:
    void onSendButtonClicked();
    void onReceiverButtonClicked();
    void onMessageReceived(const QString &sender, const QString &message);
    void onConnectionError(const QString &error);

private:
    Ui::MainWindow *ui;
    ChatClient *client;
    QButtonGroup *userButtonGroup;
    QString currentReceiver;
};
#endif // MAINWINDOW_H
