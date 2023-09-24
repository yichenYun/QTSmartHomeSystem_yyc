#include "mymusic.h"
#include "ui_mymusic.h"

#include<QMediaContent>
#include<QUrl>
#include<QMessageBox>
#include<QFileDialog>
#include<QList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QMovie>
Mymusic::Mymusic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mymusic)
{
    ui->setupUi(this);
    //实例化对象
    this->player = new QMediaPlayer(this);
    this->playlist = new QMediaPlaylist(this);
    //添加音乐到播放列表
    playlist->addMedia(QMediaContent(QUrl("qrc:/music/Dan + Shay、Justin Bieber - 10,000 Hours.mp3")));
    playlist->addMedia(QMediaContent(QUrl("qrc:/music/Alec Benjamin - Oh My God.mp3")));
    player->setPlaylist(playlist);
    //添加到ui
    ui->listWidget->addItem("10,000 Hours.mp3");
    ui->listWidget->addItem("Oh My God.mp3");
    //下一曲
    connect(ui->pushButton_next,&QPushButton::clicked,playlist,&QMediaPlaylist::next);
    //上一曲
    connect(ui->pushButton_pre,&QPushButton::clicked,playlist,&QMediaPlaylist::previous);
    //添加
    connect(ui->pushButton_add,&QPushButton::clicked,[=](){
        QList<QUrl> path = QFileDialog::getOpenFileUrls(this,"选择文件",QUrl("./"),"音频(*.mp3);;视频(*.mp4);;(*.*)");
        for(int i = 0;i < path.size();i++)
        {
            playlist->addMedia(QMediaContent(path[i]));//添加音乐到播放列表
            ui->listWidget->addItem(path[i].fileName());//添加到ui
        }
    });
    //删除
    connect(ui->pushButton_del,&QPushButton::clicked,[=](){
        int row = ui->listWidget->currentRow();//获取当前选中的行
        playlist->removeMedia(row);//删除音乐
        ui->listWidget->takeItem(row);//删除ui音乐
    });
    //给多媒体播放列表与 ui 显示的列表建立联系
    ui->listWidget->setCurrentRow(0);//设置默认选择为第一行
    connect(ui->listWidget,&QListWidget::currentRowChanged,playlist,&QMediaPlaylist::setCurrentIndex);//选中歌名，播放选中歌曲
    connect(playlist,&QMediaPlaylist::currentIndexChanged,[=](int index){//歌曲自动变化时，改变选中
        if(index<0 || index >= ui->listWidget->count())//防止越界
        {
            return;
        }
        ui->listWidget->setCurrentRow(index);//设置当前行
    });
    //播放模式
    void(QComboBox:: *combox_signal)(int) = QComboBox::currentIndexChanged;//改变值发送对应信号
    connect(ui->comboBox_list,combox_signal,[=](int index){
        switch (index) {
        case 0:
            playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//单曲循环
            break;
        case 1:
            playlist->setPlaybackMode(QMediaPlaylist::Sequential);//顺序播放
            break;
        case 2:
            playlist->setPlaybackMode(QMediaPlaylist::Loop);//列表循环
            break;
        case 3:
            playlist->setPlaybackMode(QMediaPlaylist::Random);//随机播放
            break;
        }
    });
    //音量
    player->setVolume(70);
    ui->verticalSlider->setValue(70);   //设置初始音量
    connect(ui->verticalSlider,&QSlider::valueChanged,player,&QMediaPlayer::setVolume);//滑块信号和音量槽绑定

    //视频播放
//    player->setVideoOutput(ui->widget_3);

    //标签播放gif
    movie = new QMovie("://pictur/music.gif");
    ui->label_gif->setMovie(movie);
    ui->label_gif->setScaledContents(true);

    //播放进度条
    connect(player,&QMediaPlayer::positionChanged,[=](qint64 position){
        //qDebug()<<player->duration();
        //ui->Slider_time->setRange(0,player->duration()/1000);
        ui->Slider_time->setValue(position/1000);
        ui->label_currentTime->setText("00:"+QString::number(position/1000));
    });
    connect(player,&QMediaPlayer::durationChanged,[=](){
        qDebug()<<player->duration();
    });
//    connect(ui->Slider_time,&QSlider::valueChanged,player,[=](int value){
//        player->setPosition(value*1000);
//    });


    //在线搜索
    //1.导入network模块到工程文件,QNetworkAccessManager
    //2.实例化QNetworkAccessManager对象
    this->http = new QNetworkAccessManager(this);
    //3.发送请求信息
    connect(ui->pushButton_find,&QPushButton::clicked,this,slot_request);
    //4.等待响应
    connect(http,&QNetworkAccessManager::finished,this,slot_reply);
}

Mymusic::~Mymusic()
{
    delete ui;
}

void Mymusic::slot_request()//请求数据的槽函数
{
    QString name = ui->lineEdit->text();
    if(name.size() == 0 || name.size()>10)
    {
        QMessageBox::critical(this,"错误信息","长度过短或过长，请重新输入!");
        return;
    }
    QString path = "http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s="+name+"&type=1&offset=0&total=true&limit=2";
    http->get(QNetworkRequest(QUrl(path)));
}

void Mymusic::slot_reply(QNetworkReply *reply)//接收数据的槽函数
{
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    //判断返回状态
    if(status != 200)
    {
        QMessageBox::critical(this,"错误信息","未找到！");
        return;
    }
    //解析数据
    QByteArray data = reply->readAll();
    parseData(data);//调用解析数据函数
    emit signal_sendCommondToClient("music",data);//发送信号给客户端
}
void Mymusic::parseData(QByteArray data)//解析服务器返回数据函数
{
    QJsonDocument document=QJsonDocument::fromJson(data);
    if(document.isObject())
    {
        QJsonObject obj = document.object();
        if(obj.find("result") != obj.end())
        {
            if(obj["result"].isObject())
            {
                QJsonObject obj_1 = obj["result"].toObject();
                QJsonArray array = obj_1["songs"].toArray();
                obj_1 = array[0].toObject();
                int id = obj_1["id"].toInt();
                //音乐播放路径
                QString urls = "http://music.163.com/song/media/outer/url?id=" + QString::number(id) + ".mp3";
                //添加到音乐播放列表
                playlist->addMedia(QMediaContent(QUrl(urls)));
                ui->listWidget->addItem(obj_1["name"].toString()+".mp3");
            }
        }
    }
}


void Mymusic::on_pushButton_playORpause_clicked()
{
    if(ui->pushButton_playORpause->text() == "播放")
    {
        player->play();
        movie->start();
        ui->pushButton_playORpause->setText("暂停");
        emit signal_playState("音乐状态：播放");
    }
    else
    {
        player->pause();
        movie->stop();
        ui->pushButton_playORpause->setText("播放");
        emit signal_playState("音乐状态：暂停");
    }
}

void Mymusic::CommondFromClient(QString key, QString value)
{
    QByteArray data = value.toUtf8();
    if(key == "music")
        parseData(data);
}


