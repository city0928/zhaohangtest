#pragma once
#include <QObject>
#include <QDebug>

#include <iostream>

#include "TryCatcher.h"
#include "Logger.h"

using namespace LinkSenseAIFI;

class TestTryCatcher
{
  public:
    explicit TestTryCatcher(){
        const auto& state = TryCatcher::exec(Logur::i().Ptr(), "异常错误！", [&](){

            int* i = nullptr;
            int* j = new int(10);
            delete j;

            std::cout << *i << " " << *j << "\n";
        });
    }

};
