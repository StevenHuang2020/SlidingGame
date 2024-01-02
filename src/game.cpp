#include "game.h"

QString Game::m_res_path = "./res/";
QString Game::m_tmp_path = "tmp/";

Game::Game(QWidget* parent)
    : QMainWindow(parent), m_type(GameType::E_Number),
      m_bkmusic_player(std::make_unique<QMediaPlayer>()),
      m_audioOutputDev(std::make_unique<QAudioOutput>()),
      m_effect(std::make_unique<QSoundEffect>()),
      m_image(default_game_image()), m_music(default_game_music())
{
    ui.setupUi(this);
    centralWidget()->setLayout(ui.gridLayout);

    read_settings();

    m_game = std::make_unique<SlidingGame>(m_row, m_col);

    init_game();

    on_actionNew_triggered();

    play_background(m_music_on);
}

Game::~Game()
{
    save_settings();
    final_game();
}

QString Game::get_abs_path(const QString& file) const
{
    QDir dir(file);
    if (!dir.exists())
        return dir.absolutePath();

    return QString("");
}

void Game::on_actionNew_triggered()
{
    QElapsedTimer timer;
    timer.start();

    m_game->new_game();
    qDebug() << "----------------%d milliseconds" << timer.elapsed();

    print_data("new game:");

    update_labels();
}

void Game::mousePressEvent(QMouseEvent* event)
{
    QWidget* const widget = childAt(event->pos());
    qDebug() << "child widget: " << widget;
    if (widget)
    {
        const QLabel* const label = qobject_cast<QLabel*>(widget);
        if (label)
        {
            qDebug() << "label: " << label->text();
        }

        const int index = centralWidget()->layout()->indexOf(widget);
        qDebug() << "layout index: " << index;
        if (index >= 0)
        {
            const QLayoutItem* const item = centralWidget()->layout()->itemAt(index);
            qDebug() << "layout item: " << item;

            block_clicked(index);
        }
    }
}

void Game::init_game()
{
    final_game();

    if (m_type == GameType::E_Image)
        split_images(); // GameType::E_Image

    for (int i = 0; i < m_game->total(); i++)
    {
        QLabel* pLabel = new QLabel(this);

        int id = i;
        pLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

        QString common_style = "border-style: outset;	\
					border-width: 2px;	\
					border-radius: 10px;	\
					border-color: beige;	\
					padding: 6px; ";

        if (id == 0)
        {
            QString style = "background-color: ";
            style += m_color_noactive;
            style += ";";
            pLabel->setStyleSheet(style + common_style);
        }
        else
        {
            if (m_type == GameType::E_Number)
            {
                pLabel->setText(QString::number(id));

                QString style = "background-color: ";
                style += m_color_active;
                style += ";";
                style += "font: 28pt;";
                style += "font-style: italic; font-style: bold; font-family: Consolas;";

                pLabel->setStyleSheet(style + common_style);
            }
            else
            {
                QString file = QString("%1.png").arg(id);
                QImage img(m_res_path + m_tmp_path + file);
                pLabel->setPixmap(QPixmap::fromImage(img));
            }
        }

        pLabel->setAlignment(Qt::AlignCenter);
        m_labels.push_back(pLabel);
        m_map_lables[id] = pLabel;
    }
}

void Game::clear_tempfiles(const QString& path)
{
    QDir dir(path);
    for (QString& dirFile : dir.entryList())
        dir.remove(dirFile);
}

void Game::split_images()
{
    if (!QFile::exists(m_image))
        m_image = default_game_image();

    QImage img(m_image);
    if (img.width() > 1024)
        img = img.scaledToWidth(1024);
    if (img.height() > 800)
        img = img.scaledToHeight(800);

    int row = m_game->row();
    int col = m_game->col();
    QRect rtImg = img.rect();
    int width = img.width();
    int height = img.height();
    int subW = width / col;
    int subH = height / row;

    QString filePath = m_res_path + m_tmp_path;
    QDir dir(filePath);
    if (!dir.exists())
    {
        dir.mkdir(".");
    }
    else
    {
        clear_tempfiles(filePath);
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            QRect rt(j * subW, i * subH, subW, subH);

            QImage sub = createSubImage(img, rt);

            QString file = filePath + QString("%1.png").arg(i * row + j + 1);
            sub.save(file);
        }
    }
}

QImage Game::createSubImage(const QImage& image, const QRect& rect)
{
    size_t offset = rect.x() * image.depth() / 8 + rect.y() * image.bytesPerLine();
    return QImage(image.bits() + offset, rect.width(), rect.height(), image.bytesPerLine(), image.format());
}

void Game::final_game()
{
    for (auto& i : m_labels)
        delete i;

    m_labels.clear();
    m_map_lables.clear();
}

void Game::print_data(const QString& pre) const
{
    qDebug() << pre;
    const std::vector<int>& data = m_game->get_data();
    for (const int& i : data)
    {
        /*QString str = QString("%1").arg(i);
            qDebug("%s", qUtf8Printable(str));*/
        qDebug().nospace() << i;
    }
}

