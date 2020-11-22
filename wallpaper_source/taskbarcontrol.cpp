#include "taskbarcontrol.h"

TaskbarControl::TaskbarControl(QObject *parent) : QObject(parent)
{
    HMODULE hUser = GetModuleHandle(L"user32.dll");
    m_hTray = FindWindow(L"Shell_TrayWnd", NULL);
    m_setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");

    if (hUser && m_setWindowCompositionAttribute)
    {
        connect(&m_timer, &QTimer::timeout, this, &TaskbarControl::onUpdateWindowCompositionAttribute);

        m_timer.start(10);
    }
}

void TaskbarControl::setAutoHide(bool hide)
{
    LPARAM lParam = hide ? ABS_AUTOHIDE : ABS_ALWAYSONTOP;

    APPBARDATA apBar;
    memset(&apBar,0,sizeof(apBar));
    apBar.cbSize = sizeof(apBar);
    apBar.hWnd = FindWindow(L"Shell_TrayWnd", NULL);

    if(apBar.hWnd != NULL)
    {
        apBar.lParam = lParam;
        SHAppBarMessage(ABM_SETSTATE, &apBar);
        onUpdateWindowCompositionAttribute();
    }
}

void TaskbarControl::setAccentState(ACCENT_STATE state, QColor color)
{
    m_accentState = state;
    m_color = color;
}

void TaskbarControl::onUpdateWindowCompositionAttribute()
{
    DWORD color = m_color.alpha()<<24|m_color.blue()<<16|m_color.green()<<8|m_color.red();

    ACCENT_POLICY accent = { m_accentState, 2, color, 0 };

    if (m_accentState == ACCENT_ENABLE_BLURBEHIND)
        accent.GradientColor = QColor(255, 255, 255, 0).rgba();

    WINDOWCOMPOSITIONATTRIBDATA data;
    data.Attrib = WCA_ACCENT_POLICY;
    data.pvData = &accent;
    data.cbData = sizeof(accent);

    if (m_setWindowCompositionAttribute)
        m_setWindowCompositionAttribute(m_hTray, &data);
}

QColor TaskbarControl::color() const
{
    return m_color;
}

void TaskbarControl::setColor(const QColor &color)
{
    m_color = color;
}

TaskbarControl::ACCENT_STATE TaskbarControl::accentState() const
{
    return m_accentState;
}

void TaskbarControl::setAccentState(ACCENT_STATE accentState)
{
    m_accentState = accentState;
}

