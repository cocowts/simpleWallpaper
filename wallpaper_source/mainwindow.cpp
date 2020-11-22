#include "mainwindow.h"

#include <QApplication>
#include <QColorDialog>
#include <QDesktopServices>
#include <QEvent>
#include <QFileDialog>
#include <QFont>
#include <QFontDatabase>
#include <QFontDialog>
#include <QFontMetrics>
#include <QGridLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QMovie>
#include <QPlainTextEdit>
#include <QSettings>
#include <QStringLiteral>
#include <qt_windows.h>
#include <QUrl>
#include <QVBoxLayout>

#include <VLCQtCore/Common.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#include <VLCQtCore/Audio.h>

#include "characterlabel.h"

void Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent, Qt::WindowCloseButtonHint)
{
    initUi();
    initSystemTray();
    initConctol();
    createDefaultWallpaper(qApp->applicationDirPath() + "/default.png");
    restoreState();
    loadResourcesFile();
    m_pTrayIcon->showMessage(QString("简单桌面"), QStringLiteral("👧 我开始接管你的桌面啦"));
}

MainWindow::~MainWindow()
{
    m_pTaskbarControl->setAccentState(TaskbarControl::ACCENT_ENABLE_GRADIENT);
    m_pTaskbarControl->setColor(QColor(255, 255, 255));
    m_pTaskbarControl->setAutoHide(false);
}

