#ifndef REPL_WIDGET_HPP
#define REPL_WIDGET_HPP

#include <QWidget>
#include <QLineEdit>
#include <QString>
#include <QLabel>
#include <QVector>

class History{

public:
    History() {
        history.push_back(QString());
        currentIndex = 0;
    }
    void add(const QString& entry) {
        history.insert(1, entry);
        currentIndex = 0;
    }
    void up() {
        if(currentIndex < history.size()-1) {
            currentIndex+=1;
        }
    }
    void down() {
        if(currentIndex > 0) {
            currentIndex-=1;
        }
    }
    QString & getCurrent() {
        return history[currentIndex];
    }

private:

    QVector<QString> history;
    int currentIndex;
};




class REPLWidget: public QWidget{
Q_OBJECT

public:

  REPLWidget(QWidget * parent = nullptr);

protected:

    void keyPressEvent(QKeyEvent *event);

    History history;

private:
    QLineEdit * replBox;
    QLabel * replLabel;

signals:

  void lineEntered(QString entry);

private slots:

  void changed();
};

#endif
