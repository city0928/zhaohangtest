#include "widget.h"

#include <vector>

#include <QPainter>

#include "opencv2/core/mat.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc.hpp>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
{
    readMatStdPath("D:/guanzhaohang/desktop/signal.png");
}

Widget::~Widget()
{
}

void Widget::readMatStdPath(const std::string& strPath)
{
    auto func = [&](QPainterPath& path, const std::vector<cv::Point>& points){
        QPolygon polygon;
        for (const cv::Point& poi : points) {
            polygon << QPoint(poi.x, poi.y);
        }
        path.addPolygon(polygon);
    };

    cv::Mat mat = cv::imread(strPath, cv::IMREAD_GRAYSCALE);
    cv::resize(mat, mat, cv::Size(300, 600));
    std::vector<std::vector<cv::Point>> findContours;
    std::vector<cv::Vec4i> contourIdx;
    cv::findContours(mat, findContours, contourIdx, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat drawOutMat = cv::Mat::zeros(mat.rows, mat.cols, mat.type());
    cv::Mat drawInnerMat = cv::Mat::zeros(mat.rows, mat.cols, mat.type());
    for (int i = 0; i < (int)findContours.size(); i++) {
        if (contourIdx[i][3] == -1) {  // out contours
            cv::drawContours(drawOutMat, findContours, -1, cv::Scalar(255, 255, 255), 2, 8);
            func(path, findContours.at(i));
        } else {  // inner contours
            cv::drawContours(drawInnerMat, findContours, -1, cv::Scalar(255, 255, 255), 2, 8);
            func(path, findContours.at(i));
        }
    }
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::red);
    painter.setBrush(Qt::blue);
    painter.drawPath(path);

    QWidget::paintEvent(event);
}
