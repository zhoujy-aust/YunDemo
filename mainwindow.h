#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common/common.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //显示主窗口
    void showMainWindow();
    //处理信号
    void managerSignals();
    //重新登录
    void loginAgain();

signals:
    //切换用户按钮信号
    void changeUser();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;

    Common m_common;
};
#endif // MAINWINDOW_H
