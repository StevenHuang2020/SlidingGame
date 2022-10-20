#include "setting_dlg.h"

std::map<GameType, QString> SettingDlg::m_game_types = {
	{ GameType::E_Number, "Number" },
	{ GameType::E_Image, "Image" }
};

SettingDlg::SettingDlg(QWidget* parent)
	:QDialog(parent),
	m_data({})
{
	ui.setupUi(this);
	setLayout(ui.gridLayout);

	/*setWindowFlags(windowFlags() &
		~Qt::WindowMaximizeButtonHint &
		~Qt::WindowContextHelpButtonHint);*/

	ui.lineEdit_col->setValidator(new QIntValidator);
	ui.lineEdit_row->setValidator(new QIntValidator);

	//QObject::connect(ui.okButton, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void SettingDlg::init_dlg_value(const SettingDlgData& data)
{
	m_data = data;
	update_data();
}

void SettingDlg::get_data(SettingDlgData& data) const
{
	data.col = get_edit_col();
	data.row = get_edit_row();
	data.type = (GameType)get_type();
	data.bk_music = get_music_file();
	data.puzzle_image = get_image_file();
}

void SettingDlg::update_data()
{
	ui.lineEdit_col->setText(QString::number(m_data.col));
	ui.lineEdit_row->setText(QString::number(m_data.row));

	GameType type = GameType::E_Number;
	ui.comboBox->addItem(m_game_types[type], (int)type);

	type = GameType::E_Image;
	ui.comboBox->addItem(m_game_types[type], (int)type);

	set_combox_sel(m_data.type);

	ui.lineEdit_music->setText(m_data.bk_music);
	ui.lineEdit_image->setText(m_data.puzzle_image);
}

void SettingDlg::set_combox_sel(GameType type)
{
	int index = ui.comboBox->findData((int)type);
	if (index != -1) {
		ui.comboBox->setCurrentIndex(index);
	}
}

bool SettingDlg::is_changed()
{
	SettingDlgData data;
	get_data(data);
	if (data != m_data)
		return true;
	return false;
}

void SettingDlg::on_okButton_clicked()
{
	if (is_changed()) {
		accept();
		return;
	}

	reject();
}

void SettingDlg::on_btn_Music_clicked()
{
	const QStringList filters({ "Audios (*.mp3 *.wav *.wma)" });

	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilters(filters);
	dialog.setViewMode(QFileDialog::List);

	if (dialog.exec()) {
		QStringList fileNames = dialog.selectedFiles();
		// qDebug("file:%s\n", qUtf8Printable(fileNames[0]));
		set_music_file(fileNames[0]);
	}
}

void SettingDlg::on_btn_Image_clicked()
{
	const QStringList filters({ "Images (*.png *.jpg *.bmp)" });

	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilters(filters);
	dialog.setViewMode(QFileDialog::List);

	if (dialog.exec()) {
		QStringList fileNames = dialog.selectedFiles();
		// qDebug("file:%s\n", qUtf8Printable(fileNames[0]));
		set_image_file(fileNames[0]);
	}
}