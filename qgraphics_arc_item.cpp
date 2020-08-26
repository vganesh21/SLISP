#include "qgraphics_arc_item.hpp"

#include <cmath>
#include <iostream>

#include <QDebug>
#include <QPainter>

QGraphicsArcItem::QGraphicsArcItem( qreal x, qreal y, qreal width, qreal height, qreal spanA, QGraphicsItem * parent ){
    setRect(x, y, -(width*2), -(height*2));
    setStartAngle(16 * atan2(-height, width) * (180 / atan2(0, -1)));
    setSpanAngle(16 * spanA * (180/atan2(0, -1)));
    startx = x;
    starty = y;
    w = width;
    h = height;
    startAng = atan2(-height, width) * (180 / atan2(0, -1));
    spanAng = spanA * (180/atan2(0, -1));
}
void QGraphicsArcItem::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *widget)  {
    painter->setPen(pen());
    painter->setBrush(brush());
    //painter->drawArc(rect(), startAngle(), spanAngle());
    int radius = sqrt( pow(w, 2) + pow(h, 2));
    int centerx = startx - w;
    int centery = starty - h;
    //std::cout << "wh" << w << " " << h << std::endl;
    QRectF rectangle(startx+(radius-startx), starty+(radius-starty), -radius*2, -radius*2);
    QPointF qCenter;
    qCenter.setX(centerx);
    qCenter.setY(centery);
    rectangle.moveCenter(qCenter);
    int startAngle = startAng * 16;
    int spanAngle = spanAng * 16;
    //std::cout << startx << " " << starty << " " << radius << std::endl;
    painter->drawArc(rectangle, startAngle, spanAngle);

}
