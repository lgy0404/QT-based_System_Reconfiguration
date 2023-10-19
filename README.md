# QT-based Software Design for Complex Reconfiguration of Systems

<center><b>❤️对你有帮助的话点个star吧~❤️</b></center>



## 🆕最新消息

- 2023年10月19日：仓库初始化，上传部分资料

## 📒项目介绍

本项目设计的基于QT的配网设备数据采集系统复杂重构软件登录界面通过 `login1` 类实现，主界面通过 `widget` 类实现，通过在 `main` 函数中调用 `logwindow.show()` 实现系统的登录逻辑。软件的图形用户界面结构图如下所示。 `widget` 类中的 485 通信重构模块、UART 串口通信重构模块、以太网通信重构模块、配置下载模块、系统设置模块等 5 个模块主要由 `saveConfigToFile()`、`addUartBaseConfigToConfig()`、`addInternetModeToConfig()`、`loadUsers()` 等 5 个函数实现。 485 重构模块的 slavemap 与 mastermap 的生成分别依赖 `generateSlaveMap()` 与 `createMasterJsonObj()` 两个函数。配置下载模块的 Ymodem 传输功能通过 `Ymodem` 类实现，MD5 校验功能通过 `md5check` 类实现，串口调试模块包括了 `on_sendBt_clicked()` 等函数。 `Ymodem` 类通过调用 `YmodemFileTransmitt` 类与 `YmodemFileReceive` 类分别实现 config 文件的发送以及接收。

以下是各个程序文件的功能列表：

| 文件名                    | 功能                                                         |
| ------------------------- | ------------------------------------------------------------ |
| customcombobox.h/.cpp     | 自定义QComboBox类的行为，在下拉菜单弹出时进行控制。          |
| customtabstyle.h/.cpp     | 在Qt控件中自定义一些样式，主要绘制选项卡中的文本，并设置选中和非选中状态的字体、颜色等。 |
| login.h/.cpp              | 实现一个登录窗口，获取用户输入，验证用户名和密码，如果正确则打开主界面。 |
| login1.h/.cpp/.ui         | 实现一个登录窗口，获取用户输入，验证用户名和密码，如果正确则打开主界面。增加了一个记住密码的功能。 |
| ui_login1.h               | 在 C++ 代码中实现 login1.ui 界面的逻辑功能。                 |
| widget.h/.cpp             | 实现主窗口功能，包括文件处理、界面绘制和自定义控件等功能。   |
| Ymodem.h                  | 实现Ymodem文件传输的状态码、头部和尾部标记、数据包大小等。   |
| YmodemFileReceive.h/.cpp  | 实现Ymodem文件接收的状态处理函数，分别对不同的状态码进行处理。 |
| YmodemFileTransmit.h/.cpp | 实现Ymodem文件传输的状态处理函数，分别对不同的状态码进行处理。 |
| Ymodem.cpp                | 实现Ymodem通用的基础定义和函数。                             |
| YmodemFileReceive.cpp     | 包含 YmodemFileReceive 类，实现 Ymodem 文件接收的功能。      |
| YmodemFileTransmit.cpp    | 包含 YmodemFileTransmit 类，实现 Ymodem 文件传输的功能。     |
| main.cpp                  | 启动程序并展示用户界面。                                     |

## 🖥️软件设计

### 软件开发框架选择

在开发程序之前，必须考虑使用哪个GUI框架。与基于命令行和文本导航的文本界面不同，GUI 是一种允许用户通过图标和视觉指示器与电子设备互动的人机界面[38]。相较于开发控制台程序，制作需要图形用户界面的应用在编程过程中需要借助GUI库。通过调用GUI库内的函数，我们能够创建窗口、按钮、对话框等图形元素。以下是对一些常见的GUI库的简介和比较:

- **WinForm** 是Windows Forms的简称，它是微软推出的一种基于.NET Framework的图形用户界面框架。WinForm允许开发者使用拖放式设计器创建图形界面，支持C#、VB.NET等多种编程语言.

