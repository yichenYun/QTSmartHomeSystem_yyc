#ifndef MYWEATHER_H
#define MYWEATHER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMap>
#include <QMovie>

namespace Ui {
class Myweather;
}

class Myweather : public QWidget
{
    Q_OBJECT

public:
    explicit Myweather(QWidget *parent = nullptr);
    ~Myweather();

private:
    Ui::Myweather *ui;
    //声明网络访问管理对象
    QNetworkAccessManager  *http;
    QMap<QString,QString> city_id;//保存城市与对应id

    QMovie *movie;//定义播放动图类对象
public slots:
    void slot_reply(QNetworkReply *reply);//接收请求数据的槽函数
    void slot_request();//请求数据的槽函数
};

#endif // MYWEATHER_H
