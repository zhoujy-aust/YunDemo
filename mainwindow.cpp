#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint); // 去掉边框
    ui->btn_group->setParent(this);// 给菜单窗口传参
    managerSignals();//处理主窗口所有信号
    ui->stackedWidget->setCurrentWidget(ui->myfiles_page);//默认显示我的文件窗口
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMainWindow()
{
    m_common.moveToCenter(this);//居中显示
    ui->stackedWidget->setCurrentWidget(ui->myfiles_page);
    ui->myfiles_page->refreshFiles();//刷新用户文件列表
}

//处理主窗口所有信号
void MainWindow::managerSignals()
{
    //关闭
    connect(ui->btn_group, &ButtonGroup::closeWindow, this, &MainWindow::close);
    //最大化
    connect(ui->btn_group, &ButtonGroup::maxWindow, this, [=]()
    {
        static bool flag = false;
        if(!flag)
        {
            this->showMaximized();
            flag = true;
        }
        else
        {
            this->showNormal();
            flag = false;
        }
    });
    //最小化
    connect(ui->btn_group, &ButtonGroup::minWindow, this, &MainWindow::showMinimized);
    //栈窗口切换我的文件
    connect(ui->btn_group, &ButtonGroup::sigMyFile, this, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->myfiles_page);
        ui->myfiles_page->refreshFiles();// 刷新文件列表
    });
    //分享列表
    connect(ui->btn_group, &ButtonGroup::sigShareList, this, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->sharefile_page);
        ui->sharefile_page->refreshFiles();// 刷新分享列表
    });
    //下载榜
    connect(ui->btn_group, &ButtonGroup::sigDownload, this, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->ranking_page);
        ui->ranking_page->refreshFiles();
    });
    //传输列表
    connect(ui->btn_group, &ButtonGroup::sigTransform, this, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->transfer_page);
    });
    //切换用户
    connect(ui->btn_group, &ButtonGroup::sigSwitchUser, this, [=]()
    {
        qDebug() << "bye bye...";
        loginAgain();
    });
    //stack窗口切换
    connect(ui->myfiles_page, &MyFileWg::gotoTransfer, this, [=](TransferStatus status)
    {
        ui->btn_group->slotButtonClick(Page::TRANSFER);
        if(status == TransferStatus::Uplaod)
        {
            ui->transfer_page->showUpload();
        }
        else if(status == TransferStatus::Download)
        {
            ui->transfer_page->showDownload();
        }
    });
    //信号传递
    connect(ui->sharefile_page, &ShareList::gotoTransfer, ui->myfiles_page, &MyFileWg::gotoTransfer);
}

void MainWindow::loginAgain()
{
    //发送信号，告诉窗口，切换用户
    emit changeUser();
    //清空上一个用户的上传和下载任务
    ui->myfiles_page->clearAllTask();
    //清空上一个用户的文件显示信息
    ui->myfiles_page->clearFileList();
    ui->myfiles_page->clearItems();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap bk(":/images/title_bk3.jpg");
    painter.drawPixmap(0, 0, width(), height(), bk);
}

