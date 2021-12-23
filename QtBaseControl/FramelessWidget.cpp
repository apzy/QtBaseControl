#include "FramelessWidget.h"
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QFile>
#include <qwindow.h>
#include <qevent.h>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#endif

FramelessWidget::FramelessWidget(QWidget* parent)
    :QWidget(parent)
{
    m_resizable = true;
    setAttribute(Qt::WA_StyledBackground, true);
    set_widget_border();
}

void FramelessWidget::set_resizable(const bool enable)
{
    m_resizable = enable;
}

bool FramelessWidget::set_style_file(const QString& filePath)
{
    bool bSuccess = true;

    QFile qssFile(filePath);
    if (qssFile.open(QFile::ReadOnly))
    {
        QString qssStyle;
        qssStyle = qssFile.readAll();
        this->setStyleSheet(qssStyle);
    }
    else
    {
        bSuccess = false;
        qDebug("read qss file failed!");
    }
    qssFile.close();

    return bSuccess;
}

bool FramelessWidget::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
#ifdef Q_OS_WIN
    if (eventType != "windows_generic_MSG")
        return false;

    MSG* msg = static_cast<MSG*>(message);
    QWidget* widget = QWidget::find(reinterpret_cast<WId>(msg->hwnd));
    if (!widget)
        return false;

    switch (msg->message)
    {
    case WM_NCCALCSIZE:
    {
        // 隐藏窗口最大化后的标题栏 
        *result = 0;
        return true;
    }
    case WM_NCHITTEST:
    {
        // 边框拖拽
        if (windowState() == Qt::WindowFullScreen)
        {
            *result = HTCLIENT;
            return true;
        }

        drop_on_border_or_coner(msg, result);

        if (0 != *result)
            return true;
        *result = HTCLIENT;
        return true;
    }
    case WM_GETMINMAXINFO:
    {
        // 设置窗口最大化或全屏后的边距
        if (::IsZoomed(msg->hwnd))
        {
            RECT frame = { 0, 0, 0, 0 };
            AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
            frame.left = abs(frame.left);
            frame.top = abs(frame.bottom);
            QScreen* screen = windowHandle()->screen();
            float fScale = screen->devicePixelRatio();
            widget->setContentsMargins(frame.left / fScale, frame.top / fScale, frame.right / fScale, frame.bottom / fScale);
        }
        else
        {
            widget->setContentsMargins(0, 0, 0, 0);
        }

        *result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
        return true;
    }
    default:
        break;
    }
#endif

    return QWidget::nativeEvent(eventType, message, result);
}

void FramelessWidget::set_widget_border()
{
    this->setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);

#ifdef Q_OS_WIN
    // windows7 need disable.
    DwmEnableComposition(DWM_EC_ENABLECOMPOSITION);
    HWND hwnd = reinterpret_cast<HWND>(winId());
    DWORD style = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);

    const MARGINS shadow = { 1, 1, 1, 1 };
    DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);
#endif

}

void FramelessWidget::drop_on_border_or_coner(const MSG* msg, qintptr* result)
{
    RECT winrect;
    GetWindowRect(HWND(winId()), &winrect);

    int x = GET_X_LPARAM(msg->lParam);
    int y = GET_Y_LPARAM(msg->lParam);

    const LONG border_width = 5;

    if (m_resizable)
    {
        bool resizeWidth = minimumWidth() != maximumWidth();
        bool resizeHeight = minimumHeight() != maximumHeight();

        if (resizeWidth)
        {
            //left border
            if (x >= winrect.left && x < winrect.left + border_width)
            {
                *result = HTLEFT;
            }
            //right border
            if (x < winrect.right && x >= winrect.right - border_width)
            {
                *result = HTRIGHT;
            }
        }
        if (resizeHeight)
        {
            //bottom border
            if (y < winrect.bottom && y >= winrect.bottom - border_width)
            {
                *result = HTBOTTOM;
            }
            //top border
            if (y >= winrect.top && y < winrect.top + border_width)
            {
                *result = HTTOP;
            }
        }
        if (resizeWidth && resizeHeight)
        {
            //bottom left corner
            if (x >= winrect.left && x < winrect.left + border_width &&
                y < winrect.bottom && y >= winrect.bottom - border_width)
            {
                *result = HTBOTTOMLEFT;
            }
            //bottom right corner
            if (x < winrect.right && x >= winrect.right - border_width &&
                y < winrect.bottom && y >= winrect.bottom - border_width)
            {
                *result = HTBOTTOMRIGHT;
            }
            //top left corner
            if (x >= winrect.left && x < winrect.left + border_width &&
                y >= winrect.top && y < winrect.top + border_width)
            {
                *result = HTTOPLEFT;
            }
            //top right corner
            if (x < winrect.right && x >= winrect.right - border_width &&
                y >= winrect.top && y < winrect.top + border_width)
            {
                *result = HTTOPRIGHT;
            }
        }
    }
}
