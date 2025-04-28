//#include "opencv2/imgcodecs.hpp"

#include <QApplication>
#include "Logger.h"

#include "testclass.h"
#include "TestComboBoxUi.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LinkSenseAIFI::LogParam libParam{LogMode::Both, "./Logs", "test", "logsName"};
    libParam.level = LinkSenseAIFI::LogLevel::trace;
    LinkSenseAIFI::Logur::create(libParam);

    // TestTryCatcher cla;

    TestComboBoxUi ui;
    ui.show();

    return a.exec();
}
