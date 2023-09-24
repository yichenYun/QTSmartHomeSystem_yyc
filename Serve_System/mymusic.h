#ifndef MYMUSIC_H
#define MYMUSIC_H

#include <QWidget>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QVideoWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
namespace Ui {
class Mymusic;
}

class Mymusic : public QWidget
{
    Q_OBJECT

public:
    explicit Mymusic(QWidget *parent = nullptr);
    ~Mymusic();

private slots:
    void on_pushButton_playORpause_clicked();


private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QMovie *movie;
    QNetworkAccessManager *http;

    void parseData(QByteArray data);//解析服务器返回数据函数
public slots:
    void slot_request();//请求数据槽函数
    void slot_reply(QNetworkReply *reply);//接收请求的数据槽函数

signals:
    void signal_playState(QString state);//发送播放状态的信号函数
    void signal_sendCommondToClient(QString key,QString value);//发送给客户端信息的信号函数
public:
    void CommondFromClient(QString key,QString value);//接收客户端发来的消息，并执行对应的操作


private:
    Ui::Mymusic *ui;
};

#endif // MYMUSIC_H
