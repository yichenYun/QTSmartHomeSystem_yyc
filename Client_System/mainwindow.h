#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include "mymusic.h"
#include "myhouse.h"
#include "myweather.h"
#include "mycamera.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Myhouse *my_house;//智能家居对象
    Mymusic *my_music;//音乐播放器对象
    Myweather *my_weather;//天气预报对象
    Mycamera *my_camera;//天气预报对象

public:
    void sendMessage(QString key,QString value);//发送信息格式，key为目标，value为命令
    void reciveMessage(QByteArray data);//客户端数据处理函数
    //服务器和套接字对象
    QTcpSocket *socket;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
