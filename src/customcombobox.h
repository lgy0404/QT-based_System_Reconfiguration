#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include <QComboBox>

class CustomComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CustomComboBox(QWidget *parent = nullptr);

protected:
    void showPopup() override;
};

#endif // CUSTOMCOMBOBOX_H
