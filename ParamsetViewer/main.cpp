#include "MainWindow.h"
#include "Logger.h"
#include <QApplication>

int main(int argc, char *argv[])
{
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
    MainWindow w;
    w.show();
    return a.exec();
}
