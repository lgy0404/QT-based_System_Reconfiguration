#include "login1.h"
#include "widget.h"
#include "ui_login1.h"
#include <QString>
#include <QMessageBox>

#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>


login1::login1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login1)
{
    ui->setupUi(this);
    checkAndCreateUsersFile();
}

login1::~login1()
{
    delete ui;
}

void login1::checkAndCreateUsersFile() {
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QFile usersFile(desktopPath + "/users.txt");

    if (!usersFile.exists()) {
        // 默认用户
        QStringList defaultUsers = {
            "001,superadmin1,789,19852126666,测试中心,超级管理员",
            "002,admin1,123,19852127777,测试中心,普通管理员",
            "003,admin2,456,19852128888,研发中心,普通管理员"
        };

        if (usersFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&usersFile);
            // 移除了向 usersFile 中写入表头的代码
            for (const QString &user : defaultUsers) {
                out << user << "\n";
            }
            usersFile.close();
        }
    }
}

bool login1::validateCredentials(const QString &username, const QString &password) {
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QFile usersFile(desktopPath + "/users.txt");

    if (usersFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&usersFile);
        QString line = in.readLine(); // 读取并忽略第一行（标题行）

        while (!in.atEnd()) {
            line = in.readLine();
            QStringList columns = line.split(',');
            if (columns[1] == username && columns[2] == password) {
                usersFile.close();
                return true;
            }
        }
        usersFile.close();
    }
    return false;
}

void login1::on_loginButton_clicked()
{
    QString userName = ui->userEdit->text();
    QString passwd = ui->passwordEdit->text();

   if (validateCredentials(userName, passwd))
    {
        Widget *wi = new Widget;
        this->close();
        wi->show();

        QMessageBox::information(this, "提示", "您已成功登录配网设备数据采集系统");
    }
    else
    {
        QMessageBox::warning(this, "错误", "用户名或密码错误，请重新输入，如有疑问请联系超级管理员");
    }
}

void login1::keyPressEvent(QKeyEvent *event)//回车热键
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        on_loginButton_clicked();
    }
}
