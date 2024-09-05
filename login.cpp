#include "login.h"
#include "ui_login.h"
#include <QPainter>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    //初始化网络请求类
    m_manager = Common::getNetManager();
    m_mainWin  = new MainWindow;
    m_myfilewg = new MyFileWg;
    m_mainWin->setWindowIcon(QIcon(":/images/yunpan.png"));

    //去除边框
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    //设置当前窗口所以字体
    this->setFont(QFont("新宋体",12,QFont::Bold, false));

    // 密码输入框 设置为密码方式显示
    ui->password->setEchoMode(QLineEdit::Password);
    ui->reg_pw->setEchoMode(QLineEdit::Password);
    ui->reg_conf->setEchoMode(QLineEdit::Password);

    ui->stackedWidget->setCurrentWidget(ui->login_page);
    ui->user->setFocus();
    //数据的格式提示
    ui->user->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");
    ui->reg_user->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");
    ui->reg_nick->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");
    ui->password->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");
    ui->reg_pw->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");
    ui->reg_conf->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");
    loadLoginConfig();// 读取配置文件信息，并初始化
    m_cm.getFileTypeList();// 加载图片信息 - 显示文件列表的时候用，在此初始化

    //title_wg信号处理
    connect(ui->title_widget,&Title_Wg::closeWindow,this, [=]()
    {
        //判断当前stackwidght显示的页面
        if(ui->stackedWidget->currentWidget() == ui->set_page)
        {
            //切换到登录
            ui->stackedWidget->setCurrentWidget(ui->login_page);
            //情况控件数据
            ui->ip->clear();
            ui->port->clear();
            ui->user->setFocus();
        }
        else if(ui->stackedWidget->currentWidget() == ui->reg_page)
        {
            //切换到登录
            ui->stackedWidget->setCurrentWidget(ui->login_page);
            //情况控件数据
            ui->reg_user->clear();
            ui->reg_nick->clear();
            ui->reg_pw->clear();
            ui->reg_conf->clear();
            ui->reg_phone->clear();
            ui->reg_mail->clear();
            ui->user->setFocus();
        }
        else
        {
            this->close();
        }
    });
    connect(ui->title_widget,&Title_Wg::showSetWg,this, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->set_page);
        ui->ip->setFocus();
    });
    connect(ui->toolButton, &QToolButton::clicked, this, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->reg_page);
        ui->reg_user->setFocus();
    });

    // 切换用户 - 重新登录
    connect(m_mainWin, &MainWindow::changeUser, this, [=]()
    {
        m_mainWin->hide();
        this->show();
    });
    connect(m_myfilewg, &MyFileWg::loginAgainSignal, this, [=]()
    {
        m_mainWin->hide();
        this->show();
    });
}

Login::~Login()
{
    delete ui;
}

void Login::paintEvent(QPaintEvent *)
{
    //给窗口画背景图
    QPainter painter(this);
    QPixmap pixmap(":/images/login_bk.jpg");
    painter.drawPixmap(0, 0, width(), height(), pixmap);
}

