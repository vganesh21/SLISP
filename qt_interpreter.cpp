#include "qt_interpreter.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <fstream>

#include <QBrush>
#include <QDebug>

#include "qgraphics_arc_item.hpp"

#include "interpreter_semantic_error.hpp"

QtInterpreter::QtInterpreter(QObject * parent): QObject(parent){
  // TODO: your code here...
}

void QtInterpreter::parseAndEvaluate(QString entry){

    std::string text = entry.toStdString();
    std::istringstream iss(text);
    bool ok = parse(iss);

    if(!ok) {
        QString rs = "Error: could not parse expression";
        emit error(rs);
        return;
    }
    try {
        Expression result = eval();
        std::ostringstream oss;
        oss << result;
        std::cout << oss.str() << std::endl;

        QString rs = QString::fromStdString(oss.str());
        emit info(rs);

        draw();
    }
    catch (InterpreterSemanticError& e) {
        graphics.clear();
        QString rs = QString::fromStdString(e.what());
        emit error(rs);
    }
}
// (draw (point 100 100) )
void QtInterpreter::draw() {
    //std::cout << "We drawing " << graphics.size() << std::endl;
    //std::cout << graphics[0].type << std::endl;
    for (auto a : graphics) {
        //std::cout << "we trying" << std::endl;
        if(a.type == PointType) {
            //std::cout << "It's a point" << std::endl;
            const qreal r = 2;
            double x = a.value.point_value.x;
            double y = a.value.point_value.y;
            auto point = new QGraphicsEllipseItem(x-r, y-r, 2*r, 2*r);
            point->setBrush(QBrush(Qt::black, Qt::SolidPattern));
            emit drawGraphic(point);

        }
        if(a.type == LineType) {
            double x1 = a.value.line_value.first.x;
            double y1 = a.value.line_value.first.y;
            double x2 = a.value.line_value.second.x;
            double y2 = a.value.line_value.second.y;
            auto line = new QGraphicsLineItem(x1, y1, x2, y2);
            emit drawGraphic(line);
        }
        if(a.type == ArcType) {
            double x1 = a.value.arc_value.center.x;
            double y1 = a.value.arc_value.center.y;
            double x2 = a.value.arc_value.start.x;
            double y2 = a.value.arc_value.start.y;
            double span = a.value.arc_value.span;
            double dx = x2 - x1;
            double dy = y2 - y1;
            auto arc = new QGraphicsArcItem(x2, y2, dx, dy, span);
            emit drawGraphic(arc);
        }
    }
}

void QtInterpreter::preload(std::string filename) {
    if(filename.length() > 0) {
        try {
            std::ifstream ifs(filename);
            bool ok = parse(ifs);
            if(!ok) {
                QString rs = "Error: could not parse expression";
                emit error(rs);
                return;
            }
            Expression result = eval();
            std::ostringstream oss;
            oss << result;
            std::cout << oss.str() << std::endl;

            QString rs = QString::fromStdString(oss.str());
            emit info(rs);

            draw();
        }
        catch (InterpreterSemanticError& e) {
            graphics.clear();
            QString rs = QString::fromStdString(e.what());
            emit error(rs);
        }
        //std::cout << filename << std::endl;
    }
}