- **Microsoft Foundation Class (MFC)** 库是一个基于C++的微软应用程序框架。MFC为开发者提供了许多封装了Windows API的类，以简化基于C++的Windows应用程序开发.

- **Windows Template Library (WTL)** 是一个基于模板的C++库，用于开发Windows应用程序。它提供了一系列的类，用于封装Windows API，类似于MFC，但比MFC更轻量级.

- **WxWidgets** 是一个开源的、跨平台的C++图形用户界面库。它允许开发者创建基于本地控件的应用程序，支持Windows、Mac、Linux等多个平台.

- **GTK+** 是一个跨平台的开源图形用户界面库，用于开发图形应用程序。最初是为GIMP编写的，后来演变成了GNOME桌面环境的基础. GTK+支持C、C++、Python等多种编程语言.

- **Qt** 是一个跨平台的C++图形用户界面应用程序开发框架。Qt提供了一套统一的API，支持Windows、Mac、Linux等多个平台。Qt还包括了一个集成开发环境（IDE）——Qt Creator，支持快速开发和设计界面.

在以上的开发工具中，WinForm主要限制在Windows平台，不支持跨平台开发；MFC过于庞大且复杂，学习曲线较陡峭，且仅限于Windows平台；WTL主要限制在Windows平台，不支持跨平台开发；功能相对有限，不如MFC完善；WxWidgets虽然跨平台，但在不同平台上的界面可能存在一定的差异；GTK+对于C++开发者来说，封装程度较低，可能需要额外学习GObject系统。而相较于其他框架，QT支持Windows、Mac、Linux等多个平台，且保持了一致的界面和体验。Qt的API设计直观易懂，降低了学习成本，提供了丰富的功能和组件，如多线程、网络编程、数据库访问、OpenGL集成等。并且Qt使用了原生的GUI系统，保证了应用程序的高性能表现，还集成了代码编辑、调试、UI设计等功能，方便快速开发，所以本课题使用Qt作为GUI开发框架.

Qt常用的IDE有官方的Qt Creator和集成Qt的Visual Studio。本课题选择Qt Creator，原因如下:

1. Qt Creator属于轻量级IDE，占系统内存少，可在多个平台运行。Visual Studio 属于重量级IDE，占系统内存多，不需要的模块多，不同版本的Visual Studio适用的平台也不同.

2. Visual Studio 即使集成Qt也无法使用Qt Quick模块，而Qt Creator天然地包含了Qt Quick模块.

3. 与Qt Creator相比，Visual Studio的自动补全功能并不强大，界面不够友好，没有语法高亮等特点，使用户编程效率不高.

