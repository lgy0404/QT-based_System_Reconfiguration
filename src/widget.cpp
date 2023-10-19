#include "widget.h"
#include "ui_widget.h"
#include "customtabstyle.h"
#include <QSerialPortInfo>   //串口信息类
#include <QMessageBox>
#include <QString>

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QScreen>
#include <QDesktopWidget>

#include <QPainter> // 包含QPainter类
#include <QProxyStyle> // 包含QProxyStyle类

#include <QTimer>
#include <QInputDialog>

#include <QFontDatabase>

#include <QJsonDocument>

//Ymodem部分
#include <QMessageBox>
#include <QFileDialog>
#include <QSerialPortInfo>

//MD5校验部分
#include "md5check.h"



//构造函数
Widget::Widget(QWidget *parent) :QWidget(parent),ui(new Ui::Widget),  serialPort(new QSerialPort),
    ymodemFileTransmit(new YmodemFileTransmit),
    ymodemFileReceive(new YmodemFileReceive)
{
    //Ymodem部分
    transmitButtonStatus = false;
    receiveButtonStatus  = false;

    ui->setupUi(this);


      QSerialPortInfo serialPortInfo;

      foreach(serialPortInfo, QSerialPortInfo::availablePorts())
      {
          ui->comPort->addItem(serialPortInfo.portName());
      }

      serialPort->setPortName("COM1");
      serialPort->setBaudRate(115200);
      serialPort->setDataBits(QSerialPort::Data8);
      serialPort->setStopBits(QSerialPort::OneStop);
      serialPort->setParity(QSerialPort::NoParity);
      serialPort->setFlowControl(QSerialPort::NoFlowControl);

      connect(ymodemFileTransmit, SIGNAL(transmitProgress(int)), this, SLOT(transmitProgress(int)));
      connect(ymodemFileReceive, SIGNAL(receiveProgress(int)), this, SLOT(receiveProgress(int)));
      connect(ymodemFileTransmit, SIGNAL(transmitStatus(YmodemFileTransmit::Status)), this, SLOT(transmitStatus(YmodemFileTransmit::Status)));
      connect(ymodemFileReceive, SIGNAL(receiveStatus(YmodemFileReceive::Status)), this, SLOT(receiveStatus(YmodemFileReceive::Status)));

      //md5校验部分
      m_md5check = nullptr;
      connect(ui->MD5Browse, &QPushButton::clicked, this, &Widget::on_MD5Browse_clicked);
      connect(ui->MD5Button, &QPushButton::clicked, this, &Widget::on_MD5Button_clicked);



    // 定制tab样式，设置tab的位置
    ui->tabWidget->setTabPosition(QTabWidget::West);
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle); // 设置tab的样式
    ui->tabWidget->setStyleSheet("QTabWidget::pane{ \
            border: 1px solid #7f7f7f;\
        }");

    //定制窗体样式
    this->setStyleSheet("background-color:rgb(240, 240, 240);");
    QRect screenGeometry = qApp->screens().at(0)->geometry();// 获取主显示器的大小
    int x = (screenGeometry.width() - width()) / 2;// 计算窗口的左上角位置，使其在屏幕中央显示
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);


    //“串口工具”模块功能实现，自动搜索可用串口并在UI中进行显示
    QStringList serialNamePort;
    serialPort = new QSerialPort(this);
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(serialPortReadyRead_slot()));
    foreach(const QSerialPortInfo &info ,QSerialPortInfo::availablePorts())
    {
        serialNamePort<<info.portName();
    }
    ui->serialCb->addItems(serialNamePort);


    //“485X配置”模块功能实现（主机模式）
    //柜型图片显示部分
    connect(ui->Cabinet_comboBox_4851_master_c1, SIGNAL(currentIndexChanged(QString)), this, SLOT(onComboBoxCurrentIndexChanged(QString)));    // 柜型图片显示，将QComboBox的currentIndexChanged信号连接到槽
    QPixmap cbbrCabinetPixmap(":/images/696CBBR-close-close.png");// Initialize the images map
    QPixmap cbCabinetPixmap(":/images/696CB-close-close.png");
    QPixmap beCabinetPixmap(":/images/BE-open.png");
    QPixmap cCabinetPixmap(":/images/C-open.png");
    QPixmap dCabinetPixmap(":/images/D.png");
    QPixmap deCabinetPixmap(":/images/DE-openPE.png");
    QPixmap fCabinetPixmap(":/images/F-open.png");
    QPixmap mCabinetPixmap(":/images/M.png");
    QPixmap ptCabinetPixmap(":/images/PT.png");
    QPixmap ptc500CabinetPixmap(":/images/PTC500-open.png");
    QPixmap ptc600CabinetPixmap(":/images/PTC600-open.png");
    QPixmap slCabinetPixmap(":/images/SL-close.png");
    QPixmap sv25sv20CabinetPixmap(":/images/SV25SV20-open.png");
    QPixmap svCabinetPixmap(":/images/SV-open-open.png");
    QPixmap v25v20CabinetPixmap(":/images/V25V20-open-open.png");
    QPixmap vCabinetPixmap(":/images/V-open-close.png");

    if (cCabinetPixmap.isNull() || dCabinetPixmap.isNull() || vCabinetPixmap.isNull()) {// 检查柜型图片是否加载成功
        qDebug() << "Image(s) not loaded!";
    } else {
        m_images.insert("696CBBR柜", cbbrCabinetPixmap);
        m_images.insert("696CB柜", cbCabinetPixmap);
        m_images.insert("BE柜", beCabinetPixmap);
        m_images.insert("C柜", cCabinetPixmap);
        m_images.insert("D柜", dCabinetPixmap);
        m_images.insert("DE柜", deCabinetPixmap);
        m_images.insert("F柜", fCabinetPixmap);
        m_images.insert("M柜", mCabinetPixmap);
        m_images.insert("PT柜", ptCabinetPixmap);
        m_images.insert("PTC500柜", ptc500CabinetPixmap);
        m_images.insert("PTC600柜", ptc600CabinetPixmap);
        m_images.insert("SL柜", slCabinetPixmap);
        m_images.insert("SV25SV20柜", sv25sv20CabinetPixmap);
        m_images.insert("SV柜", svCabinetPixmap);
        m_images.insert("V25V20柜",v25v20CabinetPixmap);
        m_images.insert("V柜", vCabinetPixmap);
    }

    //设置默认传感器类型
    // 字符串列表用于存储下拉菜单中的传感器类型
    QStringList sensorTypes;
    sensorTypes << "TEV 和超声波（断路器）" << "断路器触头温度" << "内部气压" << "分合闸时间" << "动作电流" << "TEV 和超声波（母线）" << "母线与断路器连接处温度" << "绝缘电阻" << "TEV 和超声波（出线电缆）" << "电缆接头温度";

    // 循环遍历每个表格部件（c1到c8）
    for (int mode = 4851; mode <= 4853; ++mode) {
        for (int c = 1; c <= 8; c++) {
            QTableWidget *table = findChild<QTableWidget*>("tableWidget_" + QString::number(mode) + "_master_c" + QString::number(c));
            if (!table) {
                continue; // 如果未找到表格部件，则跳过
            }

            // 为表格中的每个单元格设置下拉菜单
            int rowCount = table->rowCount();
            int colCount = table->columnCount();
            for (int row = 0; row < rowCount; ++row) {
                for (int col = 0; col < colCount; ++col) {
                    QComboBox *comboBox = new QComboBox();
                    comboBox->addItems(sensorTypes);
                    table->setCellWidget(row, col, comboBox);
                }
            }

            // 为每个单元格设置默认传感器类型
            static_cast<QComboBox*>(table->cellWidget(1, 0))->setCurrentIndex(1);
            static_cast<QComboBox*>(table->cellWidget(2, 0))->setCurrentIndex(2);
            static_cast<QComboBox*>(table->cellWidget(3, 0))->setCurrentIndex(3);
            static_cast<QComboBox*>(table->cellWidget(4, 0))->setCurrentIndex(4);
            static_cast<QComboBox*>(table->cellWidget(5, 0))->setCurrentIndex(5);
            static_cast<QComboBox*>(table->cellWidget(6, 0))->setCurrentIndex(6);
            static_cast<QComboBox*>(table->cellWidget(7, 0))->setCurrentIndex(7);
        }

    setupTableWidget();//设置列宽

    //加载485主机模式下机柜1-8的图片，并实现自动切换
    QList<QPair<QComboBox*, QLabel*>> comboBoxLabelPairs = {
        { ui->Cabinet_comboBox_4851_master_c1, ui->picture_lable_4851_master_c1 },
        { ui->Cabinet_comboBox_4851_master_c2, ui->picture_lable_4851_master_c2 },
        { ui->Cabinet_comboBox_4851_master_c3, ui->picture_lable_4851_master_c3 },
        { ui->Cabinet_comboBox_4851_master_c4, ui->picture_lable_4851_master_c4 },
        { ui->Cabinet_comboBox_4851_master_c5, ui->picture_lable_4851_master_c5 },
        { ui->Cabinet_comboBox_4851_master_c6, ui->picture_lable_4851_master_c6 },
        { ui->Cabinet_comboBox_4851_master_c7, ui->picture_lable_4851_master_c7 },
        { ui->Cabinet_comboBox_4851_master_c8, ui->picture_lable_4851_master_c8 },
        { ui->Cabinet_comboBox_4852_master_c1, ui->picture_lable_4852_master_c1 },
        { ui->Cabinet_comboBox_4852_master_c2, ui->picture_lable_4852_master_c2 },
        { ui->Cabinet_comboBox_4852_master_c3, ui->picture_lable_4852_master_c3 },
        { ui->Cabinet_comboBox_4852_master_c4, ui->picture_lable_4852_master_c4 },
        { ui->Cabinet_comboBox_4852_master_c5, ui->picture_lable_4852_master_c5 },
        { ui->Cabinet_comboBox_4852_master_c6, ui->picture_lable_4852_master_c6 },
        { ui->Cabinet_comboBox_4852_master_c7, ui->picture_lable_4852_master_c7 },
        { ui->Cabinet_comboBox_4852_master_c8, ui->picture_lable_4852_master_c8 },
        { ui->Cabinet_comboBox_4853_master_c1, ui->picture_lable_4853_master_c1 },
        { ui->Cabinet_comboBox_4853_master_c2, ui->picture_lable_4853_master_c2 },
        { ui->Cabinet_comboBox_4853_master_c3, ui->picture_lable_4853_master_c3 },
        { ui->Cabinet_comboBox_4853_master_c4, ui->picture_lable_4853_master_c4 },
        { ui->Cabinet_comboBox_4853_master_c5, ui->picture_lable_4853_master_c5 },
        { ui->Cabinet_comboBox_4853_master_c6, ui->picture_lable_4853_master_c6 },
        { ui->Cabinet_comboBox_4853_master_c7, ui->picture_lable_4853_master_c7 },
        { ui->Cabinet_comboBox_4853_master_c8, ui->picture_lable_4853_master_c8 }

    };

    for (auto pair : comboBoxLabelPairs) {
        // 触发comboBox的currentIndexChanged信号
        QMetaObject::invokeMethod(pair.first, "currentIndexChanged", Qt::QueuedConnection, Q_ARG(QString, pair.first->currentText()));
    }

    for (auto pair : comboBoxLabelPairs) {
        connect(pair.first, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), [this, pair](){
            QPixmap image = m_images.value(pair.first->currentText());
            pair.second->setPixmap(image);
        });
    }
}


    // “UART串口配置”模块功能实现，设置 radioButton_uart_baudrate_9600 为默认选中
    ui->radioButton_uart_baudrate_9600->setChecked(true);


    //“系统设置”模块功能实现
    ui->userTableWidget->setColumnCount(6);
    ui->userTableWidget->setHorizontalHeaderLabels({"ID", "用户名", "密码", "手机号", "部门", "账户类型"});

    // 设置每一列的列宽，并设置单元格文字居中显示
    for (int column = 0; column < ui->userTableWidget->columnCount(); ++column) {
        if (column == 0) { // ID列
            ui->userTableWidget->setColumnWidth(column, 100);
        } else {
            ui->userTableWidget->setColumnWidth(column, 232);
        }
    }

    //用户信息保存
    usersFile.setFileName(QDir::homePath() + "/Desktop/users.txt");
    loadUsers();

    // 设置每个单元格的文字居中显示
    ui->userTableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->userTableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    connect(ui->userTableWidget, &QTableWidget::itemChanged, this, &Widget::onUserTableItemChanged);

    for (int row = 0; row < ui->userTableWidget->rowCount(); ++row) {
        // 设置每个单元格的文字居中显示
        for (int column = 0; column < ui->userTableWidget->columnCount(); ++column) {
            QTableWidgetItem *item = ui->userTableWidget->item(row, column);
            if (item) {
                item->setTextAlignment(Qt::AlignCenter);
            }
        }

        QComboBox *departmentComboBox = qobject_cast<QComboBox *>(ui->userTableWidget->cellWidget(row, 4));
        QComboBox *accountTypeComboBox = qobject_cast<QComboBox *>(ui->userTableWidget->cellWidget(row, 5));

        if (departmentComboBox) {
            connect(departmentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::onComboBoxIndexChanged);
        }
        if (accountTypeComboBox) {
            connect(accountTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::onComboBoxIndexChanged);
        }
    }

    //防恶意攻击机制
     fileWatcherTimer = new QTimer(this);
       connect(fileWatcherTimer, &QTimer::timeout, this, &Widget::checkFileExistence);
       fileWatcherTimer->start(1000); // 每1秒检查一次文件是否存在


     // 实时显示系统时间
     // 加载字体
     int fontId = QFontDatabase::addApplicationFont(":/digital-7.ttf");
     if (fontId != -1) {
         QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
         QFont font(fontFamily, 36, QFont::Bold);

         // 设置字体、颜色和阴影效果
         QString style = QString("font:18px '%1';").arg(fontFamily);
         style += "text-shadow: 2px 2px 2px rgba(0, 0, 0, 0.5);";
         ui->dateTimeLabel->setFont(font);
         ui->dateTimeLabel->setStyleSheet(style);
     }

         timer = new QTimer(this);
         connect(timer, &QTimer::timeout, this, &Widget::updateDateTime);
         timer->start(1000);

         updateDateTime();

         //tab自动切换
         //获取下拉框和TabWidget控件指针
         QComboBox* resmode_comboBox_4851 = ui->resmode_comboBox_4851;
         QTabWidget* tabWidget_4851 = ui->tabWidget_4851;
         QComboBox* resmode_comboBox_4852 = ui->resmode_comboBox_4852;
         QTabWidget* tabWidget_4852 = ui->tabWidget_4852;
         QComboBox* resmode_comboBox_4853 = ui->resmode_comboBox_4853;
         QTabWidget* tabWidget_4853 = ui->tabWidget_4853;

         //连接下拉框的currentIndexChanged信号到槽函数，实现自动切换Tab
         connect(resmode_comboBox_4851, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
             if (resmode_comboBox_4851->currentText() == "slave") {
                 tabWidget_4851->setCurrentWidget(ui->slavetab_4851);
             } else if (resmode_comboBox_4851->currentText() == "master") {
                 tabWidget_4851->setCurrentWidget(ui->mastertab_4851);
             }
         });

         connect(resmode_comboBox_4852, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
             if (resmode_comboBox_4852->currentText() == "slave") {
                 tabWidget_4852->setCurrentWidget(ui->slavetab_4852);
             } else if (resmode_comboBox_4852->currentText() == "master") {
                 tabWidget_4852->setCurrentWidget(ui->mastertab_4852);
             }
         });

         connect(resmode_comboBox_4853, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
             if (resmode_comboBox_4853->currentText() == "slave") {
                 tabWidget_4853->setCurrentWidget(ui->slavetab_4853);
             } else if (resmode_comboBox_4853->currentText() == "master") {
                 tabWidget_4853->setCurrentWidget(ui->mastertab_4853);
             }
         });

         // 在程序初始化时手动调用一次槽函数，以保证当前tab的选择正确
         if (resmode_comboBox_4851->currentText() == "slave") {
             tabWidget_4851->setCurrentWidget(ui->slavetab_4851);
         } else if (resmode_comboBox_4851->currentText() == "master") {
             tabWidget_4851->setCurrentWidget(ui->mastertab_4851);
         }

         if (resmode_comboBox_4852->currentText() == "slave") {
             tabWidget_4852->setCurrentWidget(ui->slavetab_4852);
         } else if (resmode_comboBox_4852->currentText() == "master") {
             tabWidget_4852->setCurrentWidget(ui->mastertab_4852);
         }

         if (resmode_comboBox_4853->currentText() == "slave") {
             tabWidget_4853->setCurrentWidget(ui->slavetab_4853);
         } else if (resmode_comboBox_4853->currentText() == "master") {
         tabWidget_4853->setCurrentWidget(ui->mastertab_4853);
         }

         // 调用可视化函数
         visualizeSlavemap();


}


