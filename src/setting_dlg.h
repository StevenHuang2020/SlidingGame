#pragma once
#include<QDialog>
#include<QFileDialog>
#include "ui_setting_dlg.h"

enum class GameType { E_Number, E_Image };

class SettingDlg : public QDialog
{
	Q_OBJECT

public:
	SettingDlg(QWidget* parent = Q_NULLPTR);
	~SettingDlg() {};
private:
	Ui::setting_dlg ui;
public:
	typedef struct SettingDlgData {
		int col;
		int row;
		GameType type;
		QString puzzle_image;
		QString bk_music;
		bool operator==(const SettingDlgData& data) const
		{
			return (col == data.col && row == data.row
				&& type == data.type && puzzle_image == data.puzzle_image
				&& bk_music == data.bk_music);
		}
		bool operator!=(const SettingDlgData& data) const
		{
			return (col != data.col || row != data.row
				|| type != data.type || puzzle_image != data.puzzle_image
				|| bk_music != data.bk_music);
		}
	}SettingDlgData;
private:
	SettingDlgData m_data;
	static std::map<GameType, QString> m_game_types;
private slots:
	void on_okButton_clicked();
	void on_btn_Music_clicked();
	void on_btn_Image_clicked();
private:
	int get_edit_col() const { return ui.lineEdit_col->text().toInt(); }
	int get_edit_row() const { return ui.lineEdit_row->text().toInt(); }
	int get_type() const { return ui.comboBox->itemData(ui.comboBox->currentIndex()).toInt(); }
	QString get_image_file() const { return ui.lineEdit_image->text(); }
	QString get_music_file() const { return ui.lineEdit_music->text(); }
	void set_image_file(const QString& file) { ui.lineEdit_image->setText(file); }
	void set_music_file(const QString& file) { ui.lineEdit_music->setText(file); }

	void update_data();
	bool is_changed();
	void set_combox_sel(GameType type);
public:
	void init_dlg_value(const SettingDlgData& data);
	void get_data(SettingDlgData& data) const;
};
