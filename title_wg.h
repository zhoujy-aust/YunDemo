#ifndef TITLE_WG_H
#define TITLE_WG_H

#include <QWidget>

namespace Ui {
class Title_Wg;
}

class Title_Wg : public QWidget
{
    Q_OBJECT

public:
    explicit Title_Wg(QWidget *parent = nullptr);
    ~Title_Wg();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void showSetWg();
    void closeWindow();

private:
    Ui::Title_Wg *ui;
    QPoint m_pt;//鼠标当前位置减去窗口左上角

    QWidget* m_parent;
};

#endif // TITLE_WG_H
