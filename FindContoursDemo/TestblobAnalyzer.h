#pragma once

#include "ui_TestblobAnalyzer.h"
#include <QWidget>

#include "opencv2/core/mat.hpp"
#include <QList>
#include <QPolygonF>
#include <QPaintEvent>

class TestblobAnalyzer : public QWidget
{
    Q_OBJECT

  public:
    TestblobAnalyzer(QWidget* parent = nullptr);
    ~TestblobAnalyzer();

    QList<QPolygonF> findBlobAnalyzerCA(cv::Mat& mat);

    void drawPolygonsOnImage(cv::Mat& mat, const QList<QPolygonF>& listPolygonf, const QString& outputPath);

    void testTwo(cv::Mat& mat);

  private:
    Ui::TestblobAnalyzer ui;
};
