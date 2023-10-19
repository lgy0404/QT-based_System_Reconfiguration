#include "customtabstyle.h"
#include <QStyleOptionTab>
#include <QPainter>
#include <QTextOption>

// CustomTabStyle的构造函数，传递给QProxyStyle的基类构造函数
CustomTabStyle::CustomTabStyle(QStyle *style) : QProxyStyle(style) {}

CustomTabStyle::~CustomTabStyle() { // 添加虚析构函数实现
}

// 重写sizeFromContents函数，返回每个tab的大小
QSize CustomTabStyle::sizeFromContents(ContentsType type, const QStyleOption *option,
    const QSize &size, const QWidget *widget) const
{
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab) {
        s.transpose(); // 交换宽高值
        s.rwidth() = 95; // 设置每个tabBar中item的宽度
        s.rheight() = 75; // 设置每个tabBar中item的高度
    }
    return s;
}


// 重写drawControl方法，根据需要自定义部分样式
void CustomTabStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    if (element == CE_TabBarTabLabel) {
        const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option);

        if (tab) {
            QRect allRect = tab->rect; // 获取标签的矩形区域

            if (tab->state & QStyle::State_Selected) {
                painter->save(); // 保存画笔的当前设置

                QFont font("黑体", 10);
                painter->setFont(font);

                painter->setPen(0x89cfff); // 设置画笔颜色
                painter->setBrush(QBrush(0x89cfff)); // 设置画刷颜色
                painter->drawRect(allRect); // 绘制矩形，适应整个标签矩形区域

                painter->restore(); // 恢复画笔的之前的设置
            } else {
                QFont font("黑体", 10);
                painter->setFont(font);
            }

            QTextOption textOption; // 创建文本选项对象
            textOption.setAlignment(Qt::AlignCenter); // 设置文本居中对齐

            // 设置文本颜色，选中状态和非选中状态使用不同的颜色
            if (tab->state & QStyle::State_Selected) {
                painter->setPen(0xf8fcff);
            } else {
                painter->setPen(0x5d5d5d);
            }

            // 在标签矩形区域内绘制文本
            painter->drawText(allRect, tab->text, textOption);
            return; // 提前返回，因为我们已经完成了自定义绘制
        }
    }

    // 对于非Tab标签元素，调用基类的drawControl方法
    QProxyStyle::drawControl(element, option, painter, widget);
}
