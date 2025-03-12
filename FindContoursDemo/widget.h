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

    void testOutPolygon();
    void testAddPixelPolygon();

  protected:
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    QPainterPath path;
    QPainterPath path2;
};
#endif // WIDGET_H