//析构函数
Widget::~Widget()
{
    delete ui;
    delete m_md5check;
}


//采集器一键配置
void Widget::on_disposeBt_clicked() {

    //配置方案检查，满足"有一路RS-485设置为从机"
    QString resmode1 = ui->resmode_comboBox_4851->currentText();
    QString resmode2 = ui->resmode_comboBox_4852->currentText();
    QString resmode3 = ui->resmode_comboBox_4853->currentText();

    int count = 0;
    if (resmode1 == "slave") count++;
    if (resmode2 == "slave") count++;
    if (resmode3 == "slave") count++;
    if (count != 1) {
          QMessageBox::warning(this, "错误", "请检查配置方案，注意有且仅有一路RS-485设置为从机");
          return;
      }

    //"UART串口配置"模块功能实现
    // 获取被选中的单选按钮的值
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->radioButton_uart_baudrate_2400, 2400);
    buttonGroup->addButton(ui->radioButton_uart_baudrate_4800, 4800);
    buttonGroup->addButton(ui->radioButton_uart_baudrate_9600, 9600);
    buttonGroup->addButton(ui->radioButton_uart_baudrate_115200, 115200);
    buttonGroup->addButton(ui->radioButton_uart_baudrate_38400, 38400);
    int checkedButtonValue = buttonGroup->checkedId();

    addUartBaseConfigToConfig();//调用Uart串口波特率配置的函数

    //"网口配置"模块功能实现
    addInternetModeToConfig();//调用网口模式配置的函数


    //"485X配置"模块功能实现
    //主机模式"mastermap"键生成
    for (int deviceNumber = 4851; deviceNumber <= 4853; deviceNumber++) {
        QJsonObject deviceObj;

        // 添加一个名为“mode”的子键
        QString modeValue;
        switch (deviceNumber) {
            case 4851:
                modeValue = ui->resmode_comboBox_4851->currentText();
                break;
            case 4852:
                modeValue = ui->resmode_comboBox_4852->currentText();
                break;
            case 4853:
                modeValue = ui->resmode_comboBox_4853->currentText();
                break;
        }
        deviceObj["mode"] = modeValue;

        // 如果mode子键的值不为"slave"，则添加"mastermap"子键
        if (modeValue != "slave") {
            deviceObj["mastermap"] = createMasterJsonObj(deviceNumber);
        }


        config[QString::number(deviceNumber)] = deviceObj;

    }

    // 调用generateSlaveMap函数
    generateSlaveMap(config);


    //用于生成"base_config"
    addBaseConfigToSlaveKeys();

    //搜索/创建"ConfigJson"文件夹
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString folderPath = desktopPath + "/disposeJson";
    QDir dir(folderPath);
    if (!dir.exists()) {
        dir.mkpath(folderPath);
    }

    //更新/创建"config.json"文件
    QFile configFile(folderPath + "/config.json");

    if (configFile.open(QFile::WriteOnly)) {
        QJsonDocument configDoc(config);
        configFile.write(configDoc.toJson());
        configFile.close();
        QMessageBox::information(this, "Success", "配置文件已更新");
    } else {
        QMessageBox::critical(this, "Error", "Unable to save config.json.");
        }

    // 调用可视化函数
    visualizeSlavemap();

}

