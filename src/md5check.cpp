#include "md5check.h"
#include <QFileDialog>
#include <QFile>
#include <QCryptographicHash>
#include <QMessageBox>

MD5check::MD5check(QObject *parent) : QObject(parent),
    m_progressBar(nullptr), m_pathLineEdit(nullptr), m_contentLineEdit(nullptr)
{

}

void MD5check::browseFile()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, tr("Select File"), QString(), tr("All Files (*)"));
    if (!filePath.isEmpty()) {
        m_pathLineEdit->setText(filePath);
    }
}

void MD5check::addMD5Check()
{
    QString filePath = m_pathLineEdit->text();
    if (filePath.isEmpty()) {
        QMessageBox::warning(nullptr, tr("Warning"), tr("Please select a file first!"));
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(nullptr, tr("Warning"), tr("Failed to open file: %1").arg(filePath));
        return;
    }

    QCryptographicHash hash(QCryptographicHash::Md5);
    qint64 fileSize = file.size();
    qint64 readSize = 0;
    const qint64 blockSize = 65536;
    char buffer[blockSize];
    while (!file.atEnd()) {
        qint64 bytesRead = file.read(buffer, blockSize);
        readSize += bytesRead;
        hash.addData(buffer, bytesRead);

        int progressValue = static_cast<int>(100 * readSize / fileSize);
        emit progressUpdated(progressValue);
    }

    file.close();

    QString md5Code = QString(hash.result().toHex());
    QString md5FilePath = QString("%1/check.md5").arg(QFileInfo(filePath).path());
    QFile md5File(md5FilePath);
    if (!md5File.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, tr("Warning"), tr("Failed to create MD5 check file!"));
        return;
    }
    md5File.write(md5Code.toLatin1());
    md5File.close();

    m_contentLineEdit->setText(md5Code);
}

//为MD5check类添加setProgressBar()、setPathLineEdit()、setContentLineEdit()方法，
void MD5check::setProgressBar(QProgressBar *progressBar)
{
m_progressBar = progressBar;
}

void MD5check::setPathLineEdit(QLineEdit *pathLineEdit)
{
m_pathLineEdit = pathLineEdit;
}

void MD5check::setContentLineEdit(QLineEdit *contentLineEdit)
{
m_contentLineEdit = contentLineEdit;
}

