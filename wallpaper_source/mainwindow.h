#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QCheckBox>
#include <QColor>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QToolButton>
#include <QWidget>

#include <VLCQtWidgets/WidgetVideo.h>
#include <VLCQtCore/Instance.h>

#include "characterlabel.h"
#include "taskbarcontrol.h"

class MainWindow : public QWidget
{
    Q_OBJECT

    QLatin1String AuthorUrl = QLatin1String(R"(https://segmentfault.com/u/tiansong)");

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *object, QEvent *event) override;

protected slots:
    void onSelectResourcesBtnClicked();
    void onSysTrayAboutActionTrigger();
    void onSysTrayHelpActionTrigger();

    void onCharacteLblMove();
    void onCharacteLblCheckShow(bool sta);
    void SetCharacteLbOpacity(int val);

private:
    void initUi();
    void initSystemTray();
    void initConctol();
    bool loadResourcesFile();
    HWND findDeskTopWindow();
    void removeAllWallpaper();
    void createImageWallpaper(const QStringList &files);
    void createMovieWallpaper(const QString &file);
    void createVideoWallpaper(const QString &file);
    void createDefaultWallpaper(const QString &filePath);
    void saveState();
    void restoreState();

private:
    QRadioButton *m_pResourcesFilesRadioBtn = nullptr;
    QRadioButton *m_pResourcesFileRadioBtn  = nullptr;
    QPushButton *m_pSelectResourcesBtn      = nullptr;
    QSpinBox *m_pTimeIntervalSpinBox        = nullptr;
    QSlider *m_pVolumeSlider                = nullptr;
    CharacterLabel *m_pCharacterLbl         = nullptr;
    QCheckBox *m_pCharacterVisibleBox       = nullptr;
    QPushButton *m_pCharacteFontBtn         = nullptr;
    QPushButton *m_pCharacteColorBtn        = nullptr;
    QLineEdit *m_pCharacteEdit              = nullptr;
    QSpinBox *m_pCharacteXBox               = nullptr;
    QSpinBox *m_pCharacteYBox               = nullptr;
    QSlider *m_pCharacteSlider              = nullptr;
    QCheckBox *m_pTaskBarAutoHideBox        = nullptr;
    QRadioButton *m_pTaskBarTransparentgradientBtn = nullptr;
    QRadioButton *m_pTaskBarBlurbehindBtn          = nullptr;
    QPushButton *m_pTaskBarColorBtn = nullptr;
    QSlider     *m_pTaskBarAlphaSlider    = nullptr;
    QCheckBox *m_pAutoRuningCheckBox        = nullptr;
    QSystemTrayIcon *m_pTrayIcon            = nullptr;
    QAction *m_pSysTraySetAction            = nullptr;
    QAction *m_pSysTrayAboutAction          = nullptr;
    QAction *m_pSysTrayHelpAction           = nullptr;
    QAction *m_pSysTrayExitAction           = nullptr;

    QLabel *m_pMovieLbl         = nullptr;
    QLabel *m_pImageLbl         = nullptr;
    VlcWidgetVideo *m_pVedioLbl = nullptr;

    QStringList m_filesPath;
    QList<QPixmap> m_images;
    int m_imageIndex = 0;
    VlcInstance *m_pInstance = nullptr;
    VlcMediaPlayer*m_pPlayer = nullptr;
    TaskbarControl *m_pTaskbarControl = new TaskbarControl(this);
};
#endif // MAINWINDOW_H
