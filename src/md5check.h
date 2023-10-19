#ifndef MD5CHECK_H
#define MD5CHECK_H

#include <QObject>
#include <QProgressBar>
#include <QLineEdit>

class MD5check : public QObject
{
    Q_OBJECT
public:
    explicit MD5check(QObject *parent = nullptr);


       // 添加MD5校验
       void addMD5Check();

       // 设置控件指针
       void setProgressBar(QProgressBar *progressBar);
       void setPathLineEdit(QLineEdit *pathLineEdit);
       void setContentLineEdit(QLineEdit *contentLineEdit);


public slots:
    void browseFile();



signals:
    void progressUpdated(int value);

private:
    // 控件指针
     QProgressBar *m_progressBar;
     QLineEdit *m_pathLineEdit;
     QLineEdit *m_contentLineEdit;

     // 私有成员函数
     QString calculateMD5(QString filePath);
};

#endif // MD5CHECK_H
