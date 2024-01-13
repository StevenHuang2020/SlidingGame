#include "hint_dlg.h"

HintWnd::HintWnd(QWidget* parent) : QDialog(parent)
{
    ui.setupUi(this);
    setLayout(ui.gridLayout);

    Qt::WindowFlags flags = windowFlags();
    flags &= ~Qt::WindowSystemMenuHint;
    flags &= Qt::WindowMinMaxButtonsHint;
    flags &= Qt::WindowStaysOnTopHint;
    flags &= Qt::CustomizeWindowHint;
    flags &= Qt::Window;

    setWindowFlags(flags);
}

void HintWnd::InitHint(SettingDlg::SettingDlgData& data)
{
    if (data.type == GameType::E_Number)
    {
        clear();

        QString common_style = "border-style: outset;	\
					border-width: 1px;	\
					border-radius: 5px;	\
					border-color: beige;	\
					padding: 0px; ";

        for (uint8_t i = 0; i < data.col * data.row; i++)
        {
            QLabel* pLabel = new QLabel(this);
            pLabel->setScaledContents(true);
            //pLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
            pLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            pLabel->setAlignment(Qt::AlignCenter);

            QString style = "";

            pLabel->setText(QString::number(i));

            style = "background-color: ";
            style += data.color_active;
            style += ";";
            style += "font: 28pt;";
            style += "font-style: italic; font-style: bold; font-family: Consolas;";

            pLabel->setStyleSheet(style + common_style);
            m_NumLabels.push_back(pLabel);
        }

        update_labels(data.col, data.row);
    }
    else if (data.type == GameType::E_Image)
    {
        if (!m_ImageLabel)
            m_ImageLabel = std::make_unique<QLabel>(this);

        m_ImageLabel->setScaledContents(true);
        //m_ImageLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        m_ImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        m_ImageLabel->setAlignment(Qt::AlignCenter);

        QImage img(data.puzzle_image);
        m_ImageLabel->setPixmap(QPixmap::fromImage(img));

        remove_labels();
        auto pGrid = ui.gridLayout;
        pGrid->addWidget(m_ImageLabel.get(), 0, 0);
    }
}

void HintWnd::clear()
{
    for (auto& i : m_NumLabels)
        delete i;
    m_NumLabels.clear();
}

void HintWnd::update_labels(uint8_t col, uint8_t row)
{
    remove_labels();

    auto pGrid = ui.gridLayout;
    auto size = m_NumLabels.size();
    for (int i = 0; i < size; i++)
    {
        pGrid->addWidget(m_NumLabels[i], i / col, i % col);
    }
}

void HintWnd::remove_labels()
{
    auto pGrid = ui.gridLayout;
    for (auto& pLabel : m_NumLabels)
    {
        pGrid->removeWidget(pLabel);
    }
    pGrid->update();
}
