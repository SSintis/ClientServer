/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QLineEdit *receiverEdit;

    void setupUi(QWidget *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName("LoginWindow");
        LoginWindow->setEnabled(true);
        LoginWindow->resize(730, 410);
        LoginWindow->setMouseTracking(true);
        usernameEdit = new QLineEdit(LoginWindow);
        usernameEdit->setObjectName("usernameEdit");
        usernameEdit->setGeometry(QRect(290, 170, 142, 26));
        passwordEdit = new QLineEdit(LoginWindow);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setGeometry(QRect(290, 200, 142, 26));
        passwordEdit->setEchoMode(QLineEdit::Password);
        loginButton = new QPushButton(LoginWindow);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(320, 260, 80, 26));
        loginButton->setContextMenuPolicy(Qt::NoContextMenu);
        loginButton->setLayoutDirection(Qt::LeftToRight);
        receiverEdit = new QLineEdit(LoginWindow);
        receiverEdit->setObjectName("receiverEdit");
        receiverEdit->setGeometry(QRect(290, 230, 142, 26));
        receiverEdit->setEchoMode(QLineEdit::Normal);

        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QWidget *LoginWindow)
    {
        LoginWindow->setWindowTitle(QCoreApplication::translate("LoginWindow", "\320\222\321\205\320\276\320\264 \320\262 \321\201\320\270\321\201\321\202\320\265\320\274\321\203", nullptr));
        usernameEdit->setPlaceholderText(QCoreApplication::translate("LoginWindow", "\320\233\320\276\320\263\320\270\320\275", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("LoginWindow", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginWindow", "\320\222\320\276\320\271\321\202\320\270", nullptr));
        receiverEdit->setPlaceholderText(QCoreApplication::translate("LoginWindow", "\320\237\320\276\320\273\321\203\321\207\320\260\321\202\320\265\320\273\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
