#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "Logger.h"
#include "TryCatcher.h"
#include "Logger.h"
#include "ExecHelper.h"
#include "ParamSets.h"
#include "DftAndModInit.h"
#include <QFile>
//#include "qxlsx/xlsxdocument.h"

using Logur = LinkSenseAIFI::Logur;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LinkSenseAIFI::ExecHelper::judger(uiInit());
    bindSignalAndSlots();
    // excel2json();
    // conver();
    LinkSenseAIFI::DcsParam::ParamSetMetal metalParam;
    LinkSenseAIFI::ExecHelper::judger(metalParam.load("D:/guanzhaohang/desktop/InsParamFiles/sourceJson/MetalParamSet.json"));
    LinkSenseAIFI::DcsParam::DftAndModInit::s_Defects = metalParam.DefectDict();
    LinkSenseAIFI::DcsParam::DftAndModInit::s_Modules = metalParam.ModuleDict();
    LinkSenseAIFI::DcsParam::DftAndModInit::s_CustomDftList = metalParam.CustomDftList();

}

MainWindow::~MainWindow()
{
    delete ui;
}

LinkSenseAIFI::Status<> MainWindow::uiInit()
{
    return LinkSenseAIFI::TryCatcher::exec(LinkSenseAIFI::Logur::obj().Ptr(), "", [&] () -> void {
        m_CentralWidget = new QWidget();

        m_VLayout = new QVBoxLayout();
        m_ParamWidnowHLayout = new QHBoxLayout();
        m_ButtonHLayout = new QHBoxLayout();
        m_SerializeFilePathEdit = new QLineEdit();
        m_SerializeFilePathEdit->setText("D:/guanzhaohang/desktop/InsParamFiles/modifiedJson");
        m_DeserializeFilePathEdit = new QLineEdit();
        m_DeserializeFilePathEdit->setText("D:/guanzhaohang/desktop/InsParamFiles/sourceJson");
        m_VLayout->addLayout(m_ParamWidnowHLayout, 20);
        m_VLayout->addLayout(m_ButtonHLayout, 1);

        m_ParamWindow = new ParamSetWindow("D:/ArchiverAIFI/Document");
        m_FileToUIButton = new QToolButton();
        m_FileToUIButton->setText("FileToUI");
        m_UIToFileButton = new QToolButton();
        m_UIToFileButton->setText("UIToFile");
        m_ModuleList = new QComboBox();
        m_ModuleList->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        m_DefectList = new QComboBox();
        m_DefectList->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        m_FilterButton = new QToolButton();
        m_FilterButton->setText("过滤");
        m_NoFilterButton = new QToolButton();
        m_NoFilterButton->setText("取消过滤");

        m_ParamWidnowHLayout->addWidget(m_ParamWindow);
        m_ButtonHLayout->addWidget(m_DeserializeFilePathEdit);
        m_ButtonHLayout->addWidget(m_FileToUIButton);
        m_ButtonHLayout->addWidget(m_UIToFileButton);
        m_ButtonHLayout->addWidget(m_SerializeFilePathEdit);
        m_ButtonHLayout->addWidget(m_ModuleList);
        m_ButtonHLayout->addWidget(m_DefectList);
        m_ButtonHLayout->addWidget(m_FilterButton);
        m_ButtonHLayout->addWidget(m_NoFilterButton);

        m_CentralWidget->setLayout(m_VLayout);
        this->setCentralWidget(m_CentralWidget);
    });
}

