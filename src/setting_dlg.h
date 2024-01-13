#pragma once
#include "ui_setting_dlg.h"
#include <QColorDialog>
#include <QDialog>
#include <QFileDialog>

enum class GameType
{
    E_Number,
    E_Image
};

class SettingDlg : public QDialog
{
    Q_OBJECT

public:
    SettingDlg(QWidget* parent = Q_NULLPTR);
    ~SettingDlg(){};

public:
    typedef struct SettingDlgData
    {
        uint8_t col{0};
        uint8_t row{0};
        GameType type;
        QString puzzle_image;
        QString bk_music;
        bool bk_music_check{false};
        QString color_active;
        QString color_noactive;
        bool operator==(const SettingDlgData& data) const
        {
            return (col == data.col && row == data.row && type == data.type &&
                    puzzle_image == data.puzzle_image && bk_music == data.bk_music &&
                    bk_music_check == data.bk_music_check &&
                    color_active == data.color_active &&
                    color_noactive == data.color_noactive);
        }
        bool operator!=(const SettingDlgData& data) const
        {
            return (col != data.col || row != data.row || type != data.type ||
                    puzzle_image != data.puzzle_image || bk_music != data.bk_music ||
                    bk_music_check != data.bk_music_check ||
                    color_active != data.color_active ||
                    color_noactive != data.color_noactive);
        }
    } SettingDlgData;

public:
    void init_dlg_value(const SettingDlgData& data);
    void get_data(SettingDlgData& data) const;

private slots:
    void on_okButton_clicked();
    void on_btn_Music_clicked();
    void on_btn_Image_clicked();
    void on_btn_color_active_clicked();
    void on_btn_color_noactive_clicked();

private:
    int get_edit_col() const { return ui.lineEdit_col->text().toInt(); }
    int get_edit_row() const { return ui.lineEdit_row->text().toInt(); }
    int get_type() const
    {
        return ui.comboBox->itemData(ui.comboBox->currentIndex()).toInt();
    }
    QString get_image_file() const { return ui.lineEdit_image->text(); }
    QString get_music_file() const { return ui.lineEdit_music->text(); }
    bool get_musch_checked() const { return ui.checkBox->isChecked(); }
    void set_image_file(const QString& file)
    {
        ui.lineEdit_image->setText(QDir::toNativeSeparators(file));
    }
    void set_music_file(const QString& file)
    {
        ui.lineEdit_music->setText(QDir::toNativeSeparators(file));
    }

    QString get_color_active() const { return ui.lineEdit_color_active->text(); }
    QString get_color_noactive() const
    {
        return ui.lineEdit_color_noactive->text();
    }
    void set_color_active(const QString& color)
    {
        ui.lineEdit_color_active->setText(color);
    }
    void set_color_noactive(const QString& color)
    {
        ui.lineEdit_color_noactive->setText(color);
    }

    void update_data();
    bool is_changed();
    void set_combox_sel(GameType type);

private:
    Ui::setting_dlg ui;
    SettingDlgData m_data;
    static std::map<GameType, QString> m_game_types;
};
