#include "musicsongchecktoolscore.h"
#include "musicsongtag.h"

#include <QDebug>

MusicSongCheckToolsRenameCore::MusicSongCheckToolsRenameCore(QObject *parent)
    : QThread(parent)
{
    m_run = false;
    m_songItems = nullptr;
}

MusicSongCheckToolsRenameCore::~MusicSongCheckToolsRenameCore()
{
    stopAndQuitThread();
}

QString MusicSongCheckToolsRenameCore::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsRenameCore::setRenameSongs(MusicSongItems *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsRenameCore::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicSongCheckToolsRenameCore::start()
{
    m_run = true;
    QThread::start();
}

void MusicSongCheckToolsRenameCore::run()
{
    if(m_songItems && m_songItems->count() >= 4)
    {
        MusicSongs *musicSongs = &m_songItems->first().m_songs;
        SongCheckToolsRenames items;
        MusicSongTag tag;
        foreach(const MusicSong &song, *musicSongs)
        {
            if(!m_run)
            {
                emit finished(SongCheckToolsRenames());
                return;
            }

            if(!tag.readFile(song.getMusicPath()))
            {
                continue;
            }

            if(tag.getArtist() != song.getMusicArtistFront() ||
               tag.getTitle() != song.getMusicArtistBack())
            {
                items << SongCheckToolsRename(song.getMusicName(), tag.getArtist() + " - " + tag.getTitle());
            }
        }
        emit finished(items);
    }
}



MusicSongCheckToolsDuplicateCore::MusicSongCheckToolsDuplicateCore(QObject *parent)
    : QThread(parent)
{
    m_run = false;
    m_songItems = nullptr;
}

MusicSongCheckToolsDuplicateCore::~MusicSongCheckToolsDuplicateCore()
{
    stopAndQuitThread();
}

QString MusicSongCheckToolsDuplicateCore::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsDuplicateCore::setRenameSongs(MusicSongItems *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsDuplicateCore::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicSongCheckToolsDuplicateCore::start()
{
    m_run = true;
    QThread::start();
}

void MusicSongCheckToolsDuplicateCore::run()
{
    if(m_songItems)
    {
        qDebug() << "MusicSongCheckToolsDuplicateCore" << m_songItems->count();
    }
}



MusicSongCheckToolsQualityCore::MusicSongCheckToolsQualityCore(QObject *parent)
    : QThread(parent)
{
    m_run = false;
    m_songItems = nullptr;
}

MusicSongCheckToolsQualityCore::~MusicSongCheckToolsQualityCore()
{
    stopAndQuitThread();
}

QString MusicSongCheckToolsQualityCore::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsQualityCore::setRenameSongs(MusicSongItems *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsQualityCore::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicSongCheckToolsQualityCore::start()
{
    m_run = true;
    QThread::start();
}

void MusicSongCheckToolsQualityCore::run()
{
    if(m_songItems)
    {
        qDebug() << "MusicSongCheckToolsQualityCore" << m_songItems->count();
    }
}
