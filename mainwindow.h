#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void moveEvent(QMoveEvent* e) override;
    void resizeEvent(QResizeEvent* e) override;
    void changeEvent(QEvent* e) override;
};
#endif // MAINWINDOW_H
