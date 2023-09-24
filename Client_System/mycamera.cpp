#include "mycamera.h"
#include "ui_mycamera.h"

#include <QDebug>
#include <QMediaRecorder>
Mycamera::Mycamera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mycamera)
{
    ui->setupUi(this);
    ui->pushButton_shutter->setEnabled(false);//拍照按钮初始化为不可选
    ui->pushButton_del->setEnabled(false);//删除按钮初始化为不可选

    state_camera = false;

    //添加摄像头类型信息
    cameraList = QCameraInfo::availableCameras();
    for(int i = 0;i< cameraList.size();i++)//摄像头类型
        ui->comboBox->addItem(cameraList[i].description());//摄像头类型信息添加到ui

    camera = new QCamera(ui->comboBox->currentText().toUtf8(),this);   //创建摄像头对象
    camera->setCaptureMode(QCamera::CaptureStillImage);//设置捕捉模式,静态
    camera->setViewfinder(ui->widget);  //将监控画面提升到QVideWidget控件中

    //开启摄像头或关闭摄像头
    connect(ui->pushButton_camera,&QPushButton::clicked,[=](){
        if(ui->pushButton_camera->text() == "开启摄像头")
        {
            state_camera = true;
            camera->start();
            ui->widget->show();//显示窗口
            ui->pushButton_camera->setText("关闭摄像头");
            ui->pushButton_shutter->setEnabled(true);//拍照按钮为可选
        }
        else
        {
            state_camera = false;
            camera->stop();
            ui->widget->hide();//隐藏窗口，表面上像关闭掉了
            ui->pushButton_camera->setText("开启摄像头");
            ui->pushButton_shutter->setEnabled(false);//拍照按钮为不可选
        }
    });


    //处理捕获照片
    imageCapture = new QCameraImageCapture(camera);//实例化媒体记录类对象
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);//设置目标保存位置为文件
    //捕获拍照信息的
    connect(imageCapture,&QCameraImageCapture::imageCaptured,[=](int , const QImage &preview){
        QPixmap p = QPixmap::fromImage(preview);
        ui->label_picture->setScaledContents(true);//设置为包容图片内容
        ui->label_picture->setPixmap(p);//显示图片到标签上面
        ui->pushButton_del->setEnabled(true);//删除按钮可选
    });
    //拍照按钮
    connect(ui->pushButton_shutter,&QPushButton::clicked,[=](){
       camera->searchAndLock(); //上锁
       imageCapture->capture();  //拍照
       camera->unlock();   //解锁
    });
    //删除照片
    connect(ui->pushButton_del,&QPushButton::clicked,[=](){
        ui->label_picture->clear();
        ui->pushButton_del->setEnabled(false);//删除按钮不可选
    });

    //开启客户端摄像头信号
    connect(ui->pushButton_client,&QPushButton::clicked,[=](){
        if(ui->pushButton_client->text() == "开启对方摄像头")
        {
            ui->pushButton_client->setText("关闭对方摄像头");
            emit signal_sendCommondToClient("camera","open");//同步信号
        }
        else
        {
            ui->pushButton_client->setText("开启对方摄像头");
            emit signal_sendCommondToClient("camera","close");//同步信号
        }

    });
}

Mycamera::~Mycamera()
{
    delete ui;
}

void Mycamera::CommondFromClient(QString key, QString value)
{
    if(key == "camera" && value == "open")
        if(!state_camera)
                QMetaObject::invokeMethod(ui->pushButton_camera, "clicked", Qt::QueuedConnection);//手动发出clicked信号
    if(key == "camera" && value == "close")
        if(state_camera)
            QMetaObject::invokeMethod(ui->pushButton_camera, "clicked", Qt::QueuedConnection);//手动发出clicked信号
}
