#include "GraphImgViewer.h"

#include "InsBaseEnum.h"
#include "ModulesIVS/ModelMissDetectInfo.h"
#include "ModulesIVS/SourcePanelInfo.h"

#include <QFileDialog>
#include <QRect>

GraphImgViewer::GraphImgViewer(QWidget* parent)
    : QWidget(parent)
{
    m_ObjLayout = new QGridLayout(this);

    m_SelectImg = new QPushButton("选择背景图");
    m_SelectAiriOrAipi = new QPushButton("选择Dft点位");
    m_SelectFilter = new QPushButton("选择filter文件");
    m_SelectSide = new QPushButton("Top");
    m_Viewer = new IVS::QGraphImage();

    m_ObjLayout->addWidget(m_SelectImg, 0, 0, 1, 1);
    m_ObjLayout->addWidget(m_SelectAiriOrAipi, 0, 1, 1, 1);
    m_ObjLayout->addWidget(m_SelectFilter, 0, 2, 1, 1);
    m_ObjLayout->addWidget(m_SelectSide, 0, 3, 1, 1);
    m_ObjLayout->addWidget(m_Viewer, 1, 0, 10, 4);

    connect(m_SelectImg, &QPushButton::clicked, this, &GraphImgViewer::sltSelectImg);
    connect(m_SelectAiriOrAipi, &QPushButton::clicked, this, &GraphImgViewer::sltSelectAiriOrAipi);
    connect(m_SelectFilter, &QPushButton::clicked, this, &GraphImgViewer::sltSelectFilter);
    connect(m_SelectSide, &QPushButton::clicked, this, [&](){
        m_SelectSide->setText(m_SelectSide->text() == "Bot" ? "Top" : "Bot");
    });
}

void GraphImgViewer::sltSelectImg()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "D:/ArchiverAIFI");

    qDebug() << "fileName:" << fileName;
    QImage img(fileName);

    m_Viewer->showImage(img);
}

void GraphImgViewer::sltSelectAiriOrAipi()
{
    QPen pen;
    pen.setWidth(5);
    pen.setColor(Qt::green);
    auto w = -m_Viewer->Image()->Pix().width() / 2;
    auto h = -m_Viewer->Image()->Pix().height() / 2;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Airi/Aipi"), "D:/ArchiverAIFI");

    (void)m_AiriAipi.deserialize(fileName.toStdString());
    std::vector<InsPersister::SideInspectResultInfo::ResultGroupQT> defectGroupsInfo;
    if (m_AiriAipi.getDefectGroups(defectGroupsInfo).isOK()) {
        for (size_t i = 0; i < defectGroupsInfo.size(); ++i) {
            for (const auto& dftPoi : defectGroupsInfo.at(i).defectInfo.SubDefects()) {
                QPolygonF transformPoly;
                IVS::ModelMissDetectInfo::TransformTheCoordinateSystem(
                    transformPoly, defectGroupsInfo.at(i).defectInfo.Roi2CamQt().topLeft() /*dftPoi.Roi2CamQt().topLeft()*/, dftPoi.ContourQt());
                qDebug() << "transformPoly:" << transformPoly;
                QGraphicsPolygonItem* itm = new QGraphicsPolygonItem(transformPoly, m_Viewer->Image());
                itm->setPen(pen);
                itm->setPos(w, h);
            }
        }
    }
}

void GraphImgViewer::sltSelectFilter()
{
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Airi/Aipi"), "D:/ArchiverAIFI");

    QPen pen;
    pen.setWidth(5);
    pen.setColor(Qt::white);
    auto w = -m_Viewer->Image()->Pix().width() / 2;
    auto h = -m_Viewer->Image()->Pix().height() / 2;

    auto side = m_SelectSide->text() == "Top" ? LinkSenseAIFI::InsBaseEnum::BoardSideType::SideTop : LinkSenseAIFI::InsBaseEnum::BoardSideType::SideBot;
    IVS::SourcePanelInfo::readFilterFileInfo(m_Filter, side, fileName.toStdString());
    for (size_t i = 0; i < m_Filter.size(); ++i) {
        auto itmPolyf = m_Filter.at(i).Region().toQPolyF();
        qDebug() << "filter:" << itmPolyf;
        QGraphicsPolygonItem* itm = new QGraphicsPolygonItem(itmPolyf, m_Viewer->Image());
        itm->setPen(pen);
        itm->setPos(w, h);
    }
}
