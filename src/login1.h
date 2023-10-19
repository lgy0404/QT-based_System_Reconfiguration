#ifndef LOGIN1_H
#define LOGIN1_H

#include <QWidget>

namespace Ui {
class login1;
}

class login1 : public QWidget
{
    Q_OBJECT

public:
    explicit login1(QWidget *parent = 0);
    ~login1();

private slots:
    void on_loginButton_clicked();

private:
    Ui::login1 *ui;
    void keyPressEvent(QKeyEvent *event);
    void checkAndCreateUsersFile();
    bool validateCredentials(const QString &username, const QString &password);
};

#endif // LOGIN1_H