//注册
void Login::on_toolButton_4_clicked()
{
    //取设置数据
    QString ip = ui->ip->text();
    QString port = ui->port->text();
    //注册数据
    QString name = ui->reg_user->text();
    QString nick = ui->reg_nick->text();
    QString passwd = ui->reg_pw->text();
    QString confirm = ui->reg_conf->text();
    QString phone = ui->reg_phone->text();
    QString mail = ui->reg_mail->text();
    //检验,参考server设置
    static QRegularExpression exp;
    exp.setPattern(USER_REG);
    QRegularExpressionMatch match = exp.match(name);
    if(!match.hasMatch())
    {
        QMessageBox::warning(this,"警告","用户名格式不正确");
        ui->reg_user->clear();
        ui->reg_user->setFocus();//控件设置焦点
        return;
    }
    match = exp.match(nick);
    if(!match.hasMatch())
    {
        QMessageBox::warning(this,"警告","昵称格式不正确");
        ui->reg_nick->clear();
        ui->reg_nick->setFocus();//控件设置焦点
        return;
    }
    exp.setPattern(PASSWD_REG);
    match = exp.match(passwd);
    if(!match.hasMatch())
    {
        QMessageBox::warning(this,"警告","密码格式不正确");
        ui->reg_pw->clear();
        ui->reg_pw->setFocus();//控件设置焦点
        return;
    }
    match = exp.match(confirm);
    if(!match.hasMatch())
    {
        QMessageBox::warning(this,"警告","密码格式不正确");
        ui->reg_conf->clear();
        ui->reg_conf->setFocus();//控件设置焦点
        return;
    }
    if(confirm != passwd)
    {
        QMessageBox::warning(this, "警告", "两次输入的密码不匹配, 请重新输入");
        ui->reg_pw->clear();
        ui->reg_conf->setFocus();
        return;
    }
    exp.setPattern(PHONE_REG);
    match = exp.match(phone);
    if(!match.hasMatch())
    {
        QMessageBox::warning(this,"警告","电话格式不正确");
        ui->reg_phone->clear();
        ui->reg_phone->setFocus();//控件设置焦点
        return;
    }
    exp.setPattern(EMAIL_REG);
    match = exp.match(mail);
    if(!match.hasMatch())
    {
        QMessageBox::warning(this,"警告","邮箱格式不正确");
        ui->reg_mail->clear();
        ui->reg_mail->setFocus();//控件设置焦点
        return;
    }

    //将用户输入的注册数据转换成json
    QByteArray postData = getRegJson(name,nick,m_cm.getStrMd5(passwd),phone,mail);
    //http请求协议头
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, postData.size());
    //url
    QString url = QString("http://%1:%2/reg").arg(ip, port);
    request.setUrl(QUrl(url));
    QNetworkReply* reply = m_manager->post(request, postData);
    //接受server返回的数据
    connect(reply, &QNetworkReply::readyRead, this, [=]()
    {
        //读返回的数据
        QByteArray jsondata = reply->readAll();
        //解析json字符串判断
        QString status = m_cm.getCode(jsondata);

        if("002" == status)
        {
            //success
            //将当前注册用户信息填写到登录输入框，清除注册信息,跳转登录页面
            ui->user->setText(name);
            ui->password->setText(passwd);
            ui->reg_user->clear();
            ui->reg_conf->clear();
            ui->reg_mail->clear();
            ui->reg_nick->clear();
            ui->reg_pw->clear();
            ui->reg_phone->clear();
            ui->stackedWidget->setCurrentWidget(ui->login_page);
        }
        else if("003" == status)
        {
            // 该用户已存在
            QMessageBox::warning(this, "警告", "该用户已存在");
            return;
        }
        else if("004" == status)
        {
            //failure
            QMessageBox::critical(this, "警告", "注册失败");
            return;
        }
        delete reply;
    });
}

//服务器设置
void Login::on_toolButton_3_clicked()
{
    //获取控件数据
    QString ip = ui->ip->text();
    QString port = ui->port->text();
    //IP校验
    static QRegularExpression exp;
    exp.setPattern(IP_REG);
    QRegularExpressionMatch match = exp.match(ip);
    if(!match.hasMatch())
    {
        QMessageBox::warning(this,"警告","IP格式不正确");
        ui->ip->clear();
        ui->ip->setFocus();//控件设置焦点
        return;
    }
    //端口校验
    exp.setPattern(PORT_REG);
    match = exp.match(port);
    if(!match.hasMatch())
    {
        QMessageBox::warning(this,"警告","Port格式不正确");
        ui->port->clear();
        ui->port->setFocus();//控件设置焦点
        return;
    }
    ui->stackedWidget->setCurrentWidget(ui->login_page);
    //保存到配置文件
   m_cm.writeWebInfo(ip, port);
}

QByteArray Login::getRegJson(QString user, QString nick, QString pwd, QString phone, QString mail)
{
    QJsonObject obj;
    obj.insert("userName", user);
    obj.insert("nickName", nick);
    obj.insert("firstPwd", pwd);
    obj.insert("phone", phone);
    obj.insert("email", mail);
    //obj -> jsondoc
    QJsonDocument doc = QJsonDocument(obj);
    return doc.toJson();
}

QByteArray Login::getLoginJson(QString user, QString pwd)
{
    QJsonObject obj;
    obj.insert("user", user);
    obj.insert("pwd", pwd);
    QJsonDocument doc = QJsonDocument(obj);
    return doc.toJson();
}

QStringList Login::parseLoginJson(QByteArray json)
{
    QStringList list;
    QJsonDocument doc = QJsonDocument::fromJson(json);
    if(doc.isObject())
    {
        QJsonObject obj = doc.object();
        cout << "服务器返回的数据" << obj;
        list.append(obj.value("code").toString());
        list.append(obj.value("token").toString());
    }
    return list;
}

