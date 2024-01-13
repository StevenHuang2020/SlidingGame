#pragma once
#include <QDialog>
#include "ui_hint_dlg.h"
#include "setting_dlg.h"

class HintWnd : public QDialog
{
    Q_OBJECT

public:
    HintWnd(QWidget* parent = Q_NULLPTR);
    ~HintWnd(){};

public:
    void InitHint(SettingDlg::SettingDlgData& data);
    void clear();
    void update_labels(uint8_t col, uint8_t row);
    void remove_labels();

private:
    Ui::Hint_Dialog ui;
    std::vector<QLabel*> m_NumLabels;
    std::unique_ptr<QLabel> m_ImageLabel;
};
