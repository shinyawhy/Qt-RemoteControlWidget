#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPoint centerPoint = rect().center();

    painter.save();
    painter.setPen(QColor("#34495e"));
    painter.setBrush(QColor("#ced6e0"));
    double radius = (std::min(width(), height()) - 10 * 2) / 2;
    drawRect = QRectF(centerPoint.x() - radius, centerPoint.y() - radius, radius * 2, radius * 2);


    double arcHeight = radius / 2;

    fanShaped[0] = gradientArc(45.0, 90.0, arcHeight); // top
    fanShaped[1] = gradientArc(135.0, 90.0, arcHeight);// left
    fanShaped[2] = gradientArc(225.0, 90.0, arcHeight);//下
    fanShaped[3] = gradientArc(315.0, 90.0, arcHeight);//右

    for (int i = 0;i < 4;++i)
    {
        painter.drawPath(fanShaped[i]);
    }
    painter.restore();

    centerCircularRect = QRectF(centerPoint.x() - (radius / 2), centerPoint.y() - (radius / 2), radius, radius).adjusted(2,2,-2,-2);
    painter.save();
    painter.setPen(Qt::transparent);
    painter.setBrush(QColor("#bdc3c7"));
    painter.drawEllipse(centerCircularRect);
    painter.restore();

    // text
    // left
    painter.save();
    QRectF textRect = QRectF(drawRect.x(), centerCircularRect.y(), radius / 2, radius);
    QPen p(Qt::SolidLine);
    p.setColor("#130f40");
    p.setWidth(2);
    painter.setPen(p);
    QFont font = painter.font();
    font.setPixelSize(24);
    painter.setFont(font);
    painter.drawText(textRect, Qt::AlignCenter, "←");

    // top
    textRect = QRectF(centerCircularRect.x(), drawRect.y(), radius, radius / 2);
    painter.drawText(textRect, Qt::AlignCenter, "↑");

    // bottom
    textRect = QRect(centerCircularRect.bottomLeft().x(), centerCircularRect.bottomLeft().y(), radius, radius / 2);
    painter.drawText(textRect, Qt::AlignCenter, "↓");

    // right
    textRect = QRectF(centerCircularRect.topRight().x(), centerCircularRect.topRight().y(), radius / 2, radius);
    painter.drawText(textRect, Qt::AlignCenter, "→");

    painter.drawText(centerCircularRect, Qt::AlignCenter, "OK");
    painter.restore();

    if(pressedBtn != pressBtnType::None)
    {
        painter.save();
        QColor slightlyOpaqueBlack(0, 0, 0, 63);
        painter.setBrush(slightlyOpaqueBlack);
        painter.setPen(Qt::transparent);
        if(pressedBtn == pressBtnType::center)
        {
            painter.drawEllipse(centerCircularRect);
        }
        else
        {
            int index = -1;
            index = static_cast<int>(pressedBtn);
            if(index >= 0)
            {
                painter.drawPath(fanShaped[index]);
            }
        }
        painter.restore();
    }


//    painter.restore();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    QPointF point = event->pos();

    if(fanShaped[0].contains(point))
    {
        pressedBtn = pressBtnType::up;
            update();
        return;
    }
    if(fanShaped[1].contains(point))
    {
        pressedBtn = pressBtnType::left;
            update();
        return;
    }
    if(fanShaped[2].contains(point))
    {
        pressedBtn = pressBtnType::down;
            update();
        return;
    }
    if(fanShaped[3].contains(point))
    {
        pressedBtn = pressBtnType::right;
            update();
        return;
    }
    if(centerCircularRect.contains(point))
    {
        pressedBtn = pressBtnType::center;
            update();
        return;
    }
    pressedBtn = pressBtnType::None;



    return QWidget::mousePressEvent(event);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(pressedBtn != pressBtnType::None)
    {
        pressedBtn = pressBtnType::None;
        update();
    }
    return QWidget::mouseReleaseEvent(event);

}

QPainterPath Widget::gradientArc(double startAngle, double angleLength, double arcHeight)
{
    //  扇形
    QPainterPath path;
    path.moveTo(drawRect.center());
    path.arcTo(drawRect, startAngle, angleLength);

    // 椭圆
    QPainterPath subPath;
    subPath.addEllipse(drawRect.adjusted(arcHeight, arcHeight, -arcHeight, - arcHeight));

    path -= subPath;
    return path;
}

bool Widget::isPointInCir(const QPoint &point, const QRect &rect)
{
    const QPoint  &centerPoint = rect.center();
    int x = point.x() - centerPoint.x();
    int y = point.y() - centerPoint.y();
}

