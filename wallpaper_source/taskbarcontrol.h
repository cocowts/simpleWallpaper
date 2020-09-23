#ifndef TASKBARCONTROL_H
#define TASKBARCONTROL_H

#include <QObject>
#include <QColor>
#include <QTimer>
#include <QThread>

#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

class TaskbarControl : public QObject
{
    Q_OBJECT

public:
    enum ACCENT_STATE
    {
        ACCENT_DISABLED = 0,                    // 禁用
        ACCENT_ENABLE_GRADIENT = 1,             // 单色
        ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,  // 全透明
        ACCENT_ENABLE_BLURBEHIND = 3,           // 毛玻璃
        ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,    // 亚克力
        ACCENT_INVALID_STATE = 5
    };

private:
    enum WINDOWCOMPOSITIONATTRIB
    {
        WCA_UNDEFINED = 0,
        WCA_NCRENDERING_ENABLED = 1,
        WCA_NCRENDERING_POLICY = 2,
        WCA_TRANSITIONS_FORCEDISABLED = 3,
        WCA_ALLOW_NCPAINT = 4,
        WCA_CAPTION_BUTTON_BOUNDS = 5,
        WCA_NONCLIENT_RTL_LAYOUT = 6,
        WCA_FORCE_ICONIC_REPRESENTATION = 7,
        WCA_EXTENDED_FRAME_BOUNDS = 8,
        WCA_HAS_ICONIC_BITMAP = 9,
        WCA_THEME_ATTRIBUTES = 10,
        WCA_NCRENDERING_EXILED = 11,
        WCA_NCADORNMENTINFO = 12,
        WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
        WCA_VIDEO_OVERLAY_ACTIVE = 14,
        WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
        WCA_DISALLOW_PEEK = 16,
        WCA_CLOAK = 17,
        WCA_CLOAKED = 18,
        WCA_ACCENT_POLICY = 19,
        WCA_FREEZE_REPRESENTATION = 20,
        WCA_EVER_UNCLOAKED = 21,
        WCA_VISUAL_OWNER = 22,
        WCA_LAST = 23
    };

    struct WINDOWCOMPOSITIONATTRIBDATA
    {
        WINDOWCOMPOSITIONATTRIB Attrib;
        PVOID pvData;
        SIZE_T cbData;
    };

    struct ACCENT_POLICY
    {
        ACCENT_STATE AccentState;
        DWORD AccentFlags;
        DWORD GradientColor;
        DWORD AnimationId;
    };

    using pfnSetWindowCompositionAttribute = BOOL(WINAPI*)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

public:
    explicit TaskbarControl(QObject *parent = nullptr);
    void setAccentState(ACCENT_STATE state, QColor color);

    QColor color() const;
    ACCENT_STATE accentState() const;

public slots:
    void setAutoHide(bool hide = false);
    void setColor(const QColor &color);
    void setAccentState(ACCENT_STATE accentState);

private:
    QColor m_color = QColor(255, 255, 255, 0);
    ACCENT_STATE m_accentState = ACCENT_ENABLE_GRADIENT;
    QTimer m_timer;
};

#endif // TASKBARCONTROL_H
