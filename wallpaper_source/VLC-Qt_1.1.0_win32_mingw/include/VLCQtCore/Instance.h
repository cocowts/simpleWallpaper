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

#ifndef VLCQT_VLCINSTANCE_H_
#define VLCQT_VLCINSTANCE_H_

#include <QtCore/QObject>

#include "Enums.h"
#include "SharedExportCore.h"

class VlcModuleDescription;

struct libvlc_instance_t;

/*!
    \class VlcInstance Instance.h VLCQtCore/Instance.h
    \ingroup VLCQtCore
    \brief Main instance

    A basic Instance manager for VLC-Qt library.
    It provides main instance controls.
*/
class VLCQT_CORE_EXPORT VlcInstance : public QObject
{
    Q_OBJECT
public:
    /*!
        \brief VlcInstance constructor.

        This is mandatory for using VLC-Qt and all its other classes.

        \param args libvlc arguments (QStringList)
        \param parent Instance's parent object (QObject *)
    */
    explicit VlcInstance(const QStringList &args,
                         QObject *parent = NULL);

    /*!
        VlcInstance destructor
    */
    ~VlcInstance();

    /*!
        \brief Returns libvlc instance object.
        \return libvlc instance (libvlc_instance_t *)
    */
    libvlc_instance_t *core();

    /*!
        \brief Returns libvlc initialisation status.
        \return libvlc status (bool)
    */
    bool status() const;

    /*!
        \brief Returns current log level (default Vlc::ErrorLevel)
        \return log level
        \since VLC-Qt 1.1
    */
    Vlc::LogLevel logLevel() const;

    /*!
        \brief Set current log level
        \param level desired log level
        \see Vlc::LogLevel
        \since VLC-Qt 1.1
    */
    void setLogLevel(Vlc::LogLevel level);

    /*!
        \brief VLC-Qt version info
        \return a string containing the VLC-Qt version (QString)
    */
    static QString libVersion();

    /*!
        \brief VLC-Qt major version
        \return VLC-Qt major version (int)
    */
    static int libVersionMajor();

    /*!
        \brief VLC-Qt minor version
        \return VLC-Qt minor version (int)
    */
    static int libVersionMinor();

    /*!
        \brief libvlc version info
        \return a string containing the libvlc version (QString)
    */
    static QString version();

    /*!
        \brief libvlc compiler info
        \return a string containing the compiler version (QString)
    */
    static QString compiler();

    /*!
        \brief libvlc changeset info
        \return a string containing the changeset (QString)
    */
    static QString changeset();

    /*!
        \brief Sets the application name.

        libvlc passes this as the user agent string when a protocol requires it.

        \param application Application name (QString)
        \param version Application version (QString)

        \see setAppId
    */
    void setUserAgent(const QString &application,
                      const QString &version);

    /*!
        \brief Sets the application some meta-information.

        \param id Java-style application identifier, e.g. "com.acme.foobar"
        \param version application version numbers, e.g. "1.2.3"
        \param icon application icon name, e.g. "foobar"

        \see setUserAgent
        \since VLC-Qt 1.1
    */
    void setAppId(const QString &id,
                  const QString &version,
                  const QString &icon);

    /*!
        \brief List audio filter modules
        \return audio filter module description list
    */
    QList<VlcModuleDescription *> audioFilterList() const;

    /*!
        \brief List video filter modules
        \return video filter module description list
    */
    QList<VlcModuleDescription *> videoFilterList() const;

private:
    libvlc_instance_t *_vlcInstance;
    bool _status;
    Vlc::LogLevel _logLevel;
};

#endif // VLCQT_VLCINSTANCE_H_
