#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QWidget>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVideoWidget>
namespace Ui {
class Mycamera;
}

class Mycamera : public QWidget
{
    Q_OBJECT

public:
    explicit Mycamera(QWidget *parent = nullptr);
    ~Mycamera();

private:
    Ui::Mycamera *ui;
    QList<QCameraInfo> cameraList;//摄像头类型
    QCamera *camera;//相机对象
    QCameraImageCapture *imageCapture;//媒体记录对象

    bool state_camera;//监控状态

signals:
    void signal_sendCommondToClient(QString key,QString value);//发送给客户端需要的信号函数

public:
    void CommondFromClient(QString key,QString value);//接收客户端发来的消息，并执行对应的操作
};

#endif // MYCAMERA_H
