#include "myhouse.h"
#include "ui_myhouse.h"

Myhouse::Myhouse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Myhouse)
{
    ui->setupUi(this);
    //初始状态，false为关闭，true为打开
    state_lamp1 = false;
    state_lamp2 = false;
    state_fan1 = false;
    state_fan2 = false;
    state_air = false;
    //关闭温度湿度滑块拖动
    ui->verticalSlider_humidity->setEnabled(false);
    ui->verticalSlider_temperature->setEnabled(false);

    //灯的槽函数
    connect(ui->pushButton_lamp1,&QPushButton::clicked,[=](){
        if(state_lamp1)
        {
            ui->pushButton_lamp1->setStyleSheet("border-image: url(:/pictur/lamp_close.png);");//切换关灯的图片
            state_lamp1 = false;
            emit signal_sendCommondToClient("lamp1","close");   //给客户端发送一个关灯信号
        }
        else
        {
            ui->pushButton_lamp1->setStyleSheet("border-image: url(:/pictur/lamp_open.png);");//切换开灯的图片
            state_lamp1 = true;
            emit signal_sendCommondToClient("lamp1","open");    //给客户端发送一个开灯信号
        }
        //加上一个对总开关的判断
        if(state_lamp1 && state_lamp2)
            ui->pushButton_lampTotal->setText("关灯");
        if(!state_lamp1 && !state_lamp2)
            ui->pushButton_lampTotal->setText("开灯");
    });
    connect(ui->pushButton_lamp2,&QPushButton::clicked,[=](){
        if(state_lamp2)
        {
            ui->pushButton_lamp2->setStyleSheet("border-image: url(:/pictur/lamp_close.png);");//切换关灯的图片
            state_lamp2 = false;
            emit signal_sendCommondToClient("lamp2","close");   //给客户端发送一个关灯信号
        }
        else
        {
            ui->pushButton_lamp2->setStyleSheet("border-image: url(:/pictur/lamp_open.png);");//切换开灯的图片
            state_lamp2 = true;
            emit signal_sendCommondToClient("lamp2","open");    //给客户端发送一个开灯信号
        }
        //加上一个对总开关的判断
        if(state_lamp1 && state_lamp2)
            ui->pushButton_lampTotal->setText("关灯");
        if(!state_lamp1 && !state_lamp2)
            ui->pushButton_lampTotal->setText("开灯");
    });

    //风扇的槽函数
    connect(ui->pushButton_fan1,&QPushButton::clicked,[=](){
        if(state_fan1)
        {
            ui->pushButton_fan1->setStyleSheet("border-image: url(:/pictur/fan_close.png);");//切换关风扇的图片
            state_fan1 = false;
            emit signal_sendCommondToClient("fan1","close");   //给客户端发送一个关风扇信号
        }
        else
        {
            ui->pushButton_fan1->setStyleSheet("border-image: url(:/pictur/fan_open.png);");//切换开风扇的图片
            state_fan1 = true;
            emit signal_sendCommondToClient("fan1","open");    //给客户端发送一个开风扇信号
        }
        //加上一个对总开关的判断
        if(state_fan1 && state_fan2)
            ui->pushButton_fanTotal->setText("关风扇");
        if(!state_fan1 && !state_fan2)
            ui->pushButton_fanTotal->setText("开风扇");
    });
    connect(ui->pushButton_fan2,&QPushButton::clicked,[=](){
        if(state_fan2)
        {
            ui->pushButton_fan2->setStyleSheet("border-image: url(:/pictur/fan_close.png);");//切换关风扇的图片
            state_fan2 = false;
            emit signal_sendCommondToClient("fan2","close");   //给客户端发送一个关风扇信号
        }
        else
        {
            ui->pushButton_fan2->setStyleSheet("border-image: url(:/pictur/fan_open.png);");//切换开风扇的图片
            state_fan2 = true;
            emit signal_sendCommondToClient("fan2","open");    //给客户端发送一个开风扇信号
        }
        //加上一个对总开关的判断
        if(state_fan1 && state_fan2)
            ui->pushButton_fanTotal->setText("关风扇");
        if(!state_fan1 && !state_fan2)
            ui->pushButton_fanTotal->setText("开风扇");
    });
    //空调的槽函数
    connect(ui->pushButton_air,&QPushButton::clicked,[=](){
        if(state_air)
        {
            ui->pushButton_air->setStyleSheet("border-image: url(:/pictur/air_close.png);");//切换关空调的图片
            state_air = false;
            emit signal_sendCommondToClient("air","close");   //给客户端发送一个关空调信号
        }
        else
        {
            ui->pushButton_air->setStyleSheet("border-image: url(:/pictur/air_open.png);");//切换开空调的图片
            state_air = true;
            emit signal_sendCommondToClient("air","open");    //给客户端发送一个开空调信号
        }
    });
    //风扇和灯的总开关槽函数
    connect(ui->pushButton_fanTotal,&QPushButton::clicked,[=](){
        if(ui->pushButton_fanTotal->text() == "开风扇")
        {
            ui->pushButton_fan1->setStyleSheet("border-image: url(:/pictur/fan_open.png);");//切换开风扇的图片
            ui->pushButton_fan2->setStyleSheet("border-image: url(:/pictur/fan_open.png);");//切换开风扇的图片
            state_fan1 = true;
            state_fan2 = true;
//            emit signal_sendCommondToClient("fan1","open");    //给客户端发送一个开风扇信号
//            emit signal_sendCommondToClient("fan2","open");      //给客户端发送一个开风扇信号
            ui->pushButton_fanTotal->setText("关风扇");
        }
        else
        {
            ui->pushButton_fan1->setStyleSheet("border-image: url(:/pictur/fan_close.png);");//切换关风扇的图片
            ui->pushButton_fan2->setStyleSheet("border-image: url(:/pictur/fan_close.png);");//切换关风扇的图片
            state_fan1 = false;
            state_fan2 = false;
//            emit signal_sendCommondToClient("fan1","close");   //给客户端发送一个关风扇信号
//            emit signal_sendCommondToClient("fan2","close");   //给客户端发送一个关风扇信号
            ui->pushButton_fanTotal->setText("开风扇");
        }
    });
    connect(ui->pushButton_lampTotal,&QPushButton::clicked,[=](){
        if(ui->pushButton_lampTotal->text() == "开灯")
        {
            ui->pushButton_lamp1->setStyleSheet("border-image: url(:/pictur/lamp_open.png);");//切换开灯的图片
            ui->pushButton_lamp2->setStyleSheet("border-image: url(:/pictur/lamp_open.png);");//切换开灯的图片
            state_lamp1 = true;
            state_lamp2 = true;
//            emit signal_sendCommondToClient("lamp1","open");    //给客户端发送一个开灯信号
//            emit signal_sendCommondToClient("lamp2","open");    //给客户端发送一个开灯信号
            ui->pushButton_lampTotal->setText("关灯");
        }
        else
        {
            ui->pushButton_lamp1->setStyleSheet("border-image: url(:/pictur/lamp_close.png);");//切换关灯的图片
            ui->pushButton_lamp2->setStyleSheet("border-image: url(:/pictur/lamp_close.png);");//切换关灯的图片
            state_lamp1 = false;
            state_lamp2 = false;
//            emit signal_sendCommondToClient("lamp1","close");   //给客户端发送一个关灯信号
//            emit signal_sendCommondToClient("lamp2","close");   //给客户端发送一个关灯信号
            ui->pushButton_lampTotal->setText("开灯");
        }
    });

    //温度控制
    //1.设置初始化温度和范围
    ui->verticalSlider_temperature->setRange(0,50);
    ui->spinBox_temperature->setRange(0,50);
    ui->verticalSlider_temperature->setValue(25);
    ui->spinBox_temperature->setValue(25);
    //2.绑定Slider和SpinBox
    connect(ui->verticalSlider_temperature,&QSlider::valueChanged,[=](int value){
        ui->spinBox_temperature->setValue(value);//同步值
    });
    void (QSpinBox:: *signal_spinBox)(int) = QSpinBox::valueChanged;//信号函数有重载，使用函数指针区分
    connect(ui->spinBox_temperature,signal_spinBox,[=](int value){
        ui->verticalSlider_temperature->setValue(value);//同步值

        emit signal_sendCommondToClient("temperature",QString::number(value));//发送当前的温度
    });

    //湿度控制
    //1.设置初始化湿度和范围
    ui->verticalSlider_humidity->setRange(0,100);
    ui->spinBox_humidity->setRange(0,100);
    ui->verticalSlider_humidity->setValue(40);
    ui->spinBox_humidity->setValue(40);
    //2.绑定Slider和SpinBox
    connect(ui->verticalSlider_humidity,&QSlider::valueChanged,[=](int value){
        ui->spinBox_humidity->setValue(value);//同步值
    });
    void (QSpinBox:: *signal_spinBox1)(int) = QSpinBox::valueChanged;//信号函数有重载，使用函数指针区分
    connect(ui->spinBox_humidity,signal_spinBox1,[=](int value){
        ui->verticalSlider_humidity->setValue(value);//同步值
        emit signal_sendCommondToClient("humidity",QString::number(value));//发送当前的湿度
    });

}