//采集器一键配置按钮中涉及到的槽函数
//"mastermap"键生成
QJsonObject Widget::createMasterJsonObj(int deviceNumber) {

    QJsonObject cabinetsObj;
    for (int i = 1; i <= 8; i++) {
        QTableWidget *tableWidget = findChild<QTableWidget*>(QString("tableWidget_%1_master_c%2").arg(deviceNumber).arg(i));
        if (!tableWidget) {
            continue;
        }
        int rowCount = tableWidget->rowCount(); // 获取当前tableWidget的行数

    QJsonArray cabinetArray; // 修改为QJsonArray
    for (int j = 1; j <= 20; j++) {
        QWidget *cellWidget = nullptr;
        QString sensorType;
        if (j <= rowCount) {
            cellWidget = tableWidget->cellWidget(j - 1, 0);
        }
        if (cellWidget) {
            if (QComboBox *comboBox = qobject_cast<QComboBox *>(cellWidget)) {
                QString chineseType = comboBox->currentText();

                if (chineseType == "TEV 和超声波（断路器）") {
                    sensorType = "TEV_and_ultrasonic_circuit_breaker";
                } else if (chineseType == "断路器触头温度") {
                    sensorType = "circuit_breaker_contact_temperature";
                } else if (chineseType == "内部气压") {
                    sensorType = "internal_air_pressure";
                } else if (chineseType == "分合闸时间") {
                    sensorType = "opening_and_closing_time";
                } else if (chineseType == "动作电流") {
                    sensorType = "operation_current";
                } else if (chineseType == "TEV 和超声波（母线）") {
                    sensorType = "TEV_and_ultrasonic_busbar";
                } else if (chineseType == "母线与断路器连接处温度") {
                    sensorType = "busbar_and_circuit_breaker_connection_temperature";
                } else if (chineseType == "绝缘电阻") {
                    sensorType = "Insulation_Resistance";
                }else if(chineseType == "TEV 和超声波（出线电缆）") {
                    sensorType = "TEV_and_Ultrasonic_Outgoing_Cable";
                } else if (chineseType == "电缆接头温度") {
                    sensorType = "Cable_Joint_Temperature";
                } else {
                    sensorType = "nonesensor";
                }
            } else {
                sensorType = "nonesensor";
            }
        } // 这里添加了一个右括号
        // 只有当sensorType不为nonesensor时，才将其添加到cabinetArray中
        if (sensorType != "nonesensor") {
            cabinetArray.append(sensorType);
        } else {
            break; // 当遇到第一个nonesensor时，跳出循环
        }
          }
    if (!cabinetArray.isEmpty()) {
        cabinetsObj[QString("cabinet%1").arg(i)] = cabinetArray;
    }

      }
   return removeEmptyStringsFromArrays(cabinetsObj);
}

//将变换更新到 "config.json"中
void Widget::saveConfigToFile()
{
    QString configFileName = "config.json";
    QFile configFile(configFileName);

    if (configFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonDocument configDoc(config);
        configFile.write(configDoc.toJson(QJsonDocument::Indented));
        configFile.close();
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Unable to save config file."));
    }
}