void Game::remove_labels()
{
    auto pGrid = ui.gridLayout;
    for (auto& pLabel : m_labels)
    {
        pGrid->removeWidget(pLabel);
    }
    pGrid->update();
}

void Game::update_labels()
{
    remove_labels();

    auto pGrid = ui.gridLayout;

    const std::vector<int>& data = m_game->get_data();
    for (int i = 0; i < data.size(); i++)
    {
        int row = i / m_game->col();
        int col = i % m_game->col();
        pGrid->addWidget(m_map_lables[data[i]], row, col);
    }
}

void Game::block_clicked(const int& index)
{
    const int dst_index = m_game->clicked(index);

    print_data("clicked:");

    if (dst_index == -1)
    {
        show_msg_Status("Can't moved!");
        return;
    }

    update_labels();

    QString msg =
        QString("Tile %1 has been moved to %2.").arg(index).arg(dst_index);
    show_msg_Status(msg);

    if (m_game->check_over())
    {
        play_win();

        bool ret = show_yesno_dlg("Congratulations!", "Paly again?");
        if (ret)
        {
            on_actionNew_triggered();
            return;
        }
    }

    play_bell();
}

bool Game::show_yesno_dlg(const QString& title, const QString& text)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, title, text,
                                  QMessageBox::Yes | QMessageBox::No);

    return (reply == QMessageBox::Yes);
}

void Game::on_actionSetting_triggered()
{
    SettingDlg dialog(this);

    SettingDlg::SettingDlgData data;
    data.col = m_game->col();
    data.row = m_game->row();
    data.type = m_type;
    data.bk_music = m_music;
    data.puzzle_image = m_image;
    data.bk_music_check = m_music_on;
    data.color_active = m_color_active;
    data.color_noactive = m_color_noactive;

    dialog.init_dlg_value(data);

    int result = dialog.exec();
    if (QDialog::Accepted == result)
    {

        SettingDlg::SettingDlgData new_data;
        dialog.get_data(new_data);

        m_col = new_data.col;
        m_row = new_data.row;
        m_type = new_data.type;
        m_image = new_data.puzzle_image;
        m_music_on = new_data.bk_music_check;
        m_music = new_data.bk_music;
        m_color_active = new_data.color_active;
        m_color_noactive = new_data.color_noactive;

        play_background(m_music_on);

        if (m_game->reset_game(m_col, m_row))
        {
            init_game();
            on_actionNew_triggered();
            save_settings();
        }
        else
        {
            show_msg_dlg("Parameters error, invalid Settings!");
        }
    }
}

void Game::play_background(bool play)
{
    QFile file(m_music);
    if (!file.exists())
        return;

    if (play)
    {
        m_bkmusic_player->setAudioOutput(m_audioOutputDev.get());
        m_bkmusic_player->setSource(QUrl::fromLocalFile(m_music));
        m_bkmusic_player->play();
    }
    else
    {
        m_bkmusic_player->stop();
        return;
    }
}

void Game::read_settings()
{
    int value;
    QVariant values = m_settings.get_general("col");
    if (values.isValid())
        m_col = values.toInt();

    values = m_settings.get_general("row");
    if (values.isValid())
        m_row = values.toInt();

    values = m_settings.get_general("type");
    if (values.isValid())
        m_type = (GameType)values.toInt();

    values = m_settings.get_general("music");
    if (values.isValid())
        m_music = values.toString();

    values = m_settings.get_general("music_on");
    if (values.isValid())
        m_music_on = (bool)values.toInt();

    values = m_settings.get_general("puzzle-image");
    if (values.isValid())
        m_image = values.toString();

    values = m_settings.get_general("color-active");
    if (values.isValid())
        m_color_active = values.toString();

    values = m_settings.get_general("color-noactive");
    if (values.isValid())
        m_color_noactive = values.toString();
}

void Game::save_settings()
{
    m_settings.set_general("col", m_col);
    m_settings.set_general("row", m_row);
    m_settings.set_general("type", (int)m_type);
    m_settings.set_general("music", m_music);
    m_settings.set_general("music_on", (int)m_music_on);
    m_settings.set_general("puzzle-image", m_image);
    m_settings.set_general("color-active", m_color_active);
    m_settings.set_general("color-noactive", m_color_noactive);
    m_settings.set_info("game", "software");
    m_settings.set_info("Steven-Huang", "author");
}

void Game::show_msg_dlg(const QString& message, const QString& windowTitle)
{
    QMessageBox msgBox;

    msgBox.setText(message);
    msgBox.setWindowTitle(windowTitle);

    msgBox.setModal(true);
    msgBox.show();
    msgBox.move(frameGeometry().center() - msgBox.rect().center());
    msgBox.setWindowFlags(msgBox.windowFlags() | Qt::Popup);
    msgBox.exec();
}

void Game::show_msg_Status(const QString& message)
{
    // statusBar()->showMessage(message);
}