Myhouse::~Myhouse()
{
    delete ui;
}

//收到客户端信息，手动发送clicked信号
void Myhouse::CommondFromClient(QString key,QString value)
{
    if(value == "open")
        flag = true;//将value值转换为bool值
    else
        flag = false;
    if(key == "lamp1")
    {
        if(state_lamp1 != flag)    //判断当前灯的状态是否相同
            QMetaObject::invokeMethod(ui->pushButton_lamp1, "clicked", Qt::QueuedConnection);//手动发出clicked信号
    }
    if(key == "lamp2")
    {
        if(state_lamp2 != flag)    //判断当前灯的状态是否相同
            QMetaObject::invokeMethod(ui->pushButton_lamp2, "clicked", Qt::QueuedConnection);//手动发出clicked信号
    }
    if(key == "fan1")
    {
        if(state_fan1 != flag)    //判断当前风扇的状态是否相同
            QMetaObject::invokeMethod(ui->pushButton_fan1, "clicked", Qt::QueuedConnection);//手动发出clicked信号
    }
    if(key == "fan2")
    {
        if(state_fan2 != flag)    //判断当前风扇的状态是否相同
            QMetaObject::invokeMethod(ui->pushButton_fan2, "clicked", Qt::QueuedConnection);//手动发出clicked信号
    }
    if(key == "air")
    {
        if(state_air != flag)    //判断当前空调的状态是否相同
            QMetaObject::invokeMethod(ui->pushButton_air, "clicked", Qt::QueuedConnection);//手动发出clicked信号
    }
    if(key == "temperature")
    {
       ui->spinBox_temperature->setValue(value.toUInt());
    }
    if(key == "humidity")
    {
       ui->spinBox_humidity->setValue(value.toUInt());
    }

}