//"base_config"键生成
void Widget::addBaseConfigToSlaveKeys() {
    for (const QString& key : config.keys()) {
        if (key.startsWith("485")) {
            QJsonObject keyObject = config[key].toObject();
            QJsonObject baseConfig;

            QString agreetypeKey = QString("agreetype_comboBox_%1").arg(key);
            QString baundrateKey = QString("baundrate_comboBox_%1").arg(key);
            QString checkbitKey = QString("checkbit_comboBox_%1").arg(key);
            QString databitKey = QString("databit_comboBox_%1").arg(key);
            QString stopbitKey = QString("stopbit_comboBox_%1").arg(key);

            QComboBox *agreetypeComboBox = findChild<QComboBox*>(agreetypeKey);
            QComboBox *baundrateComboBox = findChild<QComboBox*>(baundrateKey);
            QComboBox *checkbitComboBox = findChild<QComboBox*>(checkbitKey);
            QComboBox *databitComboBox = findChild<QComboBox*>(databitKey);
            QComboBox *stopbitComboBox = findChild<QComboBox*>(stopbitKey);

            if (agreetypeComboBox && baundrateComboBox && checkbitComboBox && databitComboBox && stopbitComboBox) {
                baseConfig["agreetype"] = agreetypeComboBox->currentText();
                baseConfig["baundrate"] = baundrateComboBox->currentText().toInt();
                baseConfig["checkbit"] = checkbitComboBox->currentText();
                baseConfig["databit"] = databitComboBox->currentText().toInt();
                baseConfig["stopbit"] = stopbitComboBox->currentText().toInt();

                keyObject["base_config"] = baseConfig;
                config[key] = keyObject;
            }
        }
    }

    saveConfigToFile();
}

//网口配置槽函数
void Widget::addInternetModeToConfig() {
    QComboBox *resmodeComboBoxInternet = findChild<QComboBox*>("resmode_comboBox_internet");

    if (resmodeComboBoxInternet) {
        QJsonObject internetObject;
        internetObject["mode"] = resmodeComboBoxInternet->currentText();
        config["internet"] = internetObject;
    }

    saveConfigToFile();
}

//UART串口配置槽函数
void Widget::addUartBaseConfigToConfig() {
    QRadioButton *baudrate2400 = findChild<QRadioButton*>("radioButton_uart_baudrate_2400");
    QRadioButton *baudrate4800 = findChild<QRadioButton*>("radioButton_uart_baudrate_4800");
    QRadioButton *baudrate9600 = findChild<QRadioButton*>("radioButton_uart_baudrate_9600");
    QRadioButton *baudrate19200 = findChild<QRadioButton*>("radioButton_uart_baudrate_19200");
    QRadioButton *baudrate38400 = findChild<QRadioButton*>("radioButton_uart_baudrate_38400");
    QRadioButton *baudrate115200 = findChild<QRadioButton*>("radioButton_uart_baudrate_115200");

    int baudrate = 0;

    if (baudrate2400 && baudrate2400->isChecked()) {
        baudrate = 2400;
    } else if (baudrate4800 && baudrate4800->isChecked()) {
        baudrate = 4800;
    } else if (baudrate9600 && baudrate9600->isChecked()) {
        baudrate = 9600;
    } else if (baudrate19200 && baudrate19200->isChecked()) {
        baudrate = 19200;
    } else if (baudrate38400 && baudrate38400->isChecked()) {
        baudrate = 38400;
    } else if (baudrate115200 && baudrate115200->isChecked()) {
        baudrate = 115200;
    }

    QJsonObject baseConfig;
    baseConfig["baundrate"] = baudrate;

    QJsonObject uartObject;
    uartObject["base_config"] = baseConfig;
    config["uart"] = uartObject;

    saveConfigToFile();
}

//删除数组中的空值
QJsonObject Widget::removeEmptyStringsFromArrays(QJsonObject &cabinetsObj) {
    QJsonObject result;
    QStringList keys = cabinetsObj.keys();

    for (const QString &key : keys) {
        QJsonArray array = cabinetsObj[key].toArray();
        QJsonArray newArray;

        for (const QJsonValue &value : array) {
            if (!value.toString().isEmpty()) {
                newArray.append(value);
            }
        }

        if (!newArray.isEmpty()) {
            result[key] = newArray;
        } else {
            result.remove(key);
        }
    }

    return result;
}

//生成"slavemap"
void Widget::generateSlaveMap(QJsonObject &config) {
    QJsonObject::iterator it;
    int registerAddress = 1;

    QString slaveKey;

    // 查找模式为"slave"的RS-485设备
    for (it = config.begin(); it != config.end(); ++it) {
        QString key = it.key();
        QJsonObject value = it.value().toObject();

        if (key.startsWith("485") && value["mode"].toString() == "slave") {
            slaveKey = key;
            break;
        }
    }

    QJsonObject slaveObject = config[slaveKey].toObject();
    QJsonObject slaveMapObject = slaveObject["slavemap"].toObject();

    for (it = config.begin(); it != config.end(); ++it) {
        QString key = it.key();
        QJsonObject value = it.value().toObject();

        if (key.startsWith("485") && value["mode"].toString() == "master") {
            QJsonObject mastermap = value["mastermap"].toObject();
            QJsonObject slavemap;

            for (auto cabinet = mastermap.begin(); cabinet != mastermap.end(); ++cabinet) {
                QJsonArray sensors = cabinet.value().toArray();
                QJsonObject sensorMap;

                for (int i = 0; i < sensors.size(); ++i) {
                    QString sensorName = sensors[i].toString();
                    int registerCount;

                    if (sensorName == "internal_air_pressure") {
                        registerCount = 3;
                    } else if (sensorName == "TEV_and_ultrasonic_busbar") {
                        registerCount = 30;
                    } else {
                        registerCount = 1;
                    }

                    QJsonArray addressArray;
                    addressArray.append(registerAddress);
                    addressArray.append(registerAddress + registerCount - 1);
                    sensorMap[sensorName] = addressArray;
                    registerAddress += registerCount;
                }

                slavemap[cabinet.key()] = sensorMap;
            }

            QString masterKey = key;
            slaveMapObject[masterKey] = slavemap;
        }
    }

    slaveObject["slavemap"] = slaveMapObject;
    config[slaveKey] = slaveObject;

    saveConfigToFile();
}


