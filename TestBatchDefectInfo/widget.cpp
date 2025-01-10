#include "widget.h"

#include <vector>

#include <QPainter>
#include <QDebug>

#include "opencv2/core/mat.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc.hpp>

#include "Logger.h"

using Logur = LinkSenseAIFI::Logur;

void BatchDefectInfo::makeVirtualData()
{
    BaseInfo baseInfo;
    baseInfo.DefectUID = 0001;
    baseInfo.DefectName = "testDefectName_1";
    baseInfo.DefectColor = {45, 0, 0, 255};

    PolygonDefectInfo polygonDefectInfo;
    polygonDefectInfo.Info = baseInfo;
    polygonDefectInfo.CenterPoint = Point(256, 256);
    polygonDefectInfo.Polygon = Points({Point(100, 100), Point(100, 200), Point(300, 200), Point(300, 100)});
    polygonDefectInfo.Radius = 9;
    polygonDefectInfo.PanelQRcode = "QRCode_1";
    polygonDefectInfo.Summary = 10;

    ImgDefectInfo defectInfo;
    defectInfo.Info = baseInfo;
    defectInfo.SmallImgIdx = 1;
    defectInfo.DefectIdx = 2;


    std::map<std::string, short> defectList = {{"有感划伤", 1}, {"无感划伤", 3}, {"孔漏铜", 4}};
    std::map<BatchSideType, std::vector<PolygonDefectInfo>> aa =
        {
            {BatchSideType::SideTop, {{polygonDefectInfo}, {polygonDefectInfo}, {polygonDefectInfo} }},
            {BatchSideType::SideBot, {{polygonDefectInfo} }}
        };
    std::map<BatchSideType, std::vector<ImgDefectInfo>> ivs =
        {
            {BatchSideType::SideTop, {{defectInfo}, {defectInfo}, {defectInfo}, {defectInfo} }},
            {BatchSideType::SideBot, {{defectInfo}, {defectInfo}, {defectInfo}, {defectInfo} }}
        };
    BatchDefectInfo batchInfo;
    batchInfo.IsVaild = true;
    batchInfo.m_DefectToleranceRange = defectList;
    batchInfo.m_DefectInfo = {{"Panel0001", aa}};
    batchInfo.m_DefectImgInfo = {{"Panel0001", ivs}};

    *this = batchInfo;

    toJson();
}

void BatchDefectInfo::wait() {

    qDebug() << "123";
    qDebug() << "123";
}

void BatchDefectInfo::toJson(const std::string& jsonPath /*= std::filesystem::current_path().generic_string() + "/generalConfig.json"*/)
{
    if (std::filesystem::exists(jsonPath))
        std::remove(jsonPath.c_str());
    yas::save<yas::file | yas::json>(jsonPath.c_str(), *this);
}

void BatchDefectInfo::fromJson(const std::string& jsonPath /*= std::filesystem::current_path().generic_string() + "/generalConfig.json"*/)
{
    if (not std::filesystem::exists(jsonPath)){
        Logur::i().error("general config path is not exists！！！init error！！！");
        return;
    }
    yas::load<yas::json | yas::file>(jsonPath.c_str(), *this);
}


// Widget::Widget(QWidget* parent)
//     : QWidget(parent)
// {
//     readMatStdPath("D:/guanzhaohang/desktop/signal.png");
// }

// Widget::~Widget()
// {
// }

// void Widget::readMatStdPath(const std::string& strPath)
// {
//     auto func = [&](QPainterPath& path, const std::vector<cv::Point>& points){
//         QPolygon polygon;
//         for (const cv::Point& poi : points) {
//             polygon << QPoint(poi.x, poi.y);
//         }
//         path.addPolygon(polygon);
//     };

//     cv::Mat mat = cv::imread(strPath, cv::IMREAD_GRAYSCALE);
//     cv::resize(mat, mat, cv::Size(300, 600));
//     std::vector<std::vector<cv::Point>> findContours;
//     std::vector<cv::Vec4i> contourIdx;
//     cv::findContours(mat, findContours, contourIdx, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

//     cv::Mat drawOutMat = cv::Mat::zeros(mat.rows, mat.cols, mat.type());
//     cv::Mat drawInnerMat = cv::Mat::zeros(mat.rows, mat.cols, mat.type());
//     for (int i = 0; i < (int)findContours.size(); i++) {
//         if (contourIdx[i][3] == -1) {  // out contours
//             cv::drawContours(drawOutMat, findContours, -1, cv::Scalar(255, 255, 255), 2, 8);
//             func(path, findContours.at(i));
//         } else {  // inner contours
//             cv::drawContours(drawInnerMat, findContours, -1, cv::Scalar(255, 255, 255), 2, 8);
//             func(path, findContours.at(i));
//         }
//     }
// }

// void Widget::paintEvent(QPaintEvent *event)
// {
//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing);
//     painter.setPen(Qt::red);
//     painter.setBrush(Qt::blue);
//     painter.drawPath(path);

//     QWidget::paintEvent(event);
// }
