#include "title_wg.h"
#include "ui_title_wg.h"
#include <QMouseEvent>

Title_Wg::Title_Wg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Title_Wg)
{
    ui->setupUi(this);

    //logo
    ui->logo->setPixmap(QPixmap(":/images/yunpan.png").scaled(40,40));

    m_parent = parent;

    //按钮功能实现
    connect(ui->set, &QToolButton::clicked, [=]()
    {
        //发送自定义信号
        emit(showSetWg());
    });
    connect(ui->min, &QToolButton::clicked, [=]()
    {
        m_parent->showMinimized();
    });
    connect(ui->close, &QToolButton::clicked, [=]()
    {
        emit(closeWindow());
    });
}

Title_Wg::~Title_Wg()
{
    delete ui;
}

void Title_Wg::mouseMoveEvent(QMouseEvent *event)
{
    //只允许左键拖动
    if(event->buttons() & Qt::LeftButton)
    {
        //窗口跟着鼠标移动
        //窗口左上角 = 鼠标当前位置-差值
        m_parent->move(event->globalPosition().toPoint() - m_pt);
    }
}

void Title_Wg::mousePressEvent(QMouseEvent *event)
{
    //如果鼠标左键按下
    if(event->button() == Qt::LeftButton)
    {
        //求差值 = 鼠标当前位置 - 窗口左上角
        m_pt = event->globalPosition().toPoint() - m_parent->geometry().topLeft();
    }
}
