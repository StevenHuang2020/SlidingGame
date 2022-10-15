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
#include "app_settings.h"

class Game : public QMainWindow
{
	Q_OBJECT

public:
	Game(QWidget* parent = nullptr);
	~Game();

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
	std::unique_ptr<QMediaPlayer> m_bkmusic_player;
	std::unique_ptr<QMediaPlaylist> m_bkmusic_playlist;

	AppSettings m_settings;
private:
	void mousePressEvent(QMouseEvent* event) override;
private slots:
	void on_actionNew_triggered();
	void on_actionQuit_triggered() { QMainWindow::close(); }
	void on_actionAbout_triggered() { QApplication::aboutQt(); }
	void on_actionSetting_triggered();
private:
	void init_game();
	void final_game();
	void print_data(const QString& pre = "") const;
	bool show_msg_dlg(const QString& title, const QString& text);
	void clear_tempfiles(const QString& path);
	void split_images();
	QImage createSubImage(const QImage& image, const QRect& rect);
	QString get_abs_path(const QString& file) const;
	void save_settings();
	void read_settings();
public:
	void update_labels();
	void remove_labels();
	void block_clicked(int index);
	void play_bell() const { QSound::play(m_res_path + "notify.wav"); }
	void play_win()const { QSound::play(m_res_path + "win.wav"); }
	void play_background();
};
