#pragma once
#include <QWidget>
#include <QDebug>
#include <QPushButton>
#include <QGridLayout>

#include "ModulesIVS/QGraphImage.h"
#include "SideInspectResultInfo.h"

namespace IVS = LinkSenseAIFI::ModulesIVS;
namespace InsPersister = LinkSenseAIFI::InsPersister;

class GraphImgViewer : public QWidget
{
    Q_OBJECT
  public:
    explicit GraphImgViewer(QWidget* parent = nullptr);

  protected Q_SLOTS:
    void sltSelectImg();
    void sltSelectAiriOrAipi();
    void sltSelectFilter();

  private:
    QGridLayout* m_ObjLayout{nullptr};
    QPushButton* m_SelectImg{nullptr};
    QPushButton* m_SelectAiriOrAipi{nullptr};
    QPushButton* m_SelectFilter{nullptr};
    QPushButton* m_SelectSide{nullptr};
    IVS::QGraphImage* m_Viewer{nullptr};

    LinkSenseAIFI::InsBaseEnum::BoardSideType m_SideType{LinkSenseAIFI::InsBaseEnum::BoardSideType::SideTop};
    std::vector<InsPersister::LabeledRegion> m_Filter;
    InsPersister::SideInspectResultInfo m_AiriAipi;
};