void MainWindow::initUi()
{
    // 资源设置
    QGroupBox *pResSettingBox      = new QGroupBox(QStringLiteral("资源设置"), this);
    QHBoxLayout *pResSettingLayout = new QHBoxLayout;
    m_pResourcesFileRadioBtn       = new QRadioButton(QStringLiteral("单静态图片&&GIF&&视频文件"));
    m_pResourcesFilesRadioBtn      = new QRadioButton(QStringLiteral("多静态图片"));
    m_pSelectResourcesBtn          = new QPushButton(QStringLiteral("选择背景文件"));

    m_pResourcesFilesRadioBtn->setChecked(true);
    pResSettingLayout->addWidget(m_pResourcesFilesRadioBtn);
    pResSettingLayout->addWidget(m_pResourcesFileRadioBtn);
    pResSettingLayout->addStretch();
    pResSettingLayout->addWidget(m_pSelectResourcesBtn);
    pResSettingLayout->setSpacing(15);
    pResSettingBox->setLayout(pResSettingLayout);

    // 特效设置
    QGroupBox *pEffectSettingBox      = new QGroupBox(QStringLiteral("特效设置"), this);
    QHBoxLayout *pEffectSettingLayout = new QHBoxLayout;
    m_pTimeIntervalSpinBox            = new QSpinBox;
    m_pVolumeSlider                   = new QSlider;

    m_pTimeIntervalSpinBox->setSuffix(QStringLiteral("秒"));
    m_pTimeIntervalSpinBox->setRange(1, 1000);
    m_pTimeIntervalSpinBox->setSingleStep(2);
    m_pTimeIntervalSpinBox->setValue(5);
    m_pVolumeSlider->setOrientation(Qt::Horizontal);
    m_pVolumeSlider->setStyleSheet("QSlider::groove{border: 1px solid #999999;background: #ffffff;}"
                               "QSlider::handle {border: 1px solid #999999;background: #88bbff;}"
                               "QSlider::groove:horizontal {height: 3px;left:5px; right: 5px;}"
                               "QSlider::handle:horizontal{width: 6px;margin: -7px;}"
                               "QSlider::add-page{border: 1px solid #999999;background:#eeeeef;}"
                               "QSlider::sub-page{background: #eeaa22;}");

    pEffectSettingLayout->addWidget(new QLabel(QStringLiteral("多图片切换间隔")));
    pEffectSettingLayout->addWidget(m_pTimeIntervalSpinBox);
    pEffectSettingLayout->addSpacing(20);
    pEffectSettingLayout->addWidget(new QLabel(QStringLiteral("视频音量大小")));
    pEffectSettingLayout->addWidget(m_pVolumeSlider);
    pEffectSettingBox->setLayout(pEffectSettingLayout);

    // 文字设置
    QGroupBox *pFontSettingBox          = new QGroupBox(QStringLiteral("文字设置 [设置背景文件后生效]"), this);
    QVBoxLayout *pFontSettingLayout     = new QVBoxLayout;
    QHBoxLayout *pFontSettingSubLayout1 = new QHBoxLayout;
    QHBoxLayout *pFontSettingSubLayout2 = new QHBoxLayout;

    m_pCharacterVisibleBox   = new QCheckBox;
    m_pCharacteFontBtn       = new QPushButton;
    m_pCharacteColorBtn      = new QPushButton;
    m_pCharacteEdit          = new QLineEdit;
    m_pCharacteXBox          = new QSpinBox;
    m_pCharacteYBox          = new QSpinBox;
    m_pCharacteSlider        = new QSlider;
    m_pCharacterLbl          = new CharacterLabel(this);

    m_pCharacterLbl->setVisible(m_pCharacterVisibleBox->isChecked());
    m_pCharacteXBox->setRange(0, 65535);
    m_pCharacteYBox->setRange(0, 65535);
    m_pCharacteXBox->setSingleStep(20);
    m_pCharacteYBox->setSingleStep(20);
    m_pCharacteSlider->setRange(10, 255);
    m_pCharacteSlider->setMinimumWidth(80);
    m_pCharacteSlider->setOrientation(Qt::Horizontal);
    m_pCharacterVisibleBox->setText(QStringLiteral("显示文字"));
    m_pCharacteFontBtn->setText(QStringLiteral("字体"));
    m_pCharacteFontBtn->setIcon(QIcon(":/image/image/font.png"));
    m_pCharacteColorBtn->setText(QStringLiteral("颜色"));
    m_pCharacteColorBtn->setIcon(QIcon(":/image/image/color.png"));
    m_pCharacteEdit->setPlaceholderText(QStringLiteral("请输出要显示的文字"));
    m_pCharacteSlider->setStyleSheet("QSlider::groove{border: 1px solid #999999;background: #ffffff;}"
                               "QSlider::handle {border: 1px solid #999999;background: #88bbff;}"
                               "QSlider::groove:horizontal {height: 3px;left:5px; right: 5px;}"
                               "QSlider::handle:horizontal{width: 6px;margin: -7px;}"
                               "QSlider::add-page{border: 1px solid #999999;background:#eeeeef;}"
                               "QSlider::sub-page{background: #eeaa22;}");

    pFontSettingSubLayout1->addWidget(m_pCharacterVisibleBox);
    pFontSettingSubLayout1->addWidget(m_pCharacteEdit);

    pFontSettingSubLayout2->addWidget(m_pCharacteFontBtn);
    pFontSettingSubLayout2->addWidget(m_pCharacteColorBtn);
    pFontSettingSubLayout2->addWidget(new QLabel(QStringLiteral("坐标")));
    pFontSettingSubLayout2->addWidget(m_pCharacteXBox);
    pFontSettingSubLayout2->addWidget(new QLabel(QStringLiteral("X")));
    pFontSettingSubLayout2->addWidget(m_pCharacteYBox);
    pFontSettingSubLayout2->addWidget(new QLabel(QStringLiteral("透明度")));
    pFontSettingSubLayout2->addWidget(m_pCharacteSlider);
    pFontSettingSubLayout2->addSpacing(25);

    pFontSettingLayout->addLayout(pFontSettingSubLayout1);
    pFontSettingLayout->addLayout(pFontSettingSubLayout2);
    pFontSettingBox->setLayout(pFontSettingLayout);

    // 任务栏设置
    QGroupBox *pTaskBaSettingBox       = new QGroupBox(QStringLiteral("任务栏设置"), this);
    QHBoxLayout *pTaskBarSettingLayout = new QHBoxLayout;

    m_pTaskBarAutoHideBox             = new QCheckBox(QStringLiteral("自动隐藏"));
    m_pTaskBarColorBtn                = new QPushButton(QStringLiteral("背景色"));
    m_pTaskBarTransparentgradientBtn  = new QRadioButton(QStringLiteral("纯色"));;
    m_pTaskBarBlurbehindBtn           = new QRadioButton(QStringLiteral("磨砂"));;
    m_pTaskBarAlphaSlider             = new QSlider;


    m_pTaskBarAutoHideBox->setChecked(true);
    m_pTaskBarColorBtn->setIcon(QIcon(":/image/image/color.png"));
    m_pTaskBarAlphaSlider->setOrientation(Qt::Horizontal);
    m_pTaskBarAlphaSlider->setRange(0, 255);
    m_pTaskBarAlphaSlider->setStyleSheet("QSlider::groove{border: 1px solid #999999;background: #ffffff;}"
                               "QSlider::handle {border: 1px solid #999999;background: #88bbff;}"
                               "QSlider::groove:horizontal {height: 3px;left:5px; right: 5px;}"
                               "QSlider::handle:horizontal{width: 6px;margin: -7px;}"
                               "QSlider::add-page{border: 1px solid #999999;background:#eeeeef;}"
                               "QSlider::sub-page{background: #eeaa22;}");

    pTaskBarSettingLayout->addWidget(m_pTaskBarAutoHideBox);
    pTaskBarSettingLayout->addWidget(m_pTaskBarColorBtn);
    pTaskBarSettingLayout->addSpacing(30);
    pTaskBarSettingLayout->addWidget(m_pTaskBarTransparentgradientBtn);
    pTaskBarSettingLayout->addWidget(m_pTaskBarBlurbehindBtn);
    pTaskBarSettingLayout->addWidget(m_pTaskBarAlphaSlider);

    pTaskBaSettingBox->setLayout(pTaskBarSettingLayout);

    // 程序设置
    QGroupBox *pAppSettingBox      = new QGroupBox(QStringLiteral("程序设置"), this);
    QHBoxLayout *pAppSettingLayout = new QHBoxLayout;
    QLabel *pUrlLbl                = new QLabel();
    m_pAutoRuningCheckBox          = new QCheckBox(QStringLiteral("开机自启动"));

    pUrlLbl->setOpenExternalLinks(true);
    pUrlLbl->setText(QStringLiteral("<a href=\"%1\">访问作者网站").arg(AuthorUrl));

    pAppSettingLayout->addWidget(m_pAutoRuningCheckBox);
    pAppSettingLayout->addStretch();
    pAppSettingLayout->addWidget(pUrlLbl);
    pAppSettingBox->setLayout(pAppSettingLayout);

    // 整体布局
    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(pResSettingBox);
    pMainLayout->addWidget(pEffectSettingBox);
    pMainLayout->addWidget(pFontSettingBox);
    pMainLayout->addWidget(pTaskBaSettingBox);
    pMainLayout->addWidget(pAppSettingBox);
    setLayout(pMainLayout);

    setWindowIcon(QIcon(":/image/image/logo.ico"));
    setWindowTitle(QStringLiteral("简单桌面 - 设置"));
    setFixedSize(500, 450);
}

