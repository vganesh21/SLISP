#include "message_widget.hpp"

#include <QLabel>
#include <QLayout>
#include <QLineEdit>

MessageWidget::MessageWidget(QWidget *parent) : QWidget(parent) {
    messageLabel = new QLabel("Message: ", this);
    messageBox = new QLineEdit(this);
    messageBox->setReadOnly(true);

    auto layout = new QGridLayout;

    layout->addWidget(messageBox, 0, 1);
    layout->addWidget(messageLabel, 0, 0);
    setLayout(layout);

    failureRed.setColor(QPalette::Highlight, Qt::red);
}

void MessageWidget::info(QString message) {
    messageBox->clear();
    messageBox->insert(message);
    messageBox->setPalette(successBlue);
}

void MessageWidget::error(QString message) {

    std::string hello;
    hello = message.toStdString();
    messageBox->clear();
    messageBox->insert(message);
    messageBox->setPalette(failureRed);
    messageBox->selectAll();
}

void MessageWidget::clear() {
    messageBox->clear();
}
