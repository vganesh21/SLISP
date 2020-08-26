#include "canvas_widget.hpp"

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLayout>
#include <iostream>

CanvasWidget::CanvasWidget(QWidget * parent): QWidget(parent){

    scene = new QGraphicsScene(this);
    auto view = new QGraphicsView(scene);
    auto layout = new QVBoxLayout;
    layout->addWidget(view);
    setLayout(layout);
}

void CanvasWidget::addGraphic(QGraphicsItem * item){
    //std::cout << "calling draws" << std::endl;
    scene->addItem(item);
}