//从机方案可视化
void Widget::visualizeSlavemap() {
    // 读取config.json文件
    QDir dir = QDir::home();
    dir.cd("Desktop");
    dir.cd("disposeJson");
    QFile file(dir.filePath("config.json"));

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("无法打开config.json文件");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject root = doc.object();

    // 找到"slave"设备的slavemap对象
    QJsonObject slavemap;
    QString slaveKey;
    for (auto key : root.keys()) {
        if (key.startsWith("485")) {
            QJsonObject deviceObj = root[key].toObject();
            if (deviceObj["mode"].toString() == "slave") {
                slavemap = deviceObj["slavemap"].toObject();
                slaveKey = key;
                break;
            }
        }
    }

    if (slavemap.isEmpty()) {
        qWarning("找不到slavemap");
        return;
    }

    // 判断具体的tab
    QWidget *targetTab = nullptr;
    if (slaveKey == "4851") {
        targetTab = ui->tabWidget_4851->findChild<QWidget *>("slavetab_4851");
    } else if (slaveKey == "4852") {
        targetTab = ui->tabWidget_4852->findChild<QWidget *>("slavetab_4852");
    } else if (slaveKey == "4853") {
        targetTab = ui->tabWidget_4853->findChild<QWidget *>("slavetab_4853");
    } else {
        qWarning("找不到合适的slavetab");
        return;
    }

    // 创建QTableWidget并设置表头
    QTableWidget *tableWidget = new QTableWidget(targetTab);
    QStringList headers;
    headers << "数据通道" << "机柜" << "传感器" << "寄存器地址";
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels(headers);

    // 设置列宽自动调整模式
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



    // 定义一个结构来存储表格数据
    struct TableRowData {
        QString dataChannel;
        QString cabinet;
        QString sensor;
        QString registerAddressesString;
        int registerAddress;
    };

    // 将slavemap数据添加到一个列表中
    QList<TableRowData> tableDataList;
    for (auto dataChannel : slavemap.keys()) {
        QJsonObject cabinetObj = slavemap[dataChannel].toObject();

        for (auto cabinet : cabinetObj.keys()) {
            QJsonObject sensorObj = cabinetObj[cabinet].toObject();

            for (auto sensor : sensorObj.keys()) {
                QJsonArray registerAddresses = sensorObj[sensor].toArray();
                QString registerAddressesString;

                for (int i = 0; i < registerAddresses.size(); ++i) {
                    if (i > 0) {
                        registerAddressesString += ", ";
                    }
                    registerAddressesString += QString::number(registerAddresses[i].toInt());
                }

                TableRowData rowData;
                rowData.dataChannel = dataChannel;
                rowData.cabinet = cabinet;
                rowData.sensor = sensor;
                rowData.registerAddressesString = registerAddressesString;
                rowData.registerAddress = registerAddresses[0].toInt();

                tableDataList.append(rowData);
            }
        }
    }

    // 按寄存器地址对列表进行排序
    std::sort(tableDataList.begin(), tableDataList.end(), [](const TableRowData &a, const TableRowData &b) {
        return a.registerAddress < b.registerAddress;
    });

    // 将排序后的数据添加到表格中
    int row = 0;
    for (const TableRowData &rowData : tableDataList) {
        tableWidget->insertRow(row);

        QTableWidgetItem *item1 = new QTableWidgetItem(rowData.dataChannel);
        QTableWidgetItem *item2 = new QTableWidgetItem(rowData.cabinet);
        QTableWidgetItem *item3 = new QTableWidgetItem(rowData.sensor);
        QTableWidgetItem *item4 = new QTableWidgetItem(rowData.registerAddressesString);

        // 为每个 QTableWidgetItem 设置对齐标志
        item1->setTextAlignment(Qt::AlignCenter);
        item2->setTextAlignment(Qt::AlignCenter);
        item3->setTextAlignment(Qt::AlignCenter);
        item4->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(row, 0, item1);
        tableWidget->setItem(row, 1, item2);
        tableWidget->setItem(row, 2, item3);
        tableWidget->setItem(row, 3, item4);

        row++;
    }


    // 清除目标tab的布局中的所有内容
    QLayout *currentLayout = targetTab->layout();
    if (currentLayout) {
        QLayoutItem *item;
        while ((item = currentLayout->takeAt(0)) != 0) {
            QWidget *widget = item->widget();
            if (widget) {
                delete widget;
            }
            delete item;
        }
    } else {
        QVBoxLayout *newLayout = new QVBoxLayout(targetTab);
        targetTab->setLayout(newLayout);
    }


    // 将QTableWidget添加到目标tab的布局中
    if (QLayout *layout = targetTab->layout()) {
        layout->addWidget(tableWidget);
    } else {
        QVBoxLayout *newLayout = new QVBoxLayout(targetTab);
        newLayout->addWidget(tableWidget);
        targetTab->setLayout(newLayout);
    }

    }


//系统侧边杂七杂八功能槽函数
//"系统帮助"提示
void Widget::on_helpBt_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("系统帮助");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setTextInteractionFlags(Qt::TextSelectableByMouse);
    QString text = "面向配网设备的可重构数据采集系统配置软件v2.5<br>";
    text += "更新时间:2023.4.10<br>";
    text += "开发者:刘广义 QQ:2528379691<br>";
    msgBox.setText(text);
    QLabel *label = msgBox.findChild<QLabel*>();
    label->setAlignment(Qt::AlignCenter);
    msgBox.exec();
}

//窗口调整部分的槽函数
void Widget::on_syscloseBt_clicked()//系统退出槽函数
{
    this->close();
}

void Widget::on_fullscrBt_clicked()//进入全屏槽函数
{
    this->showFullScreen();
}

void Widget::on_fullscrcloseBt_clicked()//退出窗口最大化槽函数
{
    this->showNormal();
}

void Widget::updateDateTime()//显示系统时间
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dateTimeText = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    ui->dateTimeLabel->setText(dateTimeText);
}


//"串口工具"模块槽函数
//打开串口的槽函数
void Widget::on_openBt_clicked()
{
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity checkBits;

    if(ui->baundrateCb->currentText() == "4800"){
        baudRate = QSerialPort::Baud4800;
    }else if(ui->baundrateCb->currentText() == "9600"){
         baudRate = QSerialPort::Baud9600;
    }else if(ui->baundrateCb->currentText() == "115200"){
         baudRate = QSerialPort::Baud115200;
    }

    if(ui->dataCb->currentText() == "5"){
        dataBits = QSerialPort::Data5;
    }else if(ui->dataCb->currentText() == "6"){
        dataBits = QSerialPort::Data6;
    }else if(ui->dataCb->currentText() == "7"){
        dataBits = QSerialPort::Data7;
    }else if(ui->dataCb->currentText() == "8"){
        dataBits = QSerialPort::Data8;
    }

    if(ui->stopCb->currentText() == "1"){
       stopBits = QSerialPort::OneStop;
    }else if(ui->stopCb->currentText() == "1.5"){
       stopBits = QSerialPort::OneAndHalfStop;
    }else if(ui->stopCb->currentText() == "2"){
       stopBits = QSerialPort::TwoStop;
    }

    if(ui->checkCb->currentText() == "none"){
        checkBits = QSerialPort::NoParity;
    }

    serialPort->setPortName(ui->serialCb->currentText());
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setStopBits(stopBits);
    serialPort->setParity(checkBits);

    if(serialPort->open(QIODevice::ReadWrite) == true){
        QMessageBox::information(this,"提示","串口打开成功");
    }else{
        QMessageBox::critical(this,"提示","串口打开失败");
    }

}

//关闭串口槽函数
void Widget::on_closeBt_clicked()
{
    serialPort->close();
}

//读取串口数据
void Widget::serialPortReadyRead_slot()
{
    QString buf;
    buf = QString(serialPort->readAll());
    ui->receiveEdit->appendPlainText(buf);
}

//数据发送槽函数
void Widget::on_sendBt_clicked()
{
    serialPort->write(ui->sendEdit->text().toLocal8Bit().data());
}

//接受框清空槽函数
void Widget::on_clearBt_clicked()
{
    ui->receiveEdit->clear();
}


//485X配置模块槽函数
//柜型图片显示槽函数
void Widget::onComboBoxCurrentIndexChanged(const QString &text)
{
    // Get the selected image from the map
    QPixmap image = m_images.value(text);

    // Set the image to the QLabel
    ui->picture_lable_4851_master_c1->setPixmap(image);//设置4851主机模式柜1的默认图片
}

//设置QTableWidget的列宽为固定值，使其可以将传感器的名称完全显示
void Widget::setupTableWidget()
{
    qDebug() << "执行列宽设置函数";
    // 设置列宽
    ui->tableWidget_4851_master_c1->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4851_master_c2->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4851_master_c3->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4851_master_c4->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4851_master_c5->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4851_master_c6->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4851_master_c7->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4851_master_c8->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4852_master_c1->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4852_master_c2->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4852_master_c3->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4852_master_c4->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4852_master_c5->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4852_master_c6->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4852_master_c7->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4852_master_c8->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4853_master_c1->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4853_master_c2->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4853_master_c3->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4853_master_c4->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4853_master_c5->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4853_master_c6->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4853_master_c7->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
    ui->tableWidget_4853_master_c8->setColumnWidth(0, 180); // 设置第一列的宽度为250像素
}

