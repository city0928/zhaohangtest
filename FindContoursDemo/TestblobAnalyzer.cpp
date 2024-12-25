#include "TestblobAnalyzer.h"

#include "BlobInfoList.h"
#include "ExecHelper.h"
#include "LibImgProc/BlobAnalyzerCA.h"
#include "LibImgProc/BlobAnalyzerCC.h"
#include "opencv2/imgproc.hpp"
#include <QDebug>
#include <QPainter>

using BlobInfoList = LinkSenseAIFI::BlobInfoList;

TestblobAnalyzer::TestblobAnalyzer(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

TestblobAnalyzer::~TestblobAnalyzer() {}

QList<QPolygonF> TestblobAnalyzer::findBlobAnalyzerCA(cv::Mat& mat)
{
    if (mat.empty()) {
        return QList<QPolygonF>();
    }

    QList<QPolygonF> listPolygonf;
    BlobInfoList infoBlobTop;
    if (0 == infoBlobTop.size()) {
        //LinkSenseAIFI::LibImgProc::BlobAnalyzerCA analyzerTop(LinkSenseAIFI::LibImgProc::ContourLevelMode::SemiInnerCnt);

        LinkSenseAIFI::LibImgProc::BlobAnalyzerCC analyzerTop;
        // LinkSenseAIFI::ExecHelper::judger(analyzerTop.analysis(mat, infoBlobTop));
        // qDebug() << &analyzerTop;
        const auto err = analyzerTop.analysis(mat, infoBlobTop);
        qDebug() << QString::fromLocal8Bit(err.info());
        for (size_t indexTop = 0; indexTop < infoBlobTop.size(); indexTop++) {
            auto polygon = infoBlobTop.at(indexTop).polyOuter.toQPolyF();
            listPolygonf.push_back(polygon);
        }
    }
    qDebug() << listPolygonf;
    drawPolygonsOnImage(mat, listPolygonf, "./123.png");

    return listPolygonf;

    // LinkSenseAIFI::BlobInfoList blobInfos;
    // auto analyzer = LinkSenseAIFI::LibImgProc::BlobAnalyzerCA(LinkSenseAIFI::LibImgProc::ContourLevelMode::OnlyOuterCnt);
    // LinkSenseAIFI::ExecHelper::judger(analyzer.analysis(mat, blobInfos));
    // qDebug() << blobInfos.size();

    // AIFI::ContourList contours;
    // std::vector<cv::Vec4i> hierarchy;
    // cv::findContours(mat, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
    // qDebug() << contours.size();
}

void TestblobAnalyzer::drawPolygonsOnImage(cv::Mat& mat, const QList<QPolygonF>& listPolygonf, const QString& outputPath)

{
    // 1. 创建一个灰度图像（尺寸可以根据需要设置）
    int width = mat.cols;
    int height = mat.rows;
    QImage image(width, height, QImage::Format_Grayscale8);

    // 2. 初始化图像，填充为白色（灰度255）
    image.fill(255);

    // 3. 创建 QPainter 对象用于绘制
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿
    painter.setPen(Qt::black);  // 设置黑色的画笔（可以根据需要更改）
    painter.setBrush(Qt::NoBrush);  // 设定画刷为无填充色（透明）

    // 4. 绘制多边形列表
    for (const QPolygonF& polygon : listPolygonf) {
        painter.drawPolygon(polygon);
    }

    // 5. 保存绘制后的图像（你可以选择显示图像或保存）
    image.save(outputPath);  // 保存图像到文件
    qDebug() << "Image saved to:" << outputPath;

    painter.end();  // 结束绘制
}

void TestblobAnalyzer::testTwo(cv::Mat& mat)
{
    // 3. 二值化
    cv::Mat binaryImage;
    cv::threshold(mat, binaryImage, 127, 255, cv::THRESH_BINARY);

    // 4. 寻找连通域
    cv::Mat labels;
    int numLabels = cv::connectedComponents(binaryImage, labels);

    // 5. 输出连通域数量
    qDebug() << "连通域数量: " << numLabels - 1 << "（排除背景）";
}
