#pragma once

#include<QtWidgets/QMainWindow>
#include<QDebug>
#include<QPushButton>
#include<QLabel>
#include<QMouseEvent>
#include<QMessageBox>
#include<QElapsedTimer>
#include<QSound>
#include<QDir>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include<memory>
#include<map>

#include "ui_game.h"
#include "sliding_game.h"
#include "setting_dlg.h"
#include "camera_dlg.h"
#include "app_settings.h"


QT_BEGIN_NAMESPACE
namespace Ui { class gameClass; }
QT_END_NAMESPACE

class Game : public QMainWindow
{
	Q_OBJECT

public:
	Game(QWidget* parent = nullptr);
	~Game();

public:
	void update_labels();
	void remove_labels();
	void block_clicked(const int& index);
	void play_bell() const { QSound::play(m_res_path + "notify.wav"); }
	void play_win()const { QSound::play(m_res_path + "win.wav"); }
	void play_background(bool play = true);

private:
	void init_game();
	void final_game();
	void print_data(const QString& pre = "") const;
	void clear_tempfiles(const QString& path);
	void split_images();
	QImage createSubImage(const QImage& image, const QRect& rect);
	QString get_abs_path(const QString& file) const;
	void save_settings();
	void read_settings();
	bool show_yesno_dlg(const QString& title, const QString& text);
	void show_msg_dlg(const QString& message = "", const QString& windowTitle = "Warning");
	void show_msg_Status(const QString& message);
private:
	void mousePressEvent(QMouseEvent* event) override;
private slots:
	void on_actionNew_triggered();
	void on_actionQuit_triggered() { QMainWindow::close(); }
	void on_actionAbout_triggered() { QApplication::aboutQt(); }
	void on_actionSetting_triggered();
	void on_actionCamera_triggered();

private:
	Ui::gameClass ui;
	std::unique_ptr<SlidingGame> m_game;

	std::map<int, QLabel*> m_map_lables;
	std::vector<QLabel*> m_labels;
	static QString m_res_path;
	static QString m_tmp_path;
	int m_col;
	int m_row;
	GameType m_type;
	QString m_image;  // puzzle image path
	QString m_music;  // background music file
	bool m_music_on;
	QString m_color_active;
	QString m_color_noactive;

	std::unique_ptr<QMediaPlayer> m_bkmusic_player;
	std::unique_ptr<QMediaPlaylist> m_bkmusic_playlist;

	AppSettings m_settings;
};
