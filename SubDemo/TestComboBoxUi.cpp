#include "TestComboBoxUi.h"

#include <QGridLayout>

TestComboBoxUi::TestComboBoxUi(QWidget* parent)
    : QWidget{parent}
{
    m_ComBox = new QComboBox();
    m_ClearBtn = new QPushButton("clear");
    m_AddBtn = new QPushButton("add");

    auto layout = new QGridLayout(this);
    layout->addWidget(m_ComBox);
    layout->addWidget(m_ClearBtn);
    layout->addWidget(m_AddBtn);

    connect(m_ComBox, &QComboBox::currentIndexChanged, this, [&](){
        qDebug() << "currentIndexChanged";
    });

    connect(m_ClearBtn, &QPushButton::clicked, this, [&](){
        m_ComBox->clear();
    });

    connect(m_AddBtn, &QPushButton::clicked, this, [&](){
        m_ComBox->addItems(QStringList{"1", "2", "3"});
    });

}