//添加传感器的函数
void Widget::addRowToTableWidget(QTableWidget* tableWidget)
{
    int rowCount = tableWidget->rowCount();
    tableWidget->insertRow(rowCount);
    QTableWidgetItem* newItem = new QTableWidgetItem(QString("传感器%1").arg(rowCount + 1));
    tableWidget->setVerticalHeaderItem(rowCount, newItem);

    QStringList sensorTypes;
    sensorTypes << "TEV 和超声波（断路器）" << "断路器触头温度" << "内部气压" << "分合闸时间" << "动作电流" << "TEV 和超声波（母线）" << "母线与断路器连接处温度" << "绝缘电阻" << "TEV 和超声波（出线电缆）" << "电缆接头温度";

    QComboBox* comboBox = new QComboBox();
    comboBox->addItems(sensorTypes);
    tableWidget->setCellWidget(rowCount, 0, comboBox);
}

//删除传感器的函数
void Widget::deleteRowFromTableWidget(QTableWidget* tableWidget)
{
    int rowCount = tableWidget->rowCount();
    if (rowCount > 0) {
        tableWidget->removeRow(rowCount - 1);
    }
}

//4851主机模式柜1添加传感器的槽函数
void Widget::on_addBt_4851_master_c1_clicked()
{
    addRowToTableWidget(ui->tableWidget_4851_master_c1);
}

//4851主机模式柜1删除传感器的槽函数
void Widget::on_deleteBt_4851_master_c1_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4851_master_c1);
}

//4851主机模式柜2添加传感器的槽函数
void Widget::on_addBt_4851_master_c2_clicked()
{
    addRowToTableWidget(ui->tableWidget_4851_master_c2);
}

//4851主机模式柜2删除传感器的槽函数
void Widget::on_deleteBt_4851_master_c2_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4851_master_c2);
}

// 4851主机模式柜3添加传感器的槽函数
void Widget::on_addBt_4851_master_c3_clicked()
{
    addRowToTableWidget(ui->tableWidget_4851_master_c3);
}

// 4851主机模式柜3删除传感器的槽函数
void Widget::on_deleteBt_4851_master_c3_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4851_master_c3);
}

// 4851主机模式柜4添加传感器的槽函数
void Widget::on_addBt_4851_master_c4_clicked()
{
    addRowToTableWidget(ui->tableWidget_4851_master_c4);
}

// 4851主机模式柜4删除传感器的槽函数
void Widget::on_deleteBt_4851_master_c4_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4851_master_c4);
}

// 4851主机模式柜5添加传感器的槽函数
void Widget::on_addBt_4851_master_c5_clicked()
{
    addRowToTableWidget(ui->tableWidget_4851_master_c5);
}

// 4851主机模式柜5删除传感器的槽函数
void Widget::on_deleteBt_4851_master_c5_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4851_master_c5);
}

// 4851主机模式柜6添加传感器的槽函数
void Widget::on_addBt_4851_master_c6_clicked()
{
    addRowToTableWidget(ui->tableWidget_4851_master_c6);
}

// 4851主机模式柜6删除传感器的槽函数
void Widget::on_deleteBt_4851_master_c6_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4851_master_c6);
}

// 4851主机模式柜7添加传感器的槽函数
void Widget::on_addBt_4851_master_c7_clicked()
{
    addRowToTableWidget(ui->tableWidget_4851_master_c7);
}

// 4851主机模式柜7删除传感器的槽函数
void Widget::on_deleteBt_4851_master_c7_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4851_master_c7);
}

//4851主机模式柜8添加传感器的槽函数
void Widget::on_addBt_4851_master_c8_clicked()
{
    addRowToTableWidget(ui->tableWidget_4851_master_c8);
}

//4851主机模式柜8删除传感器的槽函数
void Widget::on_deleteBt_4851_master_c8_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4851_master_c8);
}

//4852主机模式柜1添加传感器的槽函数
void Widget::on_addBt_4852_master_c1_clicked()
{
    addRowToTableWidget(ui->tableWidget_4852_master_c1);
}

//4852主机模式柜1删除传感器的槽函数
void Widget::on_deleteBt_4852_master_c1_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4852_master_c1);
}

//4852主机模式柜2添加传感器的槽函数
void Widget::on_addBt_4852_master_c2_clicked()
{
    addRowToTableWidget(ui->tableWidget_4852_master_c2);
}

//4852主机模式柜2删除传感器的槽函数
void Widget::on_deleteBt_4852_master_c2_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4852_master_c2);
}

// 4852主机模式柜3添加传感器的槽函数
void Widget::on_addBt_4852_master_c3_clicked()
{
    addRowToTableWidget(ui->tableWidget_4852_master_c3);
}

// 4852主机模式柜3删除传感器的槽函数
void Widget::on_deleteBt_4852_master_c3_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4852_master_c3);
}

// 4852主机模式柜4添加传感器的槽函数
void Widget::on_addBt_4852_master_c4_clicked()
{
    addRowToTableWidget(ui->tableWidget_4852_master_c4);
}

// 4852主机模式柜4删除传感器的槽函数
void Widget::on_deleteBt_4852_master_c4_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4852_master_c4);
}

// 4852主机模式柜5添加传感器的槽函数
void Widget::on_addBt_4852_master_c5_clicked()
{
    addRowToTableWidget(ui->tableWidget_4852_master_c5);
}

// 4852主机模式柜5删除传感器的槽函数
void Widget::on_deleteBt_4852_master_c5_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4852_master_c5);
}

// 4852主机模式柜6添加传感器的槽函数
void Widget::on_addBt_4852_master_c6_clicked()
{
    addRowToTableWidget(ui->tableWidget_4852_master_c6);
}

// 4852主机模式柜6删除传感器的槽函数
void Widget::on_deleteBt_4852_master_c6_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4852_master_c6);
}

// 4852主机模式柜7添加传感器的槽函数
void Widget::on_addBt_4852_master_c7_clicked()
{
    addRowToTableWidget(ui->tableWidget_4852_master_c7);
}

// 4852主机模式柜7删除传感器的槽函数
void Widget::on_deleteBt_4852_master_c7_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4852_master_c7);
}

//4852主机模式柜8添加传感器的槽函数
void Widget::on_addBt_4852_master_c8_clicked()
{
    addRowToTableWidget(ui->tableWidget_4852_master_c8);
}

//4852主机模式柜8删除传感器的槽函数
void Widget::on_deleteBt_4852_master_c8_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4852_master_c8);
}

//4853主机模式柜1添加传感器的槽函数
void Widget::on_addBt_4853_master_c1_clicked()
{
    addRowToTableWidget(ui->tableWidget_4853_master_c1);
}

//4853主机模式柜1删除传感器的槽函数
void Widget::on_deleteBt_4853_master_c1_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4853_master_c1);
}

//4853主机模式柜2添加传感器的槽函数
void Widget::on_addBt_4853_master_c2_clicked()
{
    addRowToTableWidget(ui->tableWidget_4853_master_c2);
}

//4853主机模式柜2删除传感器的槽函数
void Widget::on_deleteBt_4853_master_c2_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4853_master_c2);
}

// 4853主机模式柜3添加传感器的槽函数
void Widget::on_addBt_4853_master_c3_clicked()
{
    addRowToTableWidget(ui->tableWidget_4853_master_c3);
}

// 4853主机模式柜3删除传感器的槽函数
void Widget::on_deleteBt_4853_master_c3_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4853_master_c3);
}

// 4853主机模式柜4添加传感器的槽函数
void Widget::on_addBt_4853_master_c4_clicked()
{
    addRowToTableWidget(ui->tableWidget_4853_master_c4);
}

// 4853主机模式柜4删除传感器的槽函数
void Widget::on_deleteBt_4853_master_c4_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4853_master_c4);
}

// 4853主机模式柜5添加传感器的槽函数
void Widget::on_addBt_4853_master_c5_clicked()
{
    addRowToTableWidget(ui->tableWidget_4853_master_c5);
}

// 4853主机模式柜5删除传感器的槽函数
void Widget::on_deleteBt_4853_master_c5_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4853_master_c5);
}

// 4853主机模式柜6添加传感器的槽函数
void Widget::on_addBt_4853_master_c6_clicked()
{
    addRowToTableWidget(ui->tableWidget_4853_master_c6);
}

