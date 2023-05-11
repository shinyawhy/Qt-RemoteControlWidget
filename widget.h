#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <math.h>
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
signals:
    void selectColor(QString colorString);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    Ui::Widget *ui;
    QRectF drawRect; // big circle
    QPainterPath fanShaped[4];
    QRectF centerCircularRect; // center circle
    enum class pressBtnType{
      up = 0,
        left,
        down,
        right,
        center,
        None
    };
    pressBtnType pressedBtn =pressBtnType::None;
private:
    QPainterPath gradientArc(double startAngle, double angleLength, double arcHeight);
    bool isPointInCir(const QPoint &point, const QRect &rect);
};
#endif // WIDGET_H