void Login::loadLoginConfig()
{
    QString user = m_cm.getCfgValue("login", "user");
    QString passwd = m_cm.getCfgValue("login", "pwd");
    QString remember = m_cm.getCfgValue("login", "remember");

    int pwdlen = 0;
    int ret = 0;
    unsigned char pwdbuf[BUFSIZ] = {0};
    //记住密码
    if(remember == "yes")
    {
        //解码base64
        //toLocal8Bit(), 转换为本地字符集，默认windows则为gbk编码，linux为utf-8编码
        QByteArray pwd = QByteArray::fromBase64(passwd.toLocal8Bit());
        //解码des
        ret = DesDec((unsigned char *)pwd.data(),pwd.size(), pwdbuf, &pwdlen);
        if(ret != 0)
        {
            cout << "DesDec error\n";
            return;
        }
    #ifdef _WIN32 //如果是windows平台
        //fromLocal8Bit(), 本地字符集转换为utf-8
        ui->password->setText( QString::fromLocal8Bit( (const char *)pwdbuf, pwdlen) );
    #else //其它平台
        ui->password->setText( (const char *)encPwd );
    #endif
        ui->remember->setChecked(true);
    }
    else
    {
        ui->password->clear();
        ui->remember->setChecked(false);
    }

    //用户名
    int userlen = 0;
    unsigned char userbuf[BUFSIZ] = {0};
    QByteArray usr = QByteArray::fromBase64(user.toLocal8Bit());
    ret = DesDec((unsigned char*)usr.data(), usr.size(), userbuf, &userlen);
    if(ret != 0)
    {
        cout << "DesDec error";
        return;
    }

#ifdef _WIN32 //如果是windows平台
    ui->user->setText( QString::fromLocal8Bit( (const char *)userbuf, userlen ) );
#else //其它平台
    ui->user->setText( (const char *)encUsr );
#endif

    QString ip = m_cm.getCfgValue("web_server", "ip");
    QString port = m_cm.getCfgValue("web_server", "port");
    ui->ip->setText(ip);
    ui->port->setText(port);
}

//登录
void Login::on_toolButton_2_clicked()
{
    //获取控件数据
    QString user = ui->user->text();
    QString passwd = ui->password->text();
    QString ip = ui->ip->text();
    QString port = ui->port->text();
    //数据校验
    static QRegularExpression exp;
    exp.setPattern(USER_REG);
    QRegularExpressionMatch match = exp.match(user);
    if(!match.hasMatch())
    {
        QMessageBox::warning(this,"警告","用户名格式不正确");
        ui->user->clear();
        ui->user->setFocus();//控件设置焦点
        return;
    }
    exp.setPattern(PASSWD_REG);
    match = exp.match(passwd);
    if(!match.hasMatch())
    {
        QMessageBox::warning(this,"警告","密码格式不正确");
        ui->password->clear();
        ui->password->setFocus();//控件设置焦点
        return;
    }
    //将登录数据保存到配置文件
    m_cm.writeLoginInfo(user, passwd, ui->remember->isChecked());
    //将数据打包为json格式数据, 密码经过md5加密,getStrMd5()
    QByteArray data = getLoginJson(user, m_cm.getStrMd5(passwd));

    //http请求协议头
    QNetworkRequest request;
    QString url = QString("http://%1:%2/login").arg(ip, port);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    request.setUrl(QUrl(url));
    // 向服务器发送post请求
    QNetworkReply* reply = m_manager->post(request, data);//将数据以post格式发送给server
    cout << "post url:" << url << "post data: " << data;
    // 接收服务器发回的http响应消息
    connect(reply, &QNetworkReply::finished, this, [=]()
    {
        // 出错了
        if (reply->error() != QNetworkReply::NoError)
        {
            cout << reply->errorString();
            //释放资源
            reply->deleteLater();
            return;
        }
        QByteArray json = reply->readAll();
        QStringList list = parseLoginJson(json);
        if("000" == list.at(0))
        {
            // 登录成功
            // 当前用户信息存储起来
            LoginInfoInstance* info = LoginInfoInstance::getInstance();
            info->setLoginInfo(user, ip, port, list.at(1));
            cout << info->getUser().toUtf8().data() << ", " << info->getIp() << ", " << info->getPort() << list.at(1);
            // 隐藏当前窗口
            this->hide();
            // 显示主窗口
//            m_mainWin->show();
            m_mainWin->showMainWindow();
        }
        else
        {
            // 登录失败
            QMessageBox::warning(this, "登录失败", "用户名或密码不正确！！！");
        }
        reply->deleteLater(); //释放资源
    });
}

