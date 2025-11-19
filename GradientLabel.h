#ifndef GRADIENTLABEL_H
#define GRADIENTLABEL_H

#include <QLabel>
#include <QPainter>
#include <QPainterPath>

class GradientLabel : public QLabel
{
    Q_OBJECT
public:
    explicit GradientLabel(QWidget *parent = nullptr);

    void setGradient(const QColor &left, const QColor &right);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor leftColor;
    QColor rightColor;
};

#endif
