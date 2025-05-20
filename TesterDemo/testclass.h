#pragma once
#include <QObject>
#include <QDebug>

class base
{
public:
    explicit base() = default;
protected:
    virtual void add(){
        qDebug() << "base add.";
    }
};

class subClass : public base
{
public:
    explicit subClass(){
        add();
    }
protected:
    /*virtual void add() override{
        qDebug() << "SubClass add.";
        base::add();
    }*/
};

class testClass : public QObject
{
    Q_OBJECT
  public:
    explicit testClass(QObject* parent = nullptr) {
        subClass test; 
    }
};
