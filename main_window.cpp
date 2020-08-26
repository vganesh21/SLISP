#include "main_window.hpp"

#include <iostream>

#include <QLayout>

#include "message_widget.hpp"
#include "canvas_widget.hpp"
#include "repl_widget.hpp"
#include "interpreter_semantic_error.hpp"

MainWindow::MainWindow(QWidget * parent): MainWindow("", parent){
  // TODO: your code here...
}

MainWindow::MainWindow(std::string filename, QWidget * parent): QWidget(parent){

    auto messageView = new MessageWidget;
    auto canvasView = new CanvasWidget(this);
    auto replView = new REPLWidget(this);

    QObject::connect(replView, &REPLWidget::lineEntered, &interp, &QtInterpreter::parseAndEvaluate);
    QObject::connect(&interp, &QtInterpreter::info, messageView, &MessageWidget::info);
    QObject::connect(&interp, &QtInterpreter::error, messageView, &MessageWidget::error);
    QObject::connect(&interp, &QtInterpreter::drawGraphic, canvasView, &CanvasWidget::addGraphic);

    auto layout = new QVBoxLayout;
    layout->addWidget(messageView);
    layout->addWidget(canvasView);
    layout->addWidget(replView);

    setLayout(layout);
    interp.preload(filename);

}
