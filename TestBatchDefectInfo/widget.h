#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainterPath>

#include <iostream>
#include <string>
#include <map>
#include <array>
#include <filesystem>
#include <yas/serialize.hpp>
#include <yas/std_types.hpp>

#include "Points.h"

using Point = LinkSenseAIFI::Point;
using Points = LinkSenseAIFI::Points;

enum class BatchSideType {
    SideTop,
    SideBot
};

struct BaseInfo {
    BaseInfo() = default;

    short DefectUID;
    std::string DefectName;
    std::array<short, 4> DefectColor;

    YAS_DEFINE_STRUCT_SERIALIZE_NVP("BaseInfo",
        ("DefectUID", DefectUID),
        ("DefectName", DefectName),
        ("DefectColor", DefectColor)
    )
};

struct PolygonDefectInfo{
    PolygonDefectInfo() = default;

    BaseInfo Info;

    Points Polygon;
    Point CenterPoint;
    short Radius;
    short Summary;
    std::string PanelQRcode;

    YAS_DEFINE_STRUCT_SERIALIZE_NVP("PolygonDefectInfo",
        ("BaseInfo", Info),
        ("Polygon", Polygon),
        ("CenterPoint", CenterPoint),
        ("Radius", Radius),
        ("Summary", Summary),
        ("PanelQRcode", PanelQRcode)
    )
};

struct ImgDefectInfo {
    ImgDefectInfo() = default;

    BaseInfo Info;

    short SmallImgIdx;
    short DefectIdx;

    YAS_DEFINE_STRUCT_SERIALIZE_NVP("ImgDefectInfo",
        ("BaseInfo", Info),
        ("SmallImgIdx", SmallImgIdx),
        ("DefectIdx", DefectIdx)
    )
};

class 
#ifdef NEEDEXPOTR
    __declspec(dllexport)
#endif
BatchDefectInfo
{
public:
    explicit BatchDefectInfo() = default;
    ~BatchDefectInfo() {}

    void makeVirtualData();
    void wait();

    void toJson(const std::string& jsonPath = std::filesystem::current_path().generic_string() + "/BatchDefectInfo.json");
    void fromJson(const std::string& jsonPath = std::filesystem::current_path().generic_string() + "/BatchDefectInfo.json");

    YAS_DEFINE_STRUCT_SERIALIZE_NVP("BatchDefectInfo",
                                    ("IsVaild", IsVaild),
                                    ("DefectToleranceRange", m_DefectToleranceRange),
                                    ("DefectInfo", m_DefectInfo),
                                    ("DefectImgInfo", m_DefectImgInfo)
                                    )
private:
    bool IsVaild{ false };
    std::map<std::string, short> m_DefectToleranceRange;
    std::map<std::string, std::map<BatchSideType, std::vector<PolygonDefectInfo>>> m_DefectInfo;
    std::map<std::string, std::map<BatchSideType, std::vector<ImgDefectInfo>>> m_DefectImgInfo;
};

// class Widget : public QWidget
// {
//     Q_OBJECT

// public:
//     explicit Widget(QWidget *parent = nullptr);
//     ~Widget();

//     void readMatStdPath(const std::string &strPath);

//   protected:
//     void paintEvent(QPaintEvent *event);

//     QPainterPath path;
// };
#endif // WIDGET_H
