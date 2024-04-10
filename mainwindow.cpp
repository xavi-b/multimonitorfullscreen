#include "mainwindow.h"

#include <QMoveEvent>
#include <QResizeEvent>
#include <QEvent>
#include <QDebug>
#include <QPushButton>

#include <QApplication>

#include <X11/Xlib.h>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QX11Info>
#else
#include <QGuiApplication>
#include <xcb/xcb.h>
#endif

int handler(Display * d, XErrorEvent * e)
{
    qDebug() << "Error code: " << e->error_code;
    return 0;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_NativeWindow, true);

    qDebug() << "winId" << winId();
    XSetErrorHandler(handler);

    QPushButton* btn = new QPushButton("Toggle");
    connect(btn, &QPushButton::clicked, this, [=](){
        qDebug() << "Click";
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        auto display = QX11Info::display();
#else
        auto display = qApp->nativeInterface<QNativeInterface::QX11Application>()->display();
#endif
        int screen = DefaultScreen(display);

        XEvent e1;
        e1.xany.type = ClientMessage;
        e1.xany.window = winId();
        e1.xclient.message_type = XInternAtom(display, "_NET_WM_FULLSCREEN_MONITORS", 0);
        e1.xclient.format = 32;
        // screen indices
        e1.xclient.data.l[0] = 0;
        e1.xclient.data.l[1] = 0;
        e1.xclient.data.l[2] = 1;
        e1.xclient.data.l[3] = 1;
        e1.xclient.data.l[4] = 0;
        XSendEvent(display, RootWindow(display, screen),
                   0, SubstructureNotifyMask | SubstructureRedirectMask,
                   &e1);
        QApplication::sync();
        XEvent e2;
        e2.xany.type = ClientMessage;
        e2.xany.window = winId();
        e2.xclient.message_type = XInternAtom(display, "_NET_WM_STATE", 0);
        e2.xclient.format = 32;
        e2.xclient.data.l[0] = 2; // toggle
        e2.xclient.data.l[1] = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", 0);
        XSendEvent(display, RootWindow(display, screen),
                   0, SubstructureNotifyMask | SubstructureRedirectMask,
                   &e2);
        QApplication::sync();
    });
    setCentralWidget(btn);
}

MainWindow::~MainWindow() {}

void MainWindow::moveEvent(QMoveEvent *e)
{
    qDebug() << "moveEvent"<< e->pos() << e ->oldPos();
    QWidget::moveEvent(e);
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    qDebug() << "resizeEvent"<< e->size() << e ->oldSize();
    QWidget::resizeEvent(e);
}

void MainWindow::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::WindowStateChange) {
        qDebug() << "changeEvent:" << size() << "state=" << windowState()
                 << "oldState=" << (static_cast<QWindowStateChangeEvent*>(e))->oldState();
    }
    QWidget::changeEvent(e);
}
