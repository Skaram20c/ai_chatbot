#include "GradientLabel.h"

GradientLabel::GradientLabel(QWidget *parent)
    : QLabel(parent),
    leftColor("#0BB9BF"),
    rightColor("#0E84CF")
{
}

void GradientLabel::setGradient(const QColor &left, const QColor &right)
{
    leftColor  = left;
    rightColor = right;
    update();
}

void GradientLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    QFontMetrics fm(font());
    int textWidth = fm.horizontalAdvance(text());
    int textHeight = fm.height();

    int x = (width() - textWidth) / 2;
    int y = (height() + textHeight) / 2 - fm.descent();

    QPainterPath path;
    path.addText(x, y, font(), text());

    QLinearGradient grad(0, 0, width(), 0);
    grad.setColorAt(0, leftColor);
    grad.setColorAt(1, rightColor);

    p.setBrush(grad);
    p.setPen(Qt::NoPen);
    p.drawPath(path);
}