// 4853主机模式柜6删除传感器的槽函数
void Widget::on_deleteBt_4853_master_c6_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4853_master_c6);
}

// 4853主机模式柜7添加传感器的槽函数
void Widget::on_addBt_4853_master_c7_clicked()
{
    addRowToTableWidget(ui->tableWidget_4853_master_c7);
}

// 4853主机模式柜7删除传感器的槽函数
void Widget::on_deleteBt_4853_master_c7_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4853_master_c7);
}

//4853主机模式柜8添加传感器的槽函数
void Widget::on_addBt_4853_master_c8_clicked()
{
    addRowToTableWidget(ui->tableWidget_4853_master_c8);
}

//4853主机模式柜8删除传感器的槽函数
void Widget::on_deleteBt_4853_master_c8_clicked()
{
    deleteRowFromTableWidget(ui->tableWidget_4853_master_c8);
}


//"系统设置"模块槽函数
void Widget::loadUsers() {
    if (usersFile.exists()) {
        usersFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&usersFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList data = line.split(",");
            int row = ui->userTableWidget->rowCount();
            ui->userTableWidget->insertRow(row);
            for (int i = 0; i < data.size(); ++i) {
                QTableWidgetItem *item = new QTableWidgetItem(data[i]);
                ui->userTableWidget->setItem(row, i, item);
                if (i == 4 || i == 5) { // 部门和账户类型
                    QComboBox *comboBox = new QComboBox();
                    if (i == 4) {
                        comboBox->addItem("测试中心");
                        comboBox->addItem("研发中心");
                    } else {
                        comboBox->addItem("普通管理员");
                        comboBox->addItem("超级管理员");
                    }
                    comboBox->setCurrentText(data[i]);
                    ui->userTableWidget->setCellWidget(row, i, comboBox);
                }
            }
        }
        usersFile.close();
    } else {
        usersFile.open(QIODevice::WriteOnly | QIODevice::Text);
        usersFile.close();

        // 默认用户
        QStringList defaultUsers = {
            "001,superadmin1,789,19852126666,测试中心,超级管理员",
            "002,admin1,123,19852127777,测试中心,普通管理员",
            "003,admin2,456,19852128888,研发中心,普通管理员"
        };

for (const QString &user : defaultUsers) {
    QStringList data = user.split(",");
    int row = ui->userTableWidget->rowCount();
    ui->userTableWidget->insertRow(row);
    for (int i = 0; i < data.size();++i) {
        QTableWidgetItem *item = new QTableWidgetItem(data[i]);
        ui->userTableWidget->setItem(row, i, item);
        if (i == 4 || i == 5) { // 部门和账户类型
        QComboBox *comboBox = new QComboBox();
        if (i == 4) {
        comboBox->addItem("测试中心");
        comboBox->addItem("研发中心");
        } else {
        comboBox->addItem("普通管理员");
        comboBox->addItem("超级管理员");
        }
        comboBox->setCurrentText(data[i]);
        ui->userTableWidget->setCellWidget(row, i, comboBox);
        }
        }
        }
        saveUsers();
        }

        }



void Widget::saveUsers() {

qDebug() << "Saving users...";
usersFile.open(QIODevice::WriteOnly | QIODevice::Text);
QTextStream out(&usersFile);
for (int row = 0; row < ui->userTableWidget->rowCount(); ++row) {
QStringList rowData;
for (int col = 0; col < ui->userTableWidget->columnCount(); ++col) {
if (col == 4 || col == 5) {
QComboBox *comboBox = qobject_cast<QComboBox *>(ui->userTableWidget->cellWidget(row, col));
rowData << comboBox->currentText();
} else {
rowData << ui->userTableWidget->item(row, col)->text();
}
}
out << rowData.join(",") << "\n";
}
usersFile.close();
qDebug() << "Users saved.";
}

void Widget::on_addUserButton_clicked()
{
    disconnect(ui->userTableWidget, &QTableWidget::itemChanged, this, &Widget::onUserTableItemChanged);

    int newRow = ui->userTableWidget->rowCount();
    ui->userTableWidget->insertRow(newRow);

    QString id = QString::number(newRow + 1).rightJustified(3, '0');
    QTableWidgetItem *idItem = new QTableWidgetItem(id);
    idItem->setTextAlignment(Qt::AlignCenter);
    ui->userTableWidget->setItem(newRow, 0, idItem);

    for (int column = 1; column < ui->userTableWidget->columnCount() - 2; ++column) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setTextAlignment(Qt::AlignCenter);
        ui->userTableWidget->setItem(newRow, column, item);
    }

    QComboBox *departmentComboBox = new QComboBox();
    departmentComboBox->addItems({"测试中心", "研发中心"});
    connect(departmentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::onComboBoxIndexChanged);
    ui->userTableWidget->setCellWidget(newRow, 4, departmentComboBox);

    QComboBox *accountTypeComboBox = new QComboBox();
    accountTypeComboBox->addItems({"普通管理员", "超级管理员"});
    connect(accountTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::onComboBoxIndexChanged);
    ui->userTableWidget->setCellWidget(newRow, 5, accountTypeComboBox);

    connect(ui->userTableWidget, &QTableWidget::itemChanged, this, &Widget::onUserTableItemChanged);


}



void Widget::on_deleteUserButton_clicked()
{
QList<QTableWidgetItem *> selectedItems = ui->userTableWidget->selectedItems();
if (!selectedItems.isEmpty()) {
ui->userTableWidget->removeRow(selectedItems.first()->row());
saveUsers();
}
}

void Widget::onUserTableItemChanged(QTableWidgetItem *item)
{
    saveUsers();
}

void Widget::onComboBoxIndexChanged()
{
    saveUsers();
}

void Widget::checkFileExistence()
{
    QString filePath = QDir::homePath() + "/Desktop/users.txt";
    QFile file(filePath);

    if (!file.exists()) {
        QMessageBox::warning(this, tr("警告"), tr("user.txt 文件丢失，将自动重新创建。"));
        saveUsers(); // 如果文件丢失，重新创建并保存用户数据
    }
}

//Ymodem部分
void Widget::on_comButton_clicked()
{
    static bool button_status = false;

    if(button_status == false)
    {
        serialPort->setPortName(ui->comPort->currentText());
        serialPort->setBaudRate(ui->comBaudRate->currentText().toInt());

        if(serialPort->open(QSerialPort::ReadWrite) == true)
        {
            button_status = true;

            ui->comPort->setDisabled(true);
            ui->comBaudRate->setDisabled(true);
            ui->comButton->setText(u8"关闭串口");

            ui->transmitBrowse->setEnabled(true);
            ui->receiveBrowse->setEnabled(true);

            if(ui->transmitPath->text().isEmpty() != true)
            {
                ui->transmitButton->setEnabled(true);
            }

            if(ui->receivePath->text().isEmpty() != true)
            {
                ui->receiveButton->setEnabled(true);
            }
        }
        else
        {
            QMessageBox::warning(this, u8"串口打开失败", u8"请检查串口是否已被占用！", u8"关闭");
        }
    }
    else
    {
        button_status = false;

        serialPort->close();

        ui->comPort->setEnabled(true);
        ui->comBaudRate->setEnabled(true);
        ui->comButton->setText(u8"打开串口");

        ui->transmitBrowse->setDisabled(true);
        ui->transmitButton->setDisabled(true);

        ui->receiveBrowse->setDisabled(true);
        ui->receiveButton->setDisabled(true);
    }
}

void Widget::on_transmitBrowse_clicked()
{
    ui->transmitPath->setText(QFileDialog::getOpenFileName(this, u8"打开文件", ".", u8"任意文件 (*.*)"));

    if(ui->transmitPath->text().isEmpty() != true)
    {
        ui->transmitButton->setEnabled(true);
    }
    else
    {
        ui->transmitButton->setDisabled(true);
    }
}

