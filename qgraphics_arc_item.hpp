#ifndef QGRAPHIC_ARC_ITEM_HPP
#define QGRAPHIC_ARC_ITEM_HPP

#include <QGraphicsEllipseItem>

class QGraphicsArcItem : public QGraphicsEllipseItem {
public:
	QGraphicsArcItem ( qreal x, qreal y, qreal width, qreal height, qreal spanA,
			QGraphicsItem * parent = nullptr );

protected:
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
private:
	int startx, starty, w, h, spanAng, startAng;
};


#endif
