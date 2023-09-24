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
    QNetworkAccessManager  *http;
    QMap<QString,QString> city_id;

    QMovie *movie;//定义播放动图类对象
public slots:
    void slot_reply(QNetworkReply *reply);
    void slot_request();
};

#endif // MYWEATHER_H
