#ifndef GRADIENTLABEL_H
#define GRADIENTLABEL_H

#include <QLabel>
#include <QPainter>
#include <QPainterPath>

/**
 * @class GradientLabel
 * @brief A QLabel that paints its text using a smooth left-to-right gradient.
 *
 * GradientLabel is used to display titles/headings with visually appealing
 * gradient-colored text. It behaves exactly like QLabel, except that during
 * paintEvent() it draws the text manually using QPainterPath + QLinearGradient.
 *
 * Key Features:
 *  - Fully customizable gradient colors
 *  - High-quality anti-aliased text rendering
 *  - Backward compatible: if no gradient set, defaults to solid text color
 */
class GradientLabel : public QLabel
{
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param parent Parent widget
     */
    explicit GradientLabel(QWidget *parent = nullptr);

    /**
     * @brief Sets the two gradient colors.
     * @param left  Left color of gradient
     * @param right Right color of gradient
     */
    void setGradient(const QColor &left, const QColor &right);

protected:
    /**
     * @brief Custom paint event: draws gradient text.
     *
     * If the text is empty or gradient colors are invalid,
     * it falls back to default QLabel rendering.
     */
    void paintEvent(QPaintEvent *event) override;

private:
    QColor leftColor;     ///< Left color of gradient
    QColor rightColor;    ///< Right color of gradient
};

#endif  // GRADIENTLABEL_H
