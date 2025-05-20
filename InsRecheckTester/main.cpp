#include "Logger.h"
#include <QApplication>
#include <windows.h>
#include "yas/serialize.hpp"
#include "yas/std_types.hpp"

using namespace LinkSenseAIFI;

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    LinkSenseAIFI::LogParam param{LinkSenseAIFI::LogMode::Both, "./log", "ParamsetViewer", "release_cxm"};
    param.level = LinkSenseAIFI::LogLevel::debug;
    LinkSenseAIFI::Logger &logger = LinkSenseAIFI::Logur::create(param);
    logger.info("> Tester, Start...");
    auto func = []<typename T>(T &w){
        qDebug() << "PRINT: " << __FILE__ << __LINE__ << w;
    };
    QString str("hello word!");
    func(str);
    QApplication a(argc, argv);

    return a.exec();
}
