#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QMouseEvent>
#include <QTableWidget>
#include <QWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QVBoxLayout>
#include <QJsonObject>
//Ymodem部分
#include "YmodemFileTransmit.h"
#include "YmodemFileReceive.h"

//md5校验部分
#include "md5check.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:


    explicit Widget(QWidget *parent = 0);
    ~Widget();
//    QSerialPort *serialPort;

    // 设置QTableWidget属性
    void setupTableWidget();

    void addRowToTableWidget(QTableWidget* tableWidget); // 声明添加一行到表格部件的函数
    void deleteRowFromTableWidget(QTableWidget* tableWidget); // 声明从表格部件中删除一行的函数

  // QJsonObject createSlaveJsonObj() ;
    QJsonObject createMasterJsonObj(int deviceNumber) ;
    void fillSlaveConfiguration();
    void fillSlaveData(const QString& slaveKey, const QString& masterKey1, const QString& masterKey2);
   // void removeUnnecessaryKeys(const QString& slave, const QString& master1, const QString& master2);
   // void updateSlaveKeys(const QString& slave);
    void updateSensorFormat();
    void updateModeKey();
    void addBaseConfigToSlaveKeys();
    void addInternetModeToConfig();//添加网口模式子键
    void addUartBaseConfigToConfig();
    QJsonObject removeEmptyStringsFromArrays(QJsonObject &cabinetsObj);
    bool isComboBoxValueSlave(QComboBox *comboBox);
    void generateSlaveMap(QJsonObject &config); //生成"slavemap"
    void deleteEmptyKeys();
    void visualizeSlavemap();






private slots:
 // 串口工具部分
    void on_openBt_clicked();

    void on_closeBt_clicked();

    void serialPortReadyRead_slot();


    void on_sendBt_clicked();

    void on_clearBt_clicked();


//    void on_disposeBt_clicked();

    void mousePressEvent(QMouseEvent *event) override//实现窗口的拖动逻辑
       {
           if (event->button() == Qt::LeftButton) {
               m_offset = event->globalPos() - frameGeometry().topLeft(); // 记录鼠标的初始位置和窗口的初始位置
           }
       }

       void mouseMoveEvent(QMouseEvent *event) override
       {
           if (event->buttons() & Qt::LeftButton) {
               QPoint new_pos = event->globalPos() - m_offset; // 计算鼠标的偏移量
               move(new_pos); // 将窗口的位置设置为初始位置加上偏移量
           }
       }


       void on_syscloseBt_clicked();

       void on_helpBt_clicked();

       void onComboBoxCurrentIndexChanged(const QString &text);//下拉框显示柜型的槽函数


       void on_fullscrcloseBt_clicked();

       void on_addBt_4851_master_c1_clicked();//4581传感器增删除部分的槽函数定义
       void on_deleteBt_4851_master_c1_clicked();
       void on_addBt_4851_master_c2_clicked();
       void on_deleteBt_4851_master_c2_clicked();
       void on_addBt_4851_master_c3_clicked();
       void on_deleteBt_4851_master_c3_clicked();
       void on_addBt_4851_master_c4_clicked();
       void on_deleteBt_4851_master_c4_clicked();
       void on_addBt_4851_master_c5_clicked();
       void on_deleteBt_4851_master_c5_clicked();
       void on_addBt_4851_master_c6_clicked();
       void on_deleteBt_4851_master_c6_clicked();
       void on_addBt_4851_master_c7_clicked();
       void on_deleteBt_4851_master_c7_clicked();
       void on_addBt_4851_master_c8_clicked();
       void on_deleteBt_4851_master_c8_clicked();
       void on_addBt_4852_master_c1_clicked();//4582传感器增删除部分的槽函数定义
       void on_deleteBt_4852_master_c1_clicked();
       void on_addBt_4852_master_c2_clicked();
       void on_deleteBt_4852_master_c2_clicked();
       void on_addBt_4852_master_c3_clicked();
       void on_deleteBt_4852_master_c3_clicked();
       void on_addBt_4852_master_c4_clicked();
       void on_deleteBt_4852_master_c4_clicked();
       void on_addBt_4852_master_c5_clicked();
       void on_deleteBt_4852_master_c5_clicked();
       void on_addBt_4852_master_c6_clicked();
       void on_deleteBt_4852_master_c6_clicked();
       void on_addBt_4852_master_c7_clicked();
       void on_deleteBt_4852_master_c7_clicked();
       void on_addBt_4852_master_c8_clicked();
       void on_deleteBt_4852_master_c8_clicked();
       void on_addBt_4853_master_c1_clicked();//4583传感器增删除部分的槽函数定义
       void on_deleteBt_4853_master_c1_clicked();
       void on_addBt_4853_master_c2_clicked();
       void on_deleteBt_4853_master_c2_clicked();
       void on_addBt_4853_master_c3_clicked();
       void on_deleteBt_4853_master_c3_clicked();
       void on_addBt_4853_master_c4_clicked();
       void on_deleteBt_4853_master_c4_clicked();
       void on_addBt_4853_master_c5_clicked();
       void on_deleteBt_4853_master_c5_clicked();
       void on_addBt_4853_master_c6_clicked();
       void on_deleteBt_4853_master_c6_clicked();
       void on_addBt_4853_master_c7_clicked();
       void on_deleteBt_4853_master_c7_clicked();
       void on_addBt_4853_master_c8_clicked();
       void on_deleteBt_4853_master_c8_clicked();


       void on_disposeBt_clicked();

       void on_fullscrBt_clicked();

       void on_addUserButton_clicked();
       void on_deleteUserButton_clicked();

       void onUserTableItemChanged(QTableWidgetItem *item);

       void onComboBoxIndexChanged();

       void updateDateTime();

       void saveConfigToFile();

       //Ymodem部分
       void on_comButton_clicked();
       void on_transmitBrowse_clicked();
       void on_receiveBrowse_clicked();
       void on_transmitButton_clicked();
       void on_receiveButton_clicked();
       void transmitProgress(int progress);
       void receiveProgress(int progress);
       void transmitStatus(YmodemFileTransmit::Status status);
       void receiveStatus(YmodemFileReceive::Status status);

       //MD5校验部分
       void on_MD5Browse_clicked();

       void on_MD5Button_clicked();

       void on_MD5clear_clicked();



private:

    Ui::Widget *ui;

    QPoint m_offset;

    QMap<QString, QPixmap> m_images;

    QFile usersFile;
    void loadUsers();
    void saveUsers();

    void checkFileExistence();

    QTimer *fileWatcherTimer;

    QTimer *timer;//系统时间

    QJsonObject config;//从机配置

    //Ymodem部分
    QSerialPort *serialPort;
    YmodemFileTransmit *ymodemFileTransmit;
    YmodemFileReceive *ymodemFileReceive;

    bool transmitButtonStatus;
    bool receiveButtonStatus;

    //MD5校验部分
    MD5check *m_md5check;


};

#endif // WIDGET_H
