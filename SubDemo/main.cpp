//#include "opencv2/imgcodecs.hpp"

#include <QApplication>
// #include "Logger.h"

#include "testclass.h"
#include "widget.h"
#include "threeparty/crow_all.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // LinkSenseAIFI::LogParam libParam{LinkSenseAIFI::LogMode::Both, "./Log", "LinkSenseIVS", "Running", ".log", 23, 59, 14};
    // libParam.level = LinkSenseAIFI::LogLevel::debug;
    // LinkSenseAIFI::Logger& logger = LinkSenseAIFI::Logur::create(libParam);

    // testClass cla;

    // Widget widget;
    // widget.show();

    crow::SimpleApp app;

    CROW_ROUTE(app, "/hello")([](){
        return "Hello, Crow on localhost!";
    });

    // 只监听本地回环地址127.0.0.1，端口8080
    app.bindaddr("127.0.0.1").port(8080).multithreaded().run();

    return a.exec();
}
