#ifndef CHARACTERLABEL_H
#define CHARACTERLABEL_H

#include <QColor>
#include <QFont>
#include <QLabel>
#include <QString>

class CharacterLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CharacterLabel(QWidget *parent = nullptr);
    ~CharacterLabel();

    QColor color() const;

public slots:
    void setText(const QString &text);
    void setFont(const QFont &font);
    void setColor(const QColor &color);

private:
    QColor m_color;
};

#endif // CHARACTERLABEL_H
