#include "widget.h"
#include "ui_widget.h"
#include "login.h"
#include <QString>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_loginButton_clicked()
{
    QString userName = ui->userEdit->text();
    QString passwd = ui->passwordEdit->text();
   // qDebug("ZJU log in");//在debug时输出"ZJU log in"
   if (userName == "admin" && passwd == "123")
   {
   Form *ct1 = new Form;    //新建一个Form对象
  // ct->setGeometry(this->geometry()); //获取当前窗口的x与y的数据，并将其设置为新界面的x与y
   ct1->show();
   }

}
