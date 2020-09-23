#include "taskbarcontrol.h"

TaskbarControl::TaskbarControl(QObject *parent) : QObject(parent)
{
    HWND hTray = FindWindow(L"Shell_TrayWnd", NULL);
    HMODULE hUser = GetModuleHandle(L"user32.dll");
    pfnSetWindowCompositionAttribute setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");

    if (hUser && setWindowCompositionAttribute)
    {
            connect(&m_timer, &QTimer::timeout, [=](){

                DWORD color = m_color.alpha()<<24|m_color.blue()<<16|m_color.green()<<8|m_color.red();

                ACCENT_POLICY accent = { m_accentState, 2, color, 0 };

                if (m_accentState == ACCENT_ENABLE_BLURBEHIND)
                {
                    accent.GradientColor = QColor(255, 255, 255, 0).rgba();
                }

                WINDOWCOMPOSITIONATTRIBDATA data;
                data.Attrib = WCA_ACCENT_POLICY;
                data.pvData = &accent;
                data.cbData = sizeof(accent);
                setWindowCompositionAttribute(hTray, &data);
            });
    }

    m_timer.start(15);
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
    }
}

void TaskbarControl::setAccentState(ACCENT_STATE state, QColor color)
{
    m_accentState = state;
    m_color = color;
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