void MainWindow::initSystemTray()
{
    QMenu *menu           = new QMenu;
    m_pTrayIcon           = new QSystemTrayIcon(this);
    m_pSysTraySetAction   = new QAction(QIcon(":/image/image/setting.png"), QStringLiteral("设置"), menu);
    m_pSysTrayAboutAction = new QAction(QIcon(":/image/image/about.png"),QStringLiteral("关于"), menu);
    m_pSysTrayHelpAction  = new QAction(QIcon(":/image/image/help.png"),QStringLiteral("帮助"), menu);
    m_pSysTrayExitAction  = new QAction(QIcon(":/image/image/exit.png"),QStringLiteral("退出"), menu);

    menu->addAction(m_pSysTraySetAction);
    menu->addSeparator();
    menu->addAction(m_pSysTrayAboutAction);
    menu->addAction(m_pSysTrayHelpAction);
    menu->addSeparator();
    menu->addAction(m_pSysTrayExitAction);

    m_pTrayIcon->setContextMenu(menu);
    m_pTrayIcon->setToolTip(QStringLiteral("简单桌面"));
    m_pTrayIcon->setIcon(QIcon(":/image/image/logo.ico"));

    m_pTrayIcon->show();
}

void MainWindow::initConctol()
{
    m_pInstance = new VlcInstance(VlcCommon::args(), this);
    m_pPlayer   = new VlcMediaPlayer(m_pInstance);

    connect(m_pSelectResourcesBtn, &QPushButton::clicked, this, &MainWindow::onSelectResourcesBtnClicked);
    connect(m_pSysTraySetAction,   &QAction::triggered,   this, &MainWindow::show);
    connect(m_pSysTrayAboutAction, &QAction::triggered,   this, &MainWindow::onSysTrayAboutActionTrigger);
    connect(m_pSysTrayHelpAction,  &QAction::triggered,   this, &MainWindow::onSysTrayHelpActionTrigger);
    connect(m_pSysTrayExitAction,  &QAction::triggered,   [=](){
        QApplication::exit(0);
    });

    connect(m_pAutoRuningCheckBox, &QCheckBox::stateChanged, [=](int val){
        QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
        if (val)
            reg.setValue(qApp->applicationName(), QDir::toNativeSeparators(qApp->applicationFilePath()));
        else
            reg.setValue(QCoreApplication::applicationName(), "");
    });

    connect(m_pCharacteFontBtn, &QToolButton::clicked, [=](){
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok, m_pCharacterLbl->font(), this, QStringLiteral("字体选择"));
        if (ok)
            m_pCharacterLbl->setFont(font);
    });

    connect(m_pCharacteColorBtn, &QToolButton::clicked, [=](){
        QColorDialog cd(this);
        cd.setCurrentColor(m_pCharacterLbl->color());
        cd.setOption(QColorDialog::ShowAlphaChannel);
        if (cd.exec() == QColorDialog::Accepted)
        {
            m_pCharacteSlider->setValue(cd.selectedColor().alpha());
            m_pCharacterLbl->setColor(cd.selectedColor());
        }
    });

    connect(m_pCharacterVisibleBox, &QCheckBox::stateChanged, this, &MainWindow::onCharacteLblCheckShow);
    connect(m_pCharacteEdit, &QLineEdit::textChanged, m_pCharacterLbl, &CharacterLabel::setText);
    connect(m_pCharacteXBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MainWindow::onCharacteLblMove);
    connect(m_pCharacteYBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MainWindow::onCharacteLblMove);
    connect(m_pCharacteSlider, &QSlider::valueChanged, this, &MainWindow::SetCharacteLbOpacity);

    connect(m_pVolumeSlider, &QSlider::valueChanged, [=](int val){
        if (m_pVedioLbl != nullptr)
            m_pPlayer->audio()->setVolume(val);
    });

    connect(m_pTaskBarAutoHideBox, &QCheckBox::clicked, [=](){
        m_pTaskbarControl->setAutoHide(m_pTaskBarAutoHideBox->isChecked());

    });

    connect(m_pTaskBarTransparentgradientBtn, &QRadioButton::clicked, [=](int state){
        if (state)
            m_pTaskbarControl->setAccentState(TaskbarControl::ACCENT_ENABLE_TRANSPARENTGRADIENT);
    });

    connect(m_pTaskBarBlurbehindBtn, &QRadioButton::clicked, [=](int state){
        if (state)
            m_pTaskbarControl->setAccentState(TaskbarControl::ACCENT_ENABLE_BLURBEHIND);
    });

    connect(m_pTaskBarColorBtn, &QPushButton::clicked, [=](){
        QColorDialog cd(this);
        cd.setCurrentColor(m_pTaskbarControl->color());
        cd.setOption(QColorDialog::ShowAlphaChannel);
        if (cd.exec() == QColorDialog::Accepted)
        {
            m_pTaskBarAlphaSlider->setValue(cd.selectedColor().alpha());
            m_pTaskbarControl->setColor(cd.selectedColor());
        }
    });

    connect(m_pTaskBarAlphaSlider, &QSlider::valueChanged, [=](int val){
        QColor color = m_pTaskbarControl->color();
        color.setAlpha(val);
        m_pTaskbarControl->setColor(color);
    });

    connect(m_pTrayIcon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason){
        if (reason == QSystemTrayIcon::Trigger)
            this->show();
    });

    connect(m_pPlayer, &VlcMediaPlayer::end, this, [=](){
        loadResourcesFile();
    });
}

