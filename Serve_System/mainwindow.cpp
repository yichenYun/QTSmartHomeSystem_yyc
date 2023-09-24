#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QLabel>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/pictur/tou.ico"));
    my_music = new Mymusic(this);   //实例化智能家居对象
    my_house = new Myhouse(this);   //实例化音乐播放器对象
    my_weather = new Myweather(this);   //实例化天气预报对象
    my_camera = new Mycamera(this); //实例化监控对象
    ui->tabWidget->clear();
    ui->tabWidget->addTab(my_house,"家居");//添加智能家居界面
    ui->tabWidget->addTab(my_music,"音乐");//添加音乐播放器界面
    ui->tabWidget->addTab(my_weather,"天气");//添加天气界面
    ui->tabWidget->addTab(my_camera,"监控");//添加天气界面

    //状态栏
    QLabel *state_client = new QLabel("连接状态：未连接",this);
    QLabel *state_music = new QLabel("音乐状态：暂停",this);
    ui->statusbar->addWidget(state_client);
    ui->statusbar->addPermanentWidget(state_music);

    //设置播放状态
    connect(my_music,&Mymusic::signal_playState,this,[=](QString state){
        state_music->setText(state);
    });

    //服务器搭建
    //1.实例化服务器对象
    server = new QTcpServer(this);
    //2.设置监听listen,等待连接
    server->listen(QHostAddress::Any,8888);
    //3.有连接时，接收数据并处理
    connect(server,&QTcpServer::newConnection,[=](){
        qDebug()<<"有客户端连接！";
        state_client->setText("连接状态：连接");//更新状态栏信息
        socket = server->nextPendingConnection();//客户端连接的套接字
        //接收客户端数据
        connect(socket,&QTcpSocket::readyRead,[=](){
            QByteArray data = socket->readAll();
            qDebug()<<"客户端数据"<<data;
            this->reciveMessage(data);
        });
        //判断客户端是否下线
        connect(socket,&QTcpSocket::disconnected,[=](){
            socket->close();
            qDebug()<<"客户端断开连接！";
            state_client->setText("连接状态：未连接");//更新状态栏信息
        });

    });

    //发数据   
    connect(my_house,&Myhouse::signal_sendCommondToClient,[=](QString key,QString value){//家居
        qDebug()<<"要发送给客户端的数据函数";
        if(socket->state() == QAbstractSocket::ConnectedState) //判断 客户端是否连接服务器 避免发送空数据 造成死机
            sendMessage(key,value);
    });
    connect(my_camera,&Mycamera::signal_sendCommondToClient,[=](QString key,QString value){//摄像头
        qDebug()<<"要发送给客户端的数据函数"<<key<<value;
        if(socket->state() == QAbstractSocket::ConnectedState) //判断 客户端是否连接服务器 避免发送空数据 造成死机
            sendMessage(key,value);
    });
    connect(my_music,&Mymusic::signal_sendCommondToClient,[=](QString key,QString value){//音乐播放器
        qDebug()<<"要发送给客户端的数据函数"<<key<<value;
        if(socket->state() == QAbstractSocket::ConnectedState) //判断 客户端是否连接服务器 避免发送空数据 造成死机
            sendMessage(key,value);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendMessage(QString key,QString value)
{
    //1.定义json对象保存数据
    QJsonObject obj;
    obj.insert(key,value);
    //2.将json对象数据转换为QString数据
    QJsonDocument document;
    document.setObject(obj);//将json对象添加到json文档中
    QByteArray data = document.toJson();//转换为QByteArray
    //3.发送到客户端
    socket->write(data.data());
}

void MainWindow::reciveMessage(QByteArray data)
{
   //1.把QString类型转为json格式
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject obj = document.object();
    //2.利用迭代器取出key值
    QJsonObject::iterator it = obj.begin();
    QString key = it.key();
    //3.将数据传到家居界面处理
    qDebug()<<"receveMessage"<<key<<obj[key].toString();
    if(key == "camera")
        my_camera->CommondFromClient(key,obj[key].toString());
    else
        if(key == "music")
            my_music->CommondFromClient(key,obj[key].toString());
        else
            my_house->CommondFromClient(key,obj[key].toString());
}
