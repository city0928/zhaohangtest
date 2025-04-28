#ifndef TESTCOMBOBOXUI_H
#define TESTCOMBOBOXUI_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QPushButton>

class TestComboBoxUi : public QWidget
{
    Q_OBJECT
  public:
    explicit TestComboBoxUi(QWidget* parent = nullptr);

    QComboBox *m_ComBox{nullptr};
    QPushButton *m_ClearBtn{nullptr};
    QPushButton *m_AddBtn{nullptr};
};

#endif  // TESTCOMBOBOXUI_H
