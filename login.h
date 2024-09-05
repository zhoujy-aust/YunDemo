#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "common/common.h"
#include "common/logininfoinstance.h"
#include "common/des.h"
#include "mainwindow.h"
#include "myfilewg.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    //注册信息->json
    QByteArray getRegJson(QString user, QString nick, QString pwd, QString phone, QString mail);
    //登录信息->json
    QByteArray getLoginJson(QString user, QString pwd);
    // 解析服务器返回的json字符串
    QStringList parseLoginJson(QByteArray json);
    // 登录时加载配置文件
    void loadLoginConfig();

protected:
    //绘图事件函数
    void paintEvent(QPaintEvent *);

private slots:
    void on_toolButton_4_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::Login *ui;

    Common m_cm;
    MainWindow* m_mainWin; // 主窗口指针
    QNetworkAccessManager* m_manager;// 处理网络请求类对象
    MyFileWg* m_myfilewg;
};

#endif // LOGIN_H
