#include "customcombobox.h"
#include <QAbstractItemView>
#include <QScrollBar>
#include <QFontMetrics>

CustomComboBox::CustomComboBox(QWidget *parent) : QComboBox(parent)
{
}

void CustomComboBox::showPopup()
{
    QFontMetrics fm(font());
    int maxWidth = minimumContentsLength() * fm.horizontalAdvance('X');
    for (int i = 0; i < count(); ++i) {
        int itemWidth = fm.horizontalAdvance(itemText(i)) + 32; // Add some extra padding for the scrollbar and margins
        maxWidth = qMax(maxWidth, itemWidth);
    }

    view()->setMinimumWidth(maxWidth);
    QComboBox::showPopup();
}