void Widget::on_receiveBrowse_clicked()
{
    ui->receivePath->setText(QFileDialog::getExistingDirectory(this, u8"选择目录", ".", QFileDialog::ShowDirsOnly));

    if(ui->receivePath->text().isEmpty() != true)
    {
        ui->receiveButton->setEnabled(true);
    }
    else
    {
        ui->receiveButton->setDisabled(true);
    }
}

void Widget::on_transmitButton_clicked()
{
    if(transmitButtonStatus == false)
    {
        serialPort->close();

        ymodemFileTransmit->setFileName(ui->transmitPath->text());
        ymodemFileTransmit->setPortName(ui->comPort->currentText());
        ymodemFileTransmit->setPortBaudRate(ui->comBaudRate->currentText().toInt());

        if(ymodemFileTransmit->startTransmit() == true)
        {
            transmitButtonStatus = true;

            ui->comButton->setDisabled(true);

            ui->receiveBrowse->setDisabled(true);
            ui->receiveButton->setDisabled(true);

            ui->transmitBrowse->setDisabled(true);
            ui->transmitButton->setText(u8"取消");
            ui->transmitProgress->setValue(0);
        }
        else
        {
            QMessageBox::warning(this, u8"失败", u8"文件发送失败！", u8"关闭");
        }
    }
    else
    {
        ymodemFileTransmit->stopTransmit();
    }
}

void Widget::on_receiveButton_clicked()
{
    if(receiveButtonStatus == false)
    {
        serialPort->close();

        ymodemFileReceive->setFilePath(ui->receivePath->text());
        ymodemFileReceive->setPortName(ui->comPort->currentText());
        ymodemFileReceive->setPortBaudRate(ui->comBaudRate->currentText().toInt());

        if(ymodemFileReceive->startReceive() == true)
        {
            receiveButtonStatus = true;

            ui->comButton->setDisabled(true);

            ui->transmitBrowse->setDisabled(true);
            ui->transmitButton->setDisabled(true);

            ui->receiveBrowse->setDisabled(true);
            ui->receiveButton->setText(u8"取消");
            ui->receiveProgress->setValue(0);
        }
        else
        {
            QMessageBox::warning(this, u8"失败", u8"文件接收失败！", u8"关闭");
        }
    }
    else
    {
        ymodemFileReceive->stopReceive();
    }
}

void Widget::transmitProgress(int progress)
{
    ui->transmitProgress->setValue(progress);
}

void Widget::receiveProgress(int progress)
{
    ui->receiveProgress->setValue(progress);
}

void Widget::transmitStatus(Ymodem::Status status)
{
    switch(status)
    {
        case YmodemFileTransmit::StatusEstablish:
        {
            break;
        }

        case YmodemFileTransmit::StatusTransmit:
        {
            break;
        }

        case YmodemFileTransmit::StatusFinish:
        {
            transmitButtonStatus = false;

            ui->comButton->setEnabled(true);

            ui->receiveBrowse->setEnabled(true);

            if(ui->receivePath->text().isEmpty() != true)
            {
                ui->receiveButton->setEnabled(true);
            }

            ui->transmitBrowse->setEnabled(true);
            ui->transmitButton->setText(u8"发送");

            QMessageBox::warning(this, u8"成功", u8"文件发送成功！", u8"关闭");

            break;
        }

        case YmodemFileTransmit::StatusAbort:
        {
            transmitButtonStatus = false;

            ui->comButton->setEnabled(true);

            ui->receiveBrowse->setEnabled(true);

            if(ui->receivePath->text().isEmpty() != true)
            {
                ui->receiveButton->setEnabled(true);
            }

            ui->transmitBrowse->setEnabled(true);
            ui->transmitButton->setText(u8"发送");

            QMessageBox::warning(this, u8"失败", u8"文件发送失败！", u8"关闭");

            break;
        }

        case YmodemFileTransmit::StatusTimeout:
        {
            transmitButtonStatus = false;

            ui->comButton->setEnabled(true);

            ui->receiveBrowse->setEnabled(true);

            if(ui->receivePath->text().isEmpty() != true)
            {
                ui->receiveButton->setEnabled(true);
            }

            ui->transmitBrowse->setEnabled(true);
            ui->transmitButton->setText(u8"发送");

            QMessageBox::warning(this, u8"失败", u8"文件发送失败！", u8"关闭");

            break;
        }

        default:
        {
            transmitButtonStatus = false;

            ui->comButton->setEnabled(true);

            ui->receiveBrowse->setEnabled(true);

            if(ui->receivePath->text().isEmpty() != true)
            {
                ui->receiveButton->setEnabled(true);
            }

            ui->transmitBrowse->setEnabled(true);
            ui->transmitButton->setText(u8"发送");

            QMessageBox::warning(this, u8"失败", u8"文件发送失败！", u8"关闭");
        }
    }
}

void Widget::receiveStatus(YmodemFileReceive::Status status)
{
    switch(status)
    {
        case YmodemFileReceive::StatusEstablish:
        {
            break;
        }

        case YmodemFileReceive::StatusTransmit:
        {
            break;
        }

        case YmodemFileReceive::StatusFinish:
        {
            receiveButtonStatus = false;

            ui->comButton->setEnabled(true);

            ui->transmitBrowse->setEnabled(true);

            if(ui->transmitPath->text().isEmpty() != true)
            {
                ui->transmitButton->setEnabled(true);
            }

            ui->receiveBrowse->setEnabled(true);
            ui->receiveButton->setText(u8"接收");

            QMessageBox::warning(this, u8"成功", u8"文件接收成功！", u8"关闭");

            break;
        }

        case YmodemFileReceive::StatusAbort:
        {
            receiveButtonStatus = false;

            ui->comButton->setEnabled(true);

            ui->transmitBrowse->setEnabled(true);

            if(ui->transmitPath->text().isEmpty() != true)
            {
                ui->transmitButton->setEnabled(true);
            }

            ui->receiveBrowse->setEnabled(true);
            ui->receiveButton->setText(u8"接收");

            QMessageBox::warning(this, u8"失败", u8"文件接收失败！", u8"关闭");

            break;
        }

        case YmodemFileReceive::StatusTimeout:
        {
            receiveButtonStatus = false;

            ui->comButton->setEnabled(true);

            ui->transmitBrowse->setEnabled(true);

            if(ui->transmitPath->text().isEmpty() != true)
            {
                ui->transmitButton->setEnabled(true);
            }

            ui->receiveBrowse->setEnabled(true);
            ui->receiveButton->setText(u8"接收");

            QMessageBox::warning(this, u8"失败", u8"文件接收失败！", u8"关闭");

            break;
        }

        default:
        {
            receiveButtonStatus = false;

            ui->comButton->setEnabled(true);

            ui->transmitBrowse->setEnabled(true);

            if(ui->transmitPath->text().isEmpty() != true)
            {
                ui->transmitButton->setEnabled(true);
            }

            ui->receiveBrowse->setEnabled(true);
            ui->receiveButton->setText(u8"接收");

            QMessageBox::warning(this, u8"失败", u8"文件接收失败！", u8"关闭");
        }
    }
}

//MD5校验部分
void Widget::on_MD5Browse_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select File"), QString(), tr("All Files (*)"));
    if (!filePath.isEmpty()) {
        ui->MD5Path->setText(filePath);
    }
}

void Widget::on_MD5Button_clicked()
{
    if (m_md5check == nullptr) {
        m_md5check = new MD5check(this);
        m_md5check->setProgressBar(ui->MD5Progress);
        m_md5check->setPathLineEdit(ui->MD5Path);
        m_md5check->setContentLineEdit(ui->MD5Content);
        connect(m_md5check, &MD5check::progressUpdated, ui->MD5Progress, &QProgressBar::setValue);
    }
    m_md5check->addMD5Check();
}

void Widget::on_MD5clear_clicked()
{
    if (m_md5check) {
        delete m_md5check;
        m_md5check = nullptr;
    }
    ui->MD5Path->clear();
    ui->MD5Content->clear();
    ui->MD5Progress->setValue(0);
}