bool MainWindow::loadResourcesFile()
{
    if (m_filesPath.count() <= 0 && m_pMovieLbl == nullptr && m_pImageLbl == nullptr && m_pVedioLbl == nullptr)
        return false;

    removeAllWallpaper();

    if (m_filesPath.at(0).endsWith(QStringLiteral(".gif"), Qt::CaseInsensitive))
    {
        createMovieWallpaper(m_filesPath.at(0));

        m_pCharacterLbl->setVisible(m_pCharacterVisibleBox->isChecked());
    }
    else if (m_filesPath.at(0).endsWith(QStringLiteral(".jpg"), Qt::CaseInsensitive)
          || m_filesPath.at(0).endsWith(QStringLiteral(".jpeg"), Qt::CaseInsensitive)
          || m_filesPath.at(0).endsWith(QStringLiteral(".jpe"), Qt::CaseInsensitive)
          || m_filesPath.at(0).endsWith(QStringLiteral(".jfif"), Qt::CaseInsensitive)
          || m_filesPath.at(0).endsWith(QStringLiteral(".png"), Qt::CaseInsensitive)
          || m_filesPath.at(0).endsWith(QStringLiteral(".ico"), Qt::CaseInsensitive))
    {
        createImageWallpaper(m_filesPath);

        m_pCharacterLbl->setVisible(m_pCharacterVisibleBox->isChecked());
    }
    else if (m_filesPath.at(0).endsWith(QStringLiteral(".mp4"), Qt::CaseInsensitive)
          || m_filesPath.at(0).endsWith(QStringLiteral(".flv"), Qt::CaseInsensitive)
          || m_filesPath.at(0).endsWith(QStringLiteral(".rmvb"), Qt::CaseInsensitive)
          || m_filesPath.at(0).endsWith(QStringLiteral(".avi"), Qt::CaseInsensitive)
          || m_filesPath.at(0).endsWith(QStringLiteral(".mkv"), Qt::CaseInsensitive))
    {
        createVideoWallpaper(m_filesPath.at(0));
    }

    return true;
}

