/********************************************************************************
** Form generated from reading UI file 'login1.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN1_H
#define UI_LOGIN1_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login1
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *userlogo;
    QSpacerItem *horizontalSpacer;
    QLineEdit *userEdit;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *passwordlogo;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *passwordEdit;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *loginButton;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QWidget *login1)
    {
        if (login1->objectName().isEmpty())
            login1->setObjectName(QStringLiteral("login1"));
        login1->resize(428, 223);
        widget = new QWidget(login1);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, 0, 420, 218));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_3 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 1, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        userlogo = new QLabel(widget);
        userlogo->setObjectName(QStringLiteral("userlogo"));
        userlogo->setMinimumSize(QSize(40, 40));
        userlogo->setMaximumSize(QSize(40, 40));
        userlogo->setStyleSheet(QStringLiteral("border-image: url(:/user.png);"));

        horizontalLayout_3->addWidget(userlogo);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        userEdit = new QLineEdit(widget);
        userEdit->setObjectName(QStringLiteral("userEdit"));
        userEdit->setMinimumSize(QSize(300, 40));

        horizontalLayout_3->addWidget(userEdit);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        passwordlogo = new QLabel(widget);
        passwordlogo->setObjectName(QStringLiteral("passwordlogo"));
        passwordlogo->setMinimumSize(QSize(40, 40));
        passwordlogo->setMaximumSize(QSize(40, 40));
        passwordlogo->setStyleSheet(QStringLiteral("border-image: url(:/password.png);"));

        horizontalLayout_2->addWidget(passwordlogo);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        passwordEdit = new QLineEdit(widget);
        passwordEdit->setObjectName(QStringLiteral("passwordEdit"));
        passwordEdit->setMinimumSize(QSize(300, 40));
        passwordEdit->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(passwordEdit);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_7 = new QSpacerItem(48, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);

        loginButton = new QPushButton(widget);
        loginButton->setObjectName(QStringLiteral("loginButton"));
        loginButton->setMinimumSize(QSize(160, 40));
        loginButton->setMaximumSize(QSize(161, 40));
        QFont font;
        font.setFamily(QStringLiteral("Times New Roman"));
        font.setPointSize(15);
        loginButton->setFont(font);

        horizontalLayout->addWidget(loginButton);

        horizontalSpacer_4 = new QSpacerItem(48, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 1, 1, 1, 2);

        horizontalSpacer_5 = new QSpacerItem(17, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 1, 3, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_4, 2, 2, 1, 1);


        retranslateUi(login1);

        QMetaObject::connectSlotsByName(login1);
    } // setupUi

    void retranslateUi(QWidget *login1)
    {
        login1->setWindowTitle(QApplication::translate("login1", "\351\205\215\347\275\221\350\256\276\345\244\207\346\225\260\346\215\256\351\207\207\351\233\206\347\263\273\347\273\237-\347\231\273\345\275\225\347\225\214\351\235\242", Q_NULLPTR));
        userlogo->setText(QString());
        passwordlogo->setText(QString());
        loginButton->setText(QApplication::translate("login1", "\347\231\273\345\275\225", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class login1: public Ui_login1 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN1_H
