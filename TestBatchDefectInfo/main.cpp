
#include <QApplication>
#include "opencv2/imgcodecs.hpp"
#include "Logger.h"

#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LinkSenseAIFI::LogParam libParam{ LinkSenseAIFI::LogMode::Both, "./Log", "LinkSenseIVS", "Running", ".log", 23, 59, 14 };
    libParam.level = LinkSenseAIFI::LogLevel::debug;
    LinkSenseAIFI::Logger& logger = LinkSenseAIFI::Logur::create(libParam);

    BatchDefectInfo info;
    info.makeVirtualData();
    //info.fromJson();
    //info.wait();

    return a.exec();
}