void MainWindow::bindSignalAndSlots()
{
    connect(m_FileToUIButton, &QToolButton::clicked, [this] () {
        // QFile oldParamfile("./ResistParamSetMode.json");
        // if (!oldParamfile.exists()) { return false; }
        // if (!oldParamfile.open(QIODevice::ReadOnly)) { return false; };
        // QByteArray oldParamData = oldParamfile.readAll();
        // oldParamfile.close();
        // yas::shared_buffer dataBuf{oldParamData.data(), static_cast<size_t>(oldParamData.size())};
        // LinkSenseAIFI::DcsParam::ParamSetResistOld oldParam;
        // oldParam.deserialize(dataBuf);
        // LinkSenseAIFI::DcsParam::ParamSetResist newParam;
        // newParam.conver(oldParam);

        // const auto newData = newParam.serialize();
        // QFile newParamSaveFile("./ResistParamSetMode.json");
        // if (!newParamSaveFile.open(QIODevice::WriteOnly)) { return false; };
        // QByteArray data(newData.data.get(), newData.size);
        // if (newParamSaveFile.write(data) == -1) { return false; }
        // newParamSaveFile.close();

        auto state = LinkSenseAIFI::TryCatcher::exec("", [&] () -> void {
            LinkSenseAIFI::ExecHelper::judger(m_ParamWindow->readParamFiles(m_DeserializeFilePathEdit->text(), m_DeserializeFilePathEdit->text(), m_DeserializeFilePathEdit->text()));
            for (const auto &defect : LinkSenseAIFI::DcsParam::DftAndModInit::s_Defects.DefectList()) {
                m_DefectList->addItem(QString::fromStdString(defect.name));
                Logur::i().info("m_DefectList :{}", defect.name);
            }
            Logur::i().info("===================");
            for (const auto &module : LinkSenseAIFI::DcsParam::DftAndModInit::s_Modules.ModuleList()) {
                m_ModuleList->addItem(QString::fromStdString(module.name));
                Logur::i().info("m_ModuleList :{}", module.name);
            }
        });
        if (not state.isOK()) {
            LinkSenseAIFI::Logur::obj().error("> error:{}", state.info());
        }
        // D:\caoxumian\tmp\Params
        // LinkSenseAIFI::DcsParam::ParamSets params;
        // const auto ret = ParamSetWindow::readParamFiles("D:/caoxumian/tmp/Params", params);
        // if (not ret.isOK()) {
        //     qDebug() << "PRINT: " << __FILE__ << __LINE__ << "error unkonw...";
        // }
    });
    connect(m_UIToFileButton, &QToolButton::clicked, [this] () {
        LinkSenseAIFI::DcsParam::ParamSets paramsets;
        LinkSenseAIFI::ExecHelper::judger(m_ParamWindow->writeParamFiles("./"));
    });
    connect(m_FilterButton, &QToolButton::clicked, [this] () {
        QStringList defects, modules;
        defects << m_DefectList->currentText();
        modules << m_ModuleList->currentText();
        // modules << "油墨下大铜面" << "油墨下线路" << "油墨下基材" << "焊盘周边基材" << "连通孔环" << "小焊盘";
        qDebug() << "PRINT: " << __FILE__ << __LINE__ << "defects:" << defects;
        qDebug() << "PRINT: " << __FILE__ << __LINE__ << "modules:" << modules;
        m_ParamWindow->setFilterByDefectInfo(true, modules, defects, LinkSenseAIFI::DcsParam::RegionType::BoardEdge);
    });
    connect(m_NoFilterButton, &QToolButton::clicked, [this] () {
        m_ParamWindow->setFilterByDefectInfo(false, {}, {});
    });
}

