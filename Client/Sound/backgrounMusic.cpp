#include "backgrounMusic.h"
#include <QFileInfo>

backgrounMusic::backgrounMusic()
{

}

backgrounMusic::backgrounMusic(const QString &relativePath)
{
    this->list = new QMediaPlaylist();
    list->addMedia(QUrl::fromUserInput(QFileInfo(relativePath).absoluteFilePath()));
    list->setPlaybackMode(QMediaPlaylist::Loop);
    this->player = new QMediaPlayer();
    player->setPlaylist(list);
    player->setVolume(10);
    player->play();
}

void backgrounMusic::stop()
{
    player->stop();
}

backgrounMusic::~backgrounMusic()
{
    player->stop();
    delete player;
    delete list;
}
