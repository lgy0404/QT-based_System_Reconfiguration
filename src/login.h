#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>


namespace Ui {
class LOGIN;
}

class LOGIN+ : public QWidget
{
    Q_OBJECT

public:
    explicit LOGIN(QWidget *parent = 0);
    ~LOGIN();

private slots:
    void on_loginButton_clicked();

private:
    Ui::LOGIN *ui;
};

#endif // WIDGET_H