HWND MainWindow::findDeskTopWindow()
{
    PDWORD_PTR result = nullptr;
    HWND hWorkerW = nullptr;
    HWND hDefView = nullptr;

    SendMessageTimeout(FindWindow(L"Progman",NULL), 0x52c, 0, 0, SMTO_NORMAL, 1000, result);

    hWorkerW = FindWindowEx(NULL, NULL, L"WorkerW", NULL);

    while ((!hDefView) && hWorkerW)
    {
        hDefView = FindWindowEx(hWorkerW, NULL, L"SHELLDLL_DefView", NULL);
        hWorkerW = FindWindowEx(NULL, hWorkerW, L"WorkerW", NULL);
    }

    ShowWindow(hWorkerW,0);

    return FindWindow(L"Progman",NULL);
}

void MainWindow::removeAllWallpaper()
{
    if (m_pVedioLbl != nullptr)
        m_pPlayer->stop();

    delete m_pMovieLbl;
    delete m_pImageLbl;
    delete m_pVedioLbl;

    m_pMovieLbl = nullptr;
    m_pImageLbl = nullptr;
    m_pVedioLbl = nullptr;

    m_images.clear();
    m_imageIndex = 0;
}

void MainWindow::createImageWallpaper(const QStringList &files)
{
    QPixmap pic;
    for (auto item : files)
        if (pic.load(item))
            m_images.append(pic);

    if (m_images.count() > 0)
    {
        m_pImageLbl = new QLabel();

        m_pImageLbl->installEventFilter(this);
        m_pImageLbl->setWindowFlag(Qt::FramelessWindowHint);
        m_pImageLbl->setScaledContents(true);
        m_pImageLbl->setPixmap(m_images.at(0));
        m_pImageLbl->showFullScreen();
        SetParent((HWND)m_pImageLbl->winId(), findDeskTopWindow());
        m_pImageLbl->show();

        if (m_images.count() > 1)
        {
            QTimer *timer = new QTimer(m_pImageLbl);

            connect(timer, &QTimer::timeout, [=](){
                m_pImageLbl->setPixmap(m_images.at(m_imageIndex));
                m_imageIndex = ++m_imageIndex % m_images.count();
            });

            connect(m_pTimeIntervalSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int val){
                if (timer!= nullptr)
				{
					timer->stop();
					timer->start(val * 1000);
				}
            });

            timer->start(m_pTimeIntervalSpinBox->value() * 1000);
        }
    }
}

void MainWindow::createMovieWallpaper(const QString &file)
{
   QMovie *movie = new QMovie(file);
   m_pMovieLbl   = new QLabel();

   m_pMovieLbl->installEventFilter(this);
   m_pMovieLbl->setWindowFlag(Qt::FramelessWindowHint);
   m_pMovieLbl->setMovie(movie);
   m_pMovieLbl->setScaledContents(true);
   m_pMovieLbl->showFullScreen();
   movie->setParent(m_pMovieLbl);
   SetParent((HWND)m_pMovieLbl->winId(), findDeskTopWindow());
   m_pMovieLbl->show();
   movie->start();
}

