#pragma once
#include <QObject>
#include <QDebug>

#include "LibExporter.h"

class LIB_EXPORT base
{
public:
    explicit base() = default;
protected:
    virtual void add(){
        qDebug() << "base add.";
    }
};

class LIB_EXPORT subClass : public base
{
public:
    explicit subClass(){
        add();
    }
// protected:
    /*virtual void add() override{
        qDebug() << "SubClass add.";
        base::add();
    }*/
};

class LIB_EXPORT testClass : public QObject
{
    Q_OBJECT
  public:
    explicit testClass(QObject* parent = nullptr) {
        subClass test;
    }
};
