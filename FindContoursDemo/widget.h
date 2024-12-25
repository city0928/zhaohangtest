#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainterPath>

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void readMatStdPath(const std::string &strPath);

  protected:
    void paintEvent(QPaintEvent *event);

    QPainterPath path;
};
#endif // WIDGET_H