void MainWindow::createVideoWallpaper(const QString &file)
{
    m_pVedioLbl     = new VlcWidgetVideo();
    VlcMedia *media = new VlcMedia(file, true, m_pInstance);

    m_pVedioLbl->installEventFilter(this);
    SetParent((HWND)m_pVedioLbl->winId(), findDeskTopWindow());
    media->setParent(media);
    media->setOption(":–directx-use-sysmem");
    media->setOption(":avcodec-threads=0");
    media->setOption(":avcodec-fast");
    m_pPlayer->setVideoWidget(m_pVedioLbl);
    m_pVedioLbl->showFullScreen();
    m_pPlayer->audio()->setVolume(m_pVolumeSlider->value());
    m_pPlayer->open(media);
}

void MainWindow::createDefaultWallpaper(const QString &filePath)
{
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,(void*)(filePath.toStdWString().c_str()), SPIF_SENDWININICHANGE |SPIF_UPDATEINIFILE);
}

void MainWindow::saveState()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());

    settings.beginGroup("Ui");
    settings.setValue("resType", m_pResourcesFilesRadioBtn->isChecked());
    settings.setValue("imageTime", m_pTimeIntervalSpinBox->value());
    settings.setValue("vedioVolume", m_pVolumeSlider->value());
    settings.setValue("characterVisible", m_pCharacterVisibleBox->isChecked());
    settings.setValue("characteText", m_pCharacteEdit->text());
    settings.setValue("characteX", m_pCharacteXBox->value());
    settings.setValue("characteY", m_pCharacteYBox->value());
    settings.setValue("characteSlider", m_pCharacteSlider->value());
    settings.setValue("taskBarAutoHideBox", m_pTaskBarAutoHideBox->isChecked());
    settings.setValue("taskBarTransparentgradientBtn", m_pTaskBarTransparentgradientBtn->isChecked());
    settings.setValue("taskBarBlurbehindBtn", m_pTaskBarBlurbehindBtn->isChecked());
    settings.setValue("taskBarAlphaSlider", m_pTaskBarAlphaSlider->value());
    settings.setValue("autoRuning", m_pAutoRuningCheckBox->isChecked());
    settings.endGroup();

    settings.beginGroup("Parameter");
    settings.setValue("resFilePath", m_filesPath);
    settings.setValue("characteFont", m_pCharacterLbl->font());
    settings.setValue("characteColor", m_pCharacterLbl->color());
    settings.setValue("taskBarColor", m_pTaskbarControl->color());
    settings.endGroup();
}

void MainWindow::restoreState()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());

    if (settings.status() != QSettings::NoError)
        return;

    settings.beginGroup("Ui");
    settings.value("resType").toBool() ? m_pResourcesFilesRadioBtn->setChecked(true) : m_pResourcesFileRadioBtn->setChecked(true);
    m_pTimeIntervalSpinBox->setValue(settings.value("imageTime").toInt());
    m_pVolumeSlider->setValue(settings.value("vedioVolume").toInt());
    m_pCharacterVisibleBox->setChecked(settings.value("characterVisible").toBool());
    m_pCharacteEdit->setText(settings.value("characteText").toString());
    m_pCharacteXBox->setValue(settings.value("characteX").toInt());
    m_pCharacteYBox->setValue(settings.value("characteY").toInt());
    m_pCharacteSlider->setValue(settings.value("characteSlider").toInt());
    m_pTaskBarAutoHideBox->setChecked(settings.value("taskBarAutoHideBox").toBool());
    m_pTaskBarTransparentgradientBtn->setChecked(settings.value("taskBarTransparentgradientBtn").toBool());
    m_pTaskBarBlurbehindBtn->setChecked(settings.value("taskBarBlurbehindBtn").toBool());
    m_pTaskBarAlphaSlider->setValue(settings.value("taskBarAlphaSlider").toInt());
    m_pAutoRuningCheckBox->setChecked(settings.value("autoRuning").toBool());
    settings.endGroup();

    settings.beginGroup("Parameter");
    m_filesPath = settings.value("resFilePath").toStringList();
    m_pCharacterLbl->setFont(settings.value("characteFont").value<QFont>());
    m_pCharacterLbl->setColor(settings.value("characteColor").value<QColor>());
    m_pTaskbarControl->setColor(settings.value("taskBarColor").value<QColor>());
    settings.endGroup();

    m_pCharacterLbl->setText(m_pCharacteEdit->text());
    m_pCharacterLbl->move(m_pCharacteXBox->value(), m_pCharacteYBox->value());
    SetCharacteLbOpacity(m_pCharacteSlider->value());

    m_pTaskbarControl->setAutoHide(m_pTaskBarAutoHideBox->isChecked());
    if (m_pTaskBarTransparentgradientBtn->isChecked()) m_pTaskbarControl->setAccentState(TaskbarControl::ACCENT_ENABLE_TRANSPARENTGRADIENT);
    else if (m_pTaskBarBlurbehindBtn->isChecked()) m_pTaskbarControl->setAccentState(TaskbarControl::ACCENT_ENABLE_BLURBEHIND);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveState();
    hide();
    event->ignore();
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == m_pImageLbl || object == m_pMovieLbl || object == m_pVedioLbl)
    {
        switch (event->type())
        {
        case QEvent::Show:
            m_pCharacterLbl->setParent(qobject_cast<QWidget*>(object));
            if (m_pCharacterVisibleBox->isChecked() && object != m_pVedioLbl)
                m_pCharacterLbl->show();
            break;
        case QEvent::Hide:
            m_pCharacterLbl->hide();
            m_pCharacterLbl->setParent(this);
            break;
        case QEvent::Enter:
            SetParent((HWND)qobject_cast<QWidget*>(object)->winId(), findDeskTopWindow());
            break;
        default:
            break;
        }
    }

    return QWidget::eventFilter(object, event);
}