![1](https://lgy0404.oss-cn-shanghai.aliyuncs.com/typora1.png)

### 软件与下位机的交互方式设计

本设计通过JSON文件实现上位机与下位机的交互。JSON（JavaScript Object Notation）是一种轻量级的数据交换格式。它以文本格式来描述数据，易于阅读和编写，同时也易于机器解析和生成。JSON文件是以JSON格式编写的文件，通常使用“.json”作为文件扩展名。JSON文件可以包含一个或多个JSON对象，每个JSON对象由一组“键/值”对组成，其中键表示属性名称，值表示属性值。这些键/值对可以包含各种类型的数据，如字符串、数字、布尔值、数组和其他JSON对象。JSON文件在Web应用程序和API交互中广泛使用，因为它们易于解析和生成，同时也具有精简、可读性和可扩展性的优点[39].

配网设备可重构数据采集系统用户可以按照重构需求，通过上位机软件的可视化界面选择各个通信端口的参数以及采集器搭载的配网设备的数量及型号、每个配网设备搭载的传感器的数量及型号。用户所所选择的系统配置信息会存储到config.json中，config.json包括“4851”、“4852”、“4853”、“internet”、“uart”等5个子键。若485X路通信设置为master模式，则该“485X”键由“mode”、“base_config”、“mastermap”3个子键构成。若485X路通信设置为slave模式，则该“485X”键由“mode”、“base_config”、“slavemap”3个子键构成。“internet”键仅包括“mode”子键，用于网口重定向。“uart”键仅包含"base_config"子键用于适配不同的串口屏。config.json的文件的一种示例结构如图3-2所示，本示例中假设采集器只在4851路和4852路模式下各挂在了一台配网设备。上位机软件支持MD5校验，config.json的MD5码会保存到check.md5中，以避免下位机重复解析同一配置文件的问题。配置以及校验文件通过Ymodem协议进行交互，从而通过本文设计的上位机软件实现配网设备数据采集系统的复杂可重构.

![json_image](https://lgy0404.oss-cn-shanghai.aliyuncs.com/typorajson_image.png)

### 软件功能设计

在充分调研了基于QT的配网设备数据采集系统复杂重构软件的功能需求后，本设计将软件分为登录界面与主界面。软件登录界面用户验证软件使用人员的身份信息，确保系统的安全性。系统主界面包括3路485通信重构模块、UART串口通信重构模块、以太网通信重构模块、配置下载模块、系统设置模块等5个模块：3路485通信重构模块支持通信协议配置、主从站配置、master模式下采集器搭载的柜型以及每台柜子配备的传感器的重构、slave模式下的寄存器地址自动分配以及可视化显示；UART串口通信重构模块支持波特率选择；以太网通信重构模块支持通信协议配置；配置下载模块具有串口调试、MD5校验、基于Ymodem协议的配置文件传输功能；系统设计模块可以设置系统管理员的ID、用户名、密码以及身份信息。此外，还支持时间显示、系统帮助、全屏模式、采集器一键配置功能。软件的功能结构如图3-3所示.

![20230422基于QT的配网设备数据采集系统复杂重构软件功能结构图](https://lgy0404.oss-cn-shanghai.aliyuncs.com/typora20230422%E5%9F%BA%E4%BA%8EQT%E7%9A%84%E9%85%8D%E7%BD%91%E8%AE%BE%E5%A4%87%E6%95%B0%E6%8D%AE%E9%87%87%E9%9B%86%E7%B3%BB%E7%BB%9F%E5%A4%8D%E6%9D%82%E9%87%8D%E6%9E%84%E8%BD%AF%E4%BB%B6%E5%8A%9F%E8%83%BD%E7%BB%93%E6%9E%84%E5%9B%BE.png)

## 🔋软件实现

本文设计的基于QT的配网设备数据采集系统复杂重构软件登录界面通过 `login1` 类实现，主界面通过 `widget` 类实现，通过在 `main` 函数中调用 `logwindow.show()` 实现系统的登录逻辑，软件的图形用户界面结构图如图4-1所示。

![20230424基于QT的配网设备数据采集系统复杂重构软件图形用户界面结构图](https://lgy0404.oss-cn-shanghai.aliyuncs.com/typora20230424%E5%9F%BA%E4%BA%8EQT%E7%9A%84%E9%85%8D%E7%BD%91%E8%AE%BE%E5%A4%87%E6%95%B0%E6%8D%AE%E9%87%87%E9%9B%86%E7%B3%BB%E7%BB%9F%E5%A4%8D%E6%9D%82%E9%87%8D%E6%9E%84%E8%BD%AF%E4%BB%B6%E5%9B%BE%E5%BD%A2%E7%94%A8%E6%88%B7%E7%95%8C%E9%9D%A2%E7%BB%93%E6%9E%84%E5%9B%BE.png)

###  485通信重构模块软件实现

本设计中485通信重构模块共包括3个部分，分别为 485配置1、485配置2、485配置3，分别对应于采集器硬件的3路485接口，用于实现485通信模块的可重构。每个485通信重构模块均由协议设计、主站界面、从站界面组成，用户选择的配置信息最后通过 `saveConfigToFile()` 函数保存到 `config.json` 中。

![20230424_485通信重构模块函数设计图](https://lgy0404.oss-cn-shanghai.aliyuncs.com/typora20230424_485%E9%80%9A%E4%BF%A1%E9%87%8D%E6%9E%84%E6%A8%A1%E5%9D%97%E5%87%BD%E6%95%B0%E8%AE%BE%E8%AE%A1%E5%9B%BE.png)

协议设计单元用于配置485通信所选的协议以及通信参数，通信协议支持 Modbus RTU 和 Modbus ASCII，通信参数支持响应模式、波特率、数据位、停止位、校验位的可重构，默认的参数为“Modbus RTU、maser、9600、8、1、none”。

在 `widget.ui` 中，通过 Qt Designer 实现了 master 模式下的图形用户界面的初步设计，添加了 GroupBox、QComboBox、QLabel、QTableWidget 等不同的组件。通过 Qt 的信号槽机制实现 UI 文件中的组件的操作与槽函数的连接。

![2](https://lgy0404.oss-cn-shanghai.aliyuncs.com/typora2.png)

485通信在 slave 模式下的配置文件生成 `generateSlaveMap()` 函数实现。`createMasterJsonObj()` 函数首先查找模式为 "slave" 的 RS-485 设备，并将其 key 保存在 `slaveKey` 中。随后，程序将获取 `slaveKey` 所对应的 value，并从中提取出名为 `slavemap` 的 JSON 对象。generateSlaveMap() 函数的伪代码如下：

```python
it = config.begin()
registerAddress = 1
slaveKey = ""
for item in config:
    if item.key().startsWith("485") and item.value().toObject()["mode"] is "slave":
        slaveKey = item.key()
        break
slaveMapObject = config[slaveKey].toObject()["slavemap"].toObject()
for item in config:
    if item.key().startsWith("485") and item.value().toObject()["mode"] is "master":
        slavemap = {}
        for cabinet in item.value().toObject()["mastermap"].toObject():
            sensorMap = {}
            for sensorName in cabinet.value().toArray():
                registerCount = 3 if sensorName is "internal_air_pressure" else 30 if sensorName is "TEV_and_ultrasonic_busbar" else 1
                addressArray = [registerAddress, registerAddress + registerCount - 1]
                sensorMap[sensorName.toString()] = addressArray
                registerAddress += registerCount
            slavemap[cabinet.key()] = sensorMap
        slaveMapObject[item.key()] = slavemap
config[slaveKey].toObject()["slavemap"] = slaveMapObject
saveConfigToFile()

```

在 `widget.ui` 中，通过 Qt Designer 实现了 slave 模式下的图形用户界面的初步设计，添加了 GroupBox、QComboBox、QLabel、QTableWidget 等不同的组件。通过 Qt 的信号槽机制实现 UI 文件中的组件的操作与槽函数的连接。

###  UART串口通信重构模块软件实现

重构模块的功能主要通过`addUartBaseConfigToConfig()`函数实现。在函数中，程序通过调用`findChild`函数查找GUI界面中对应的单选框控件对象，检查哪个单选框被选中，从而确定波特率的数值。接下来，函数创建一个包含波特率数值的JSON对象。此对象是使用Qt框架中的QJsonObject类创建的。然后，程序创建一个名为“base_config”的JSON对象，并将该包含波特率数值的JSON对象添加到其中。接着，程序创建一个名为“uart”的JSON对象，并将包含“base_config”对象的JSON对象添加到其中。最后，该程序使用`saveConfigToFile()`函数将更新后的JSON配置文件保存到`config.json`文件中。

![3](https://lgy0404.oss-cn-shanghai.aliyuncs.com/typora3.png)

###  配置下载模块软件实现

配置下载模块具有串口调试、MD5校验、基于Ymodem协议的配置文件传输功能三个主要功能。

![4](https://lgy0404.oss-cn-shanghai.aliyuncs.com/typora4.png)

####  串口调试单元

串口调试单元包括初始化串口、打开串口、关闭串口、读取串口数据、发送数据和清空接收框等功能。串口调试单元涉及`on_openBt_clicked()`、`on_closeBt_clicked()`、`serialPortReadyRead_slot()`、`on_sendBt_clicked()`、`on_clearBt_clicked()`等5个槽函数。

<img src="https://lgy0404.oss-cn-shanghai.aliyuncs.com/typora20230426_%E4%B8%B2%E5%8F%A3%E8%B0%83%E8%AF%95%E5%8D%95%E5%85%83%E5%87%BD%E6%95%B0%E8%AE%BE%E8%AE%A1%E5%9B%BE.png" alt="20230426_串口调试单元函数设计图" style="zoom: 150%;" />

串口调试单元程序首先在构造函数中初始化用户界面（UI）及串口实例，并连接串口的readyRead信号到`serialPortReadyRead_slot`槽函数。接着，程序遍历并列出可用的串口名称，将它们添加到ComboBox中。在打开串口的槽函数`on_openBt_clicked`中，从各组合框（ComboBox）中获取波特率、数据位、停止位和校验位的设置，然后设置串口参数并尝试打开串口，根据打开是否成功显示相应的提示信息。关闭串口的槽函数`on_closeBt_clicked`负责关闭串口。`serialPortReadyRead_slot`槽函数负责读取从串口接收到的数据，并将其追加到接收文本编辑框中。`on_sendBt_clicked`槽函数从发送文本编辑框中获取数据，并将其发送到串口。最后，`on_clearBt_clicked`槽函数用于清空接收文本编辑框。串口调试单元程序的伪代码如下：

```python
class Widget:
    Constructor(parent):
        Initialize UI
        Initialize SerialPort
        Connect SerialPort to slot serialPortReadyRead_slot
        Add available ports to ComboBox

    on_openBt_clicked():
        Get settings from ComboBoxes (baud rate, data bits, stop bits, parity)
        Set SerialPort settings
        Open SerialPort and show message

    on_closeBt_clicked():
        Close SerialPort

    serialPortReadyRead_slot():
        Read data from SerialPort and append to receiveEdit

    on_sendBt_clicked():
        Send data from sendEdit to SerialPort

    on_clearBt_clicked():
        Clear receiveEdit
```

####  MD5校验单元

MD5校验单元功能主要由`md5check`类实现，通过对`config.json`进行MD5校验生成`check.md5`校验文件，校验文件与配置文件一同通过Ymodem协议传输到配网设备数据采集器。采集器端在进行配置解析时，首先解析`check.md5`与当前采集器的配置是否匹配，如果匹配则退出本次校验，如果不匹配则读取`config

.json`配置文件来对采集器的配置进行更新。

MD5校验单元程序中`Widget`类负责创建主窗口，包括文件选择、MD5校验和显示和进度条展示等UI组件，并在构造函数中初始化`MD5check`对象及连接信号与槽。当用户点击“浏览”按钮时，程序会打开文件对话框，让用户选择一个文件，然后将文件路径显示在`MD5Path`文本框中。当用户点击“生成MD5”按钮时，程序会调用`MD5check`类的`addMD5Check`方法，该方法首先从`MD5Path`文本框获取文件路径，然后尝试打开文件。接下来，通过`QCryptographicHash`类计算文件的MD5校验和，分块读取文件并逐步更新哈希值，同时根据文件读取进度更新进度条。文件读取完毕后，将哈希结果转换为16进制字符串，并将其写入名为`check.md5`的文件中。最后，将计算出的MD5校验和显示在`MD5Content`文本框中。用户还可以点击“清除”按钮来重置所有UI组件，准备下一次MD5校验和计算。MD5校验单元程序的伪代码如下：

```python
class Widget:
    Constructor(parent):
        Initialize UI
        Initialize SerialPort
        Connect SerialPort to slot serialPortReadyRead_slot
        Add available ports to ComboBox

    on_openBt_clicked():
        Get settings from ComboBoxes (baud rate, data bits, stop bits, parity)
        Set SerialPort settings
        Open SerialPort and show message

    on_closeBt_clicked():
        Close SerialPort

    serialPortReadyRead_slot():
        Read data from SerialPort and append to receiveEdit

    on_sendBt_clicked():
        Send data from sendEdit to SerialPort

    on_clearBt_clicked():
        Clear receiveEdit
```

#### Ymodem文件传输单元

Ymodem文件传输单元功能主要由`Ymodem`类实现，通过Ymodem协议进行配置与校验文件的传输，实现上位机与下位机的交互。Ymodem协议是一种基于Xmodem协议的文件传输协议，在Ymodem协议中，首先通过发送SOH（Start of Header）或STX（Start of Text）数据包，包含文件名和文件大小信息，来启动文件传输过程。接收方等待数据包，并对其进行确认（ACK）或否认（NAK）。发送方继续发送文件数据，将文件分成多个数据包发送，每个数据包包含序列号、数据和校验和或循环冗余校验（CRC）。接收方检查数据包的有效性，包括序列号、数据和校验和或CRC，如果有效，则写入文件并发送ACK，否则发送NAK。当文件传输完毕时，发送方发送EOT（End of Transmission）信号，接收方发送ACK并关闭文件。Ymodem文件传输单元程序的伪代码如下：

```python
class Ymodem
    constructor
        initialize serial port

    openSerialPort
        configure serial port settings

    closeSerialPort

    sendFile
        open input file
        send SOH or STX packet with filename and file size
        wait for ACK or NAK
        while not end of file
            read input file in chunks
            calculate checksum or CRC
            send data packet with sequence number, data, and checksum or CRC
            wait for ACK or NAK
        send EOT
        wait for ACK
        close input file

    receiveFile
        open output file
        send "C" or NAK to request transmission
        while not EOT received
            receive packet
            validate sequence number, data, and checksum or CRC
            if valid
                write data to output file
                send ACK
            else
                send NAK
        send ACK
        close output file
```

### 系统设置模块软件实现

本设计中系统设置模块主要由`loadUsers()`、`saveUsers()`等7个函数实现，用户的ID、用户名、密码以及身份信息保存在系统桌面的“user.txt”中。系统设置模块用于从用户文件“user.txt”中中加载用户信息，并在表格控件中显示这些信息。当用户文件不存在时，程序会创建一个新的用户文件并填充默认用户数据。用户数据发生变化时，程序会将更新后的用户信息保存到文件中。

<img src="https://lgy0404.oss-cn-shanghai.aliyuncs.com/typora20230425_%E7%B3%BB%E7%BB%9F%E8%AE%BE%E7%BD%AE%E6%A8%A1%E5%9D%97%E5%87%BD%E6%95%B0%E8%AE%BE%E8%AE%A1%E5%9B%BE.png" alt="20230425_系统设置模块函数设计图" style="zoom:150%;" />

- `loadUsers()`: 负责从用户文件加载用户信息。首先检查用户文件是否存在。如果存在，则打开文件并逐行读取内容，将每行数据分割成不同的字段，并将这些信息添加到表格控件中。如果文件不存在，程序会创建新的用户文件，并向表格控件添加默认的用户数据。

- `saveUsers()`: 将表格控件中的用户信息保存回文件。遍历表格的每一行，将每个单元格的数据

添加到一个字符串列表中。对于部门和账户类型列，从下拉框中获取当前选中的文本。将这些字符串列表用逗号连接，然后写入文件中。

- `on_addUserButton_clicked()`: 添加新用户到表格控件。首先断开表格的`itemChanged`信号，以避免在添加过程中触发不必要的保存操作。然后插入新行，并设置用户ID和其他字段。为部门和账户类型列创建下拉框，并将它们添加到表格中。最后，重新连接`itemChanged`信号。

- `on_deleteUserButton_clicked()`: 从表格控件中删除选中的用户，并调用`saveUsers()`将更新后的用户信息保存到文件中。

- `onUserTableItemChanged()`: 响应表格项更改事件，当表格中的任何单元格发生更改时，调用`saveUsers()`保存用户信息。

- `onComboBoxIndexChanged()`: 响应下拉框索引更改事件，当部门或账户类型下拉框的选项发生更改时，调用`saveUsers()`保存用户信息。

- `checkFileExistence()`: 检查用户文件是否存在。如果文件丢失，弹出警告消息并重新创建文件，同时调用`saveUsers()`保存当前的用户数据。

![5](https://lgy0404.oss-cn-shanghai.aliyuncs.com/typora5.png)

---

❤️对你有帮助的话点个star吧~❤️
