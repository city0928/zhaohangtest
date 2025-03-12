#include "widget.h"

#include <vector>

#include <QPainter>

#include "opencv2/core/mat.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>       // OpenCV 主头文件，包含核心功能
#include <opencv2/highgui.hpp>      // GUI 窗口显示（cv::imshow、cv::waitKey）

Widget::Widget(QWidget* parent)
    : QWidget(parent)
{
    // readMatStdPath("D:/guanzhaohang/desktop/signal.png");
    testOutPolygon();
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

QPoint computeCentroid(const QPolygon &polygon) {
    if (polygon.isEmpty()) return QPoint(); // 空多边形，返回 (0,0)

    QPoint centroid(0, 0);
    for (const QPoint &point : polygon) {
        centroid += point; // 累加所有点的坐标
    }
    centroid /= polygon.size(); // 计算平均值
    return centroid;
}

QPolygon expandPolygon(const QPolygon& polygon, qreal distance)
{
    QPolygon expandedPolygon;
    int numPoints = polygon.size();
    if (numPoints <= 3) {
        QPoint center = polygon.boundingRect().center();
        int numSegments = 100;
        for (int i = 0; i < numSegments; ++i) {
            double theta = 2.0 * M_PI * i / numSegments;
            int x = center.x() + 3 * std::cos(theta);
            int y = center.y() + 3 * std::sin(theta);
            expandedPolygon << QPoint(x, y);
        }
        return expandedPolygon;
    }
    // 遍历每个点
    for (int i = 0; i < numPoints; ++i) {
        // 获取当前点及其前后相邻的点
        QPoint currentPoint = polygon[i];
        QPoint prevPoint = polygon[(i - 1 + numPoints) % numPoints]; // 上一个点
        QPoint nextPoint = polygon[(i + 1) % numPoints];             // 下一个点
        // 计算前后相邻边的向量
        QVector2D edge1(currentPoint - prevPoint);
        QVector2D edge2(nextPoint - currentPoint);
        // 计算边向量的单位法线向量（顺时针为外法线方向）
        QVector2D normal1(-edge1.y(), edge1.x());
        QVector2D normal2(-edge2.y(), edge2.x());
        normal1.normalize();
        normal2.normalize();
        // 计算法线向量的平均值
        QVector2D averageNormal = (normal1 + normal2).normalized() + QVector2D(-10, 10);
        // 将当前点沿平均法线方向移动指定距离
        QPoint newPoint = currentPoint + averageNormal.toPoint() * distance;
        // 将扩张后的点添加到新的多边形中
        expandedPolygon << newPoint;
    }
    return expandedPolygon;
}

QPolygon expandPolygon2(const QPolygon &polygon, float distance) {
    QPolygon expandedPolygon;
    int size = polygon.size();

    for (int i = 0; i < size; ++i) {
        // 获取当前点和下一个点
        QPointF p1 = polygon[i];
        QPointF p2 = polygon[(i + 1) % size]; // 循环遍历封闭多边形

                // 计算边向量
        QVector2D edge(p2 - p1);
        edge.normalize();

                // 计算法向量（顺时针 90° 旋转）
        QVector2D normal(-edge.y(), edge.x());

                // 沿法向量移动
        QPointF newP1 = p1 + normal.toPointF() * distance;
        QPointF newP2 = p2 + normal.toPointF() * distance;

        expandedPolygon << newP1.toPoint();
        expandedPolygon << newP2.toPoint();
    }
    return expandedPolygon;
}

QPolygon cvPoint2Polygon(const std::vector<std::vector<cv::Point>> &contours)
{
    QPolygon polygon;

    // 如果 `contours` 为空，直接返回空的 `QPolygon`
    if (contours.empty())
        return polygon;

            // 选择最大的轮廓
    size_t max_index = 0;
    size_t max_size = 0;
    for (size_t i = 0; i < contours.size(); ++i) {
        if (contours[i].size() > max_size) {
            max_size = contours[i].size();
            max_index = i;
        }
    }

            // 遍历最大轮廓中的点，转换为 QPoint 并存入 QPolygon
    for (const cv::Point &p : contours[max_index]) {
        polygon << QPoint(p.x, p.y);
    }

    return polygon;
}

void Widget::testOutPolygon()
{
    QList<QPoint> list = {QPoint(204,201), QPoint(202,203), QPoint(201,203), QPoint(200,204), QPoint(198,204),
          QPoint(197,205), QPoint(195,205), QPoint(190,210), QPoint(190,211), QPoint(187,214),
          QPoint(187,219), QPoint(189,219), QPoint(191,217), QPoint(191,216), QPoint(192,215),
          QPoint(192,213), QPoint(193,212), QPoint(192,213), QPoint(191,213), QPoint(190,212),
          QPoint(190,210), QPoint(192,208), QPoint(193,208), QPoint(194,207), QPoint(194,206),
          QPoint(195,205), QPoint(200,205), QPoint(201,206), QPoint(200,207), QPoint(199,207),
          QPoint(200,207), QPoint(201,206), QPoint(206,206), QPoint(207,205), QPoint(209,205),
          QPoint(210,204), QPoint(213,204), QPoint(214,203), QPoint(228,203), QPoint(229,204),
          QPoint(230,204), QPoint(231,205), QPoint(232,205), QPoint(232,204), QPoint(231,203),
          QPoint(231,202), QPoint(232,201)};
    // QList<QPoint> list = {QPoint(200, 200), QPoint(250, 200), QPoint(250, 300), QPoint(200, 300)};
    path.clear();
    QPolygon polygon(list);
    path.addPolygon(polygon);
    path.closeSubpath();


    // 方案一：
#if 1
    std::vector<cv::Point> points = {
        {204,201}, {202,203}, {201,203}, {200,204}, {198,204}, {197,205}, {195,205},
        {190,210}, {190,211}, {187,214}, {187,219}, {189,219}, {191,217}, {191,216},
        {192,215}, {192,213}, {193,212}, {192,213}, {191,213}, {190,212}, {190,210},
        {192,208}, {193,208}, {194,207}, {194,206}, {195,205}, {200,205}, {201,206},
        {200,207}, {199,207}, {200,207}, {201,206}, {206,206}, {207,205}, {209,205},
        {210,204}, {213,204}, {214,203}, {228,203}, {229,204}, {230,204}, {231,205},
        {232,205}, {232,204}, {231,203}, {231,202}, {232,201}
    };
    cv::Mat img = cv::Mat::zeros(400, 400, CV_8UC1);
    std::vector<std::vector<cv::Point>> contours = {points};
    cv::drawContours(img, contours, -1, cv::Scalar(255), 1);
    int dilation_size = 5;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                               cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1));
    cv::Mat dilatedImg;
    cv::dilate(img, dilatedImg, kernel);
    std::vector<std::vector<cv::Point>> expandedContours;
    cv::findContours(dilatedImg, expandedContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    path2.addPolygon(::cvPoint2Polygon(expandedContours));
#elif 0
    QPainterPathStroker stroker;
    stroker.setWidth(5);
    path2 = stroker.createStroke(path);
#elif 0
    path2.addPolygon(::expandPolygon2(polygon, 5));
#endif
    path2.closeSubpath();
}

void Widget::testAddPixelPolygon()
{
    // 生成可填充的轮廓
    QPainterPathStroker stroker;
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setJoinStyle(Qt::MiterJoin);
    stroker.setDashPattern(Qt::SolidLine);
    stroker.setWidth(10);  // 宽度

    // 生成一个新路径（可填充区域），表示原始路径 path 的轮廓
    path2 = stroker.createStroke(path);

}

void Widget::wheelEvent(QWheelEvent *event)
{
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::red);
    painter.scale(3, 3);
    painter.drawPath(path);
    painter.setPen(Qt::blue);
    painter.drawPath(path2);

    QWidget::paintEvent(event);
}