void MainWindow::onSelectResourcesBtnClicked()
{
    QFileDialog fd(this);

    fd.setWindowIcon(QIcon(":/image/image/logo.ico"));
    fd.setWindowTitle(QStringLiteral("选择资源文件"));
    fd.setAcceptMode(QFileDialog::AcceptOpen);

    QStringList fileFilters;
    fileFilters.append("图片文件(*.jpg *.jpeg *.jpe *.jfif *.png *.ico)");

    if (m_pResourcesFileRadioBtn->isChecked())
    {
        fileFilters.append("动画文件(*.gif)");
        fileFilters.append("视频文件(*.flv *.rmvb *.avi *.mp4 *.mkv)");
        fd.setFileMode(QFileDialog::ExistingFile);
    }
    else
    {
        fd.setFileMode(QFileDialog::ExistingFiles);
    }

    fd.setNameFilters(fileFilters);

    if (fd.exec() == QFileDialog::Accepted)
    {
        m_filesPath = fd.selectedFiles();
        loadResourcesFile();
    }
}

void MainWindow::onSysTrayAboutActionTrigger()
{
    QMessageBox message(this);

    showMinimized();

    message.setWindowTitle(QStringLiteral("关于 简单桌面"));
    message.setWindowIcon(QIcon(":/image/image/logo.ico"));
    message.setText(QStringLiteral("版本 1.2.31\n"
                                   "CopyRight © 1994-2020\n"
                                   "TianSong Studio\n\n"
                                   "简单桌面是一款便捷灵活的桌面壁纸管理软件\n"
                                   "此应用不会收集任何用户信息，甚至不会访问系统网络,"
                                   "为了使用安全，请前往作者网站页下载\n"
                                   "问题及使用建议反馈：1508539502@qq.com"));

    message.exec();

    showNormal();
    hide();
}

void MainWindow::onSysTrayHelpActionTrigger()
{
    QMessageBox message(this);

    showMinimized();

    message.setWindowTitle(QStringLiteral("简单桌面"));
    message.setWindowIcon(QIcon(":/image/image/logo.ico"));
    message.setText(QStringLiteral("联系简单桌面，可通过1508539502@qq.com\n\n"
                                   "觉得软件流氓？！乱弹弹窗？这说明你可能下载了非官方软件！遇到上述问题，请先到官方：%1 下载官方版本软件试试！\n").arg(AuthorUrl));

    message.exec();

    showNormal();
    hide();
}

void MainWindow::onCharacteLblMove()
{
    m_pCharacterLbl->move(m_pCharacteXBox->value(), m_pCharacteYBox->value());
}

void MainWindow::onCharacteLblCheckShow(bool sta)
{
    if (m_pMovieLbl != nullptr || m_pImageLbl != nullptr)
        m_pCharacterLbl->setVisible(sta);
}

void MainWindow::SetCharacteLbOpacity(int val)
{
    QColor color = m_pCharacterLbl->color();
    color.setAlpha(val);
    m_pCharacterLbl->setColor(color);
}
