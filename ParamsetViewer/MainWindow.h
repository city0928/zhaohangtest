#pragma once

#include <QMainWindow>
#include "Status.h"
#include "ParamSetWindow.h"
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QMainWindow>

#include <QtGlobal>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    using ParamSetWindow = LinkSenseAIFI::DcsParam::ParamSetWindow;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    [[nodiscard]] LinkSenseAIFI::Status<> uiInit();
    void bindSignalAndSlots();
    void excel2json();
    void conver();

private:
    QWidget *m_CentralWidget{nullptr};
    QVBoxLayout *m_VLayout{nullptr};
    QHBoxLayout *m_ParamWidnowHLayout{nullptr};
    QHBoxLayout *m_ButtonHLayout{nullptr};
    QToolButton *m_FileToUIButton{nullptr};
    QToolButton *m_UIToFileButton{nullptr};
    QToolButton *m_FilterButton{nullptr};
    QToolButton *m_NoFilterButton{nullptr};
    QLineEdit *m_SerializeFilePathEdit{nullptr};
    QLineEdit *m_DeserializeFilePathEdit{nullptr};
    ParamSetWindow *m_ParamWindow{nullptr};
    QComboBox *m_ModuleList{nullptr};
    QComboBox *m_DefectList{nullptr};
private:
    Ui::MainWindow *ui;
};
