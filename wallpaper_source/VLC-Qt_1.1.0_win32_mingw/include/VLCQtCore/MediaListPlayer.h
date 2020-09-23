/****************************************************************************
* VLC-Qt - Qt and libvlc connector library
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef VLCQT_MEDIALISTPLAYER_H_
#define VLCQT_MEDIALISTPLAYER_H_

#include <QtCore/QObject>
#include <QtCore/QString>

#include "Enums.h"
#include "SharedExportCore.h"

class VlcInstance;
class VlcMediaList;
class VlcMediaPlayer;

struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;
struct libvlc_media_list_player_t;

/*!
    \class VlcMediaListPlayer MediaListPlayer.h VLCQtCore/MediaListPlayer.h
    \ingroup VLCQtCore
    \brief Media List Player

    A basic MediaListPlayer manager for VLC-Qt library.
    It provides internal playlist support.
    Requires a valid VlcMediaPlayer.
*/
class VLCQT_CORE_EXPORT VlcMediaListPlayer : public QObject
{
    Q_OBJECT
public:
    /*!
        \brief VlcMediaListPlayer constructor.

        Create VlcMediaListPlayer without new core.

        \param instance instance object
    */
    explicit VlcMediaListPlayer(VlcInstance *instance);

    /*!
        \brief VlcMediaListPlayer constructor.

        Create VlcMediaListPlayer with specific VlcMediaPlayer core.

        \param player player object
        \param instance instance object
    */
    explicit VlcMediaListPlayer(VlcMediaPlayer *player,
                                VlcInstance *instance);

    /*!
        VlcMediaListPlayer destructor
    */
    ~VlcMediaListPlayer();

    /*!
        \brief Returns libvlc media list player object.
        \return libvlc media list player (libvlc_media_list_player_t *)
    */
    libvlc_media_list_player_t *core();

    /*!
        \brief Get current media list
        \return current media list (VlcMediaList *)
    */
    VlcMediaList *currentMediaList();

    /*!
        \brief Get media player core
        \return media player core (VlcMediaPlayer *)
    */
    VlcMediaPlayer *mediaPlayer();

    /*!
        \brief Get playback mode
        \return playback mode
    */
    Vlc::PlaybackMode playbackMode() const;

    /*!
        \brief Set media list to play
        \param list media list object (VlcMediaList *)
    */
    void setMediaList(VlcMediaList *list);

    /*!
        \brief Set playback mode
        \param mode playback mode
    */
    void setPlaybackMode(const Vlc::PlaybackMode &mode);

public slots:
    /*!
        \brief Starts playing item at specific position.
        \param index item position
    */
    void itemAt(int index);

    /*!
        \brief Starts playing next item in the list.
    */
    void next();

    /*!
        \brief Starts playing media list.
    */
    void play();

    /*!
        \brief Starts playing previous item in the list.
    */
    void previous();

    /*!
        \brief Stops playing
    */
    void stop();

signals:
    /*!
        \brief Signal sent when played
    */
    void played();

    /*!
        \brief Signal sent on next item set
        \param media next media item
        \since VLC-Qt 1.1
    */
    void nextItemSet(VlcMedia *media);

    /*!
        \brief Signal sent on next item set
        \param media next media item
    */
    void nextItemSet(libvlc_media_t *media);

    /*!
        \brief Signal sent when stopped
    */
    void stopped();

private:
    static void libvlc_callback(const libvlc_event_t *event,
                                void *data);

    void createCoreConnections();
    void removeCoreConnections();

    libvlc_media_list_player_t *_vlcMediaListPlayer;
    libvlc_event_manager_t *_vlcEvents;

    VlcMediaList *_list;
    VlcMediaPlayer *_player;

    Vlc::PlaybackMode _mode;
};

#endif // VLCQT_MEDIALISTPLAYER_H_
