#include "MainWindow.h"
#include "Logger.h"
#include <QApplication>
#include <windows.h>
//#include "ConfigRangeWidget.h"
//#include "CustomBaseType.h"
#include "yas/serialize.hpp"
#include "yas/std_types.hpp"

//#include "TestGadget.h"

using namespace LinkSenseAIFI;
using namespace LinkSenseAIFI::DcsParam;


//template <class Type>
//class CustomIntFloatDouType
//{
//  public:
//    explicit CustomIntFloatDouType() noexcept{
//    }
//
//    explicit CustomIntFloatDouType(const Type& vaule, const Type& minValue, const Type& maxValue) noexcept
//        : m_Vaule{vaule},
//          m_MinVaule{minValue},
//          m_MaxVaule{maxValue}
//    {
//    }
//
//  private:
//    Type m_Vaule;
//    Type m_MinVaule;
//    Type m_MaxVaule;
//
//  public:
//    YAS_DEFINE_STRUCT_SERIALIZE_NVP("", ("Vaule", m_Vaule), ("MinVaule", m_MinVaule), ("MaxVaule", m_MaxVaule));
//};

#include <QModelIndex>


int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    LinkSenseAIFI::LogParam param{LinkSenseAIFI::LogMode::Both, "./log", "ParamsetViewer", "paramSet_gzh"};
    param.level = LinkSenseAIFI::LogLevel::debug;
    LinkSenseAIFI::Logger &logger = LinkSenseAIFI::Logur::create(param);
    logger.info("> Tester, Start...");
    auto func = []<typename T>(T &w){
        qDebug() << "PRINT: " << __FILE__ << __LINE__ << w;
    };
    QString str("hello word!");
    func(str);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //w.clickUi();

    //using ConfigRangeWidget = LinkSenseAIFI::DcsParam::ConfigRangeWidget;
    //ConfigRangeWidget config;
    //config.show();

    //LinkSenseAIFI::DcsParam::TestYas test1;
    //std::string str333 = "D:/guanzhaohang/desktop/qwe.json";
    //test1.serializeJson(str333);

    // cTestBool testAAA{false};
    // QMetaObject obj = cTestBool::staticMetaObject;

    // QModelIndex index;
    // std::vector<cTestBool> vector;
    // index.data().canConvert<cTestBool>();

 /*   FloatSpinBox *spinbox = new FloatSpinBox();
       spinbox->setParamValue(cfloat{0.1, 0, 10});
       spinbox->show();*/

    return a.exec();
}
