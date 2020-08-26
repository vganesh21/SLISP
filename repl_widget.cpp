#include "repl_widget.hpp"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QKeyEvent>



REPLWidget::REPLWidget(QWidget * parent): QWidget(parent){

    replBox = new QLineEdit(this);
    replLabel = new QLabel("sldraw> ", this);
    auto layout = new QGridLayout;

    layout->addWidget(replLabel, 2, 0);
    layout->addWidget(replBox, 2, 1);
    setLayout(layout);

    QObject::connect(replBox, SIGNAL(returnPressed()), this, SLOT(changed()));

}


void REPLWidget::changed() {

    emit lineEntered(replBox->text());
    history.add(replBox->text());
    replBox->clear();
}

void REPLWidget::keyPressEvent(QKeyEvent *event) {

    if(event->key() == Qt::Key_Up) {
        history.up();
        replBox->clear();
        replBox->setText(history.getCurrent());
    }
    if(event->key() == Qt::Key_Down) {
        history.down();
        replBox->clear();
        replBox->setText(history.getCurrent());
    }

    QWidget::keyPressEvent(event);
}