void MainWindow::excel2json()
{
    //using namespace LinkSenseAIFI::DcsParam;
    //using Logi = LinkSenseAIFI::DcsParam::ParamInterface::ConditionSelecter;
    //ParamSetResist resistParam, resistParamNew;
    //const auto ret = resistParam.load("./ResistParamSet.json");
    //if (!ret.isOK()) {
    //    qDebug() << " loas resist param file failed!!!";
    //    return;
    //}
    //resistParamNew.setResistParamNameTranslateCN(resistParam.ResistParamNameTranslateCN());
    //resistParamNew.setDefectDict(resistParam.DefectDict());
    //resistParamNew.setModuleDict(resistParam.ModuleDict());
    //DftAndModInit::s_Modules = resistParam.ModuleDict();
    //DftAndModInit::s_Defects = resistParam.DefectDict();

    //std::unordered_map<QString, UsePostCondition> use_post = {{"Close", UsePostCondition::Close},
    //                                                          {"DirectlyFiltration", UsePostCondition::DirectlyFiltration},
    //                                                          {"ConditionalFiltering", UsePostCondition::ConditionalFiltering}};
    //std::unordered_map<QString, Logi>logi_list = {{"Area", Logi::Area}, {"WidthAndHeight", Logi::WidthAndHeight}, {"AreaAndWidht", Logi::AreaAndWidht},
    //                                               {"AreaAndHeight", Logi::AreaAndHeight}, {"WidthOrHeight", Logi::WidthOrHeight}, {"AreaOrWidht", Logi::AreaOrWidht},
    //                                               {"AreaOrHeight", Logi::AreaOrHeight}, {"AreaOrWidthAndHeight", Logi::AreaOrWidthAndHeight},
    //                                               {"WidthOrAreaAndHeight", Logi::WidthOrAreaAndHeight}, {"HeightOrWidthAndArea", Logi::HeightOrWidthAndArea},
    //                                               {"AreaAndWidthOrHeight", Logi::AreaAndWidthOrHeight}, {"WidthAndAreaOrHeight", Logi::WidthAndAreaOrHeight},
    //                                               {"HeightAndAreaOrWidth", Logi::HeightAndAreaOrWidth}, {"Or", Logi::Or}, {"And", Logi::And}};

    //auto core = [&] (const QString &excelFilePath, const RegionType &regionType) {
    //    using namespace QXlsx;
    //    Document xlsxR(excelFilePath);
    //    if (not xlsxR.load()) // load excel file
    //    {
    //        qDebug() << "xlsx load failed!";
    //        return;
    //    }
    //    for (int row = 2; row <= xlsxR.dimension().rowCount(); ++row) {
    //        std::string defectCode;
    //        ModuleSetKey moduleSet;
    //        ParamGlob globParam;
    //        DcsnParamsResist param;
    //        for (int col = 1; col <= xlsxR.dimension().columnCount(); ++col) {
    //            QXlsx::Cell *cell = xlsxR.cellAt(row, col);
    //            switch (col) {
    //            case 1: /*value: "区域"*/
    //                param.setHierarchyClass(cell->value().toString().toStdString());
    //                break;
    //            case 2: /*value: "缺陷名称"*/
    //                defectCode = DftAndModInit::s_Defects.name2Code(cell->value().toString().toStdString());
    //                break;
    //            case 3: /*value: "部件组合"*/
    //            {
    //                const auto modules = cell->value().toString().split("&");
    //                std::vector<std::string> modulesVec;
    //                for (const auto &str : modules) { modulesVec.push_back(str.toStdString()); };
    //                moduleSet = ModuleSetKey(modulesVec, DftAndModInit::s_Modules, ModuleSetKey::KeyMode::NameCN);
    //                break;
    //            }
    //            case 4: /*value: "SideDefectAreaLimit"*/
    //                globParam.setSideDefectAreaLimit(cell->value().toUInt());
    //                break;
    //            case 5: /*value: "SideDefectCountLimit"*/
    //                globParam.setSideDefectCountLimit(cell->value().toUInt());
    //                break;
    //            case 6: /*value: "GlobParam.DistanceWithOtherBlob"*/
    //                globParam.setDistanceWithOtherBlob(cell->value().toUInt());
    //                break;
    //            case 7: /*value: "GlobParam.OtherBlobNumberNearbyLimit"*/
    //                globParam.setOtherBlobNumberNearbyLimit(cell->value().toUInt());
    //                break;
    //            case 8: /*value: "UsePost"*/
    //                param.setUsePost(use_post.at(cell->value().toString()));
    //                break;
    //            case 9: /*value: "InterAreaRatio"*/
    //                param.setInterAreaRatio(cell->value().toFloat());
    //                break;
    //            case 10:  /*value: "InterAreaRatioLarge"*/
    //                param.setInterAreaRatioLarge(cell->value().toFloat());
    //                break;
    //            case 11:  /*value: "InterWidthRatio"*/
    //                // param.setInterWidthRatio(cell->value().toFloat());
    //                break;
    //            case 12:  /*value: "InterHeightRatio"*/
    //                // param.setInterHeightRatio(cell->value().toFloat());
    //                break;
    //            case 13:  /*value: "LimitExpandFactorBreakaway"*/
    //                // param.setLimitExpandFactorBreakaway(cell->value().toFloat());
    //                break;
    //            case 14:  /*value: "ReceptiveField"*/
    //                param.setReceptiveField(cell->value().toUInt());
    //                break;
    //            case 15:  /*value: "DefectDistanceNearbyPad"*/
    //                param.setDefectDistanceNearbyPad(cell->value().toUInt());
    //                break;
    //            case 16:  /*value: "DefectDistanceNearbyCircuit"*/
    //                param.setDefectDistanceNearbyCircuit(cell->value().toUInt());
    //                break;
    //            case 17:  /*value: "RejectedDefectNames"*/
    //            {
    //                auto defectStr = cell->value().toString();
    //                defectStr = defectStr.remove("[");
    //                defectStr = defectStr.remove("]");
    //                defectStr = defectStr.remove("'");
    //                defectStr = defectStr.remove("'");
    //                defectStr = defectStr.remove(" ");
    //                const auto defectList = defectStr.split(",");
    //                if (defectList.size() == 1 and defectList[0].isEmpty()) { break; };
    //                std::vector<std::string> defectCodes, defectNames;
    //                for (const auto &defectName : defectList) {
    //                    const auto defectNameStd = defectName.toStdString();
    //                    defectNames.push_back(defectNameStd);
    //                    defectCodes.push_back(DftAndModInit::s_Defects.name2Code(defectNameStd));
    //                }
    //                param.setRejectedDefectCodes({defectCodes});
    //                // param.setRejectedDefectNames({defectNames});
    //                break;
    //            }
    //            case 18:  /*value: "RejectedNearByModuleNames"*/
    //            {
    //                auto moduleStr = cell->value().toString();
    //                moduleStr = moduleStr.remove("[");
    //                moduleStr = moduleStr.remove("]");
    //                moduleStr = moduleStr.remove("'");
    //                moduleStr = moduleStr.remove("'");
    //                moduleStr = moduleStr.remove(" ");
    //                const auto moduleList = moduleStr.split(",");
    //                if (moduleList.size() == 1 and moduleList[0].isEmpty()) { break; };
    //                std::vector<std::string> moduleNames;
    //                std::vector<uchar> moduleIDs;
    //                for (const auto &moduleName : moduleList) {
    //                    const auto moduleNameStd = moduleName.toStdString();
    //                    moduleNames.push_back(moduleNameStd);
    //                    moduleIDs.push_back(LinkSenseAIFI::ExecHelper::judger(DftAndModInit::s_Modules.name2ID(moduleNameStd)));
    //                }
    //                // param.setRejectedNearByModuleNames({moduleNames});
    //                param.setRejectedNearByModuleIDs({moduleIDs});
    //                break;
    //            }
    //            break;
    //            case 19:  /*value: "FilterCondition"*/
    //                param.setFilterCondition(logi_list.at(cell->value().toString()));
    //                break;
    //            case 20:  /*value: "DefectAreaLimit"*/
    //                param.setDefectAreaLimit(cell->value().toUInt());
    //                break;
    //            case 21:  /*value: "DefectMinSizeLimit"*/
    //                param.setDefectMinSizeLimit(cell->value().toUInt());
    //                break;
    //            case 22:  /*value: "DefectMaxSizeLimit"*/
    //                param.setDefectMaxSizeLimit(cell->value().toUInt());
    //                break;
    //            }
    //        }
    //        param.setGlobParam(globParam);
    //        if (!LinkSenseAIFI::ExecHelper::judger(resistParamNew.insert(regionType, QString::fromStdString(defectCode), moduleSet, param))){
    //            qDebug() << "defectCode:" << QString::fromStdString(defectCode) << " moduleSet:" << QString::fromStdString(moduleSet.key2ModuleStrs(ModuleSetKey::KeyMode::NameCN)) << " failed!!!";
    //        }
    //    }
    //};
    //core("./resistParamSet_class.xlsx", RegionType::Piece);
    //core("./resistParamSet_class_breakaway.xlsx", RegionType::BoardEdge);
    //LinkSenseAIFI::ExecHelper::judger(resistParamNew.save("./ResistParamSetNew.json"));

}

