#pragma once

#include <QDebug>
#include <QDir>
#include <QElapsedTimer>
#include <QLabel>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QSoundEffect>
#include <QtWidgets/QMainWindow>
#include <QAudioOutput>
#include <map>
#include <memory>

#include "ui_game.h"
#include "app_settings.h"
#include "setting_dlg.h"
#include "sliding_game.h"
#include "hint_dlg.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class gameClass;
}
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
    void play_bell() const
    {
        play_effect(get_abs_path(m_res_path + "notify.wav"));
    }
    void play_win() const
    {
        play_effect(get_abs_path(m_res_path + "win.wav"));
    }
    void play_effect(const QString& audio) const
    {
        m_effect->setSource(QUrl::fromLocalFile(audio));
        //effect.setLoopCount(QSoundEffect::Infinite);
        m_effect->setVolume(0.5f);
        m_effect->play();
    }

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
    void get_game_data(SettingDlg::SettingDlgData &data);
    void update_hintWnd();
    QString default_game_image()
    {
        return get_abs_path(m_res_path + "puzzle.png");
    }
    QString default_game_music()
    {
        return get_abs_path(m_res_path + "music.mp3");
    }
    void closeEvent(QCloseEvent* event);

private:
    void mousePressEvent(QMouseEvent* event) override;
private slots:
    void on_actionNew_triggered();
    void on_actionQuit_triggered() { QMainWindow::close(); }
    void on_actionAbout_triggered() { QApplication::aboutQt(); }
    void on_actionSetting_triggered();
    void on_actionHint_triggered();

private:
    Ui::gameClass ui;
    std::unique_ptr<SlidingGame> m_game;

    std::map<int, QLabel*> m_map_lables;
    std::vector<QLabel*> m_labels;
    static QString m_res_path;
    static QString m_tmp_path;
    uint8_t m_col{3};
    uint8_t m_row{3};
    GameType m_type;
    QString m_image; // puzzle image path
    QString m_music; // background music file
    bool m_music_on{true};
    QString m_color_active{"#FF91AF"};
    QString m_color_noactive{"gray"};

    std::unique_ptr<QMediaPlayer> m_bkmusic_player;
    std::unique_ptr<QAudioOutput> m_audioOutputDev;
    std::unique_ptr<QSoundEffect> m_effect;
    std::unique_ptr<HintWnd> m_hintWnd;
    AppSettings m_settings;
};
