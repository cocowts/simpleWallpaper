#include "characterlabel.h"

#include <QFontMetrics>
#include <QRect>

CharacterLabel::CharacterLabel(QWidget *parent) : QLabel(parent)
{ }

CharacterLabel::~CharacterLabel()
{ }

QColor CharacterLabel::color() const
{
    return m_color;
}

void CharacterLabel::setText(const QString &text)
{
    QFontMetrics fm(font());
    QRect rect = fm.boundingRect(text);

    QLabel::setText(text);
    setFixedSize(rect.width(), rect.height());
}

void CharacterLabel::setFont(const QFont &font)
{
    QFontMetrics fm(font);
    QRect rect = fm.boundingRect(text());

    QWidget::setFont(font);
    setFixedSize(rect.width(), rect.height());
}

void CharacterLabel::setColor(const QColor &color)
{
    m_color = color;

    setStyleSheet(QStringLiteral("QLabel {color:rgba(%1, %2, %3, %4);}")
                                        .arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
}