void MainWindow::conver()
{
    LinkSenseAIFI::DcsParam::ParamSetResist resistParam, resistParamMode, resistParamModeClass;
    const auto ret = resistParam.load("./ResistParamSet.json");
    const auto retMode = resistParamMode.load("./ResistParamSetMode.json");
    if (!ret.isOK() || !retMode.isOK()) {
        qDebug() << " loas resist param file failed!!!";
        return;
    }
    using namespace LinkSenseAIFI::DcsParam;
    resistParamMode.setResistParamNameTranslateCN(resistParam.ResistParamNameTranslateCN());
    resistParamMode.setDefectDict(resistParam.DefectDict());
    resistParamMode.setModuleDict(resistParam.ModuleDict());
    resistParamModeClass.setResistParamNameTranslateCN(resistParam.ResistParamNameTranslateCN());
    resistParamModeClass.setDefectDict(resistParam.DefectDict());
    resistParamModeClass.setModuleDict(resistParam.ModuleDict());
    DftAndModInit::s_Modules = resistParam.ModuleDict();
    DftAndModInit::s_Defects = resistParam.DefectDict();
    for (auto &region : resistParamMode.RegionDftCode2ModSetDcsnParamsResist()) {
        for (auto &dft2Param : region.second) {
            for (auto &module2param : dft2Param.second) {
                if (resistParam.contains(region.first, dft2Param.first, module2param.first)) {
                    const auto classParam = resistParam.getParamsResistFromModuleSetNDefectCode(region.first, dft2Param.first, module2param.first);
                    const auto ret = resistParamModeClass.insert(region.first, QString::fromStdString(dft2Param.first), module2param.first, classParam);
                    if (not ret.isOK()) {
                        qDebug() << "insert failed!";
                    }
                    continue;
                }
                const auto ret = resistParamModeClass.insert(region.first, QString::fromStdString(dft2Param.first), module2param.first, module2param.second);
                if (not ret.isOK()) { qDebug() << "insert failed!"; };
            }
        }
    }
    resistParamModeClass.save("./ResistParamSetModeClass.json");
}
