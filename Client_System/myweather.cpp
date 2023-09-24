#include "myweather.h"
#include "ui_myweather.h"

#include <QDebug>
#include <QUrl>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

Myweather::Myweather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Myweather)
{
    ui->setupUi(this);
    city_id.insert("深圳","101280601");
    city_id.insert("上海","101020100");
    city_id.insert("长沙","101250101");

    this->http = new QNetworkAccessManager(this);

    connect(http,&QNetworkAccessManager::finished,this,&Myweather::slot_reply);
    connect(ui->pushButton_find,&QPushButton::clicked,this,&Myweather::slot_request);
    slot_request();//初始化界面
    movie = new QMovie();//实例化播放gif的类对象
}

Myweather::~Myweather()
{
    delete ui;
}

void Myweather::slot_reply(QNetworkReply *reply)
{
    //判断接收的状态
    int stat = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(stat != 200){
        QMessageBox::critical(this,"错误","请检查网络");
    }
    //获取服务器返回的数据
    QByteArray data = reply->readAll();
    //解码操作
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject obj = document.object();//转换为JSon对象

    QJsonObject cityInfo = obj["cityInfo"].toObject();//获取城市信息
    qDebug()<<cityInfo["parent"].toString()<<cityInfo["city"].toString();
    ui->label_cityInfo->setText(cityInfo["parent"].toString() + cityInfo["city"].toString());//保存 信息

    QJsonObject today = obj["data"].toObject();//获取今天的天气信息
    qDebug()<<today["wendu"].toString()<<today["quality"].toString();
    ui->label_1temp->setText(today["wendu"].toString());
    ui->label_1quality->setText("天气质量："+today["quality"].toString());

    QJsonArray arry = today["forecast"].toArray();//15天的预报数组
    obj = arry[0].toObject();

    QJsonObject todayInfo = arry[0].toObject();//今天的天气预报详细信息
    qDebug()<<todayInfo["high"].toString()<<todayInfo["low"].toString()<<todayInfo["ymd"].toString()<<todayInfo["week"].toString();
    qDebug()<<todayInfo["type"].toString()<<todayInfo["notice"].toString();
    ui->label_1high->setText(todayInfo["high"].toString());
    ui->label_1low->setText(todayInfo["low"].toString());
    ui->label_1date->setText(todayInfo["ymd"].toString());
    ui->label_1week->setText(todayInfo["week"].toString());
    ui->label_1weather->setText(todayInfo["type"].toString());
    ui->label_1notice->setText(todayInfo["notice"].toString());


    QJsonObject tomorrowInfo = arry[1].toObject();//明天的天气预报详细信息
    qDebug()<<tomorrowInfo["high"].toString()<<tomorrowInfo["low"].toString()<<tomorrowInfo["ymd"].toString()<<tomorrowInfo["week"].toString();
    qDebug()<<tomorrowInfo["type"].toString()<<tomorrowInfo["notice"].toString();
    ui->label_2high->setText(tomorrowInfo["high"].toString());
    ui->label_2low->setText(tomorrowInfo["low"].toString());
    ui->label_2date->setText(tomorrowInfo["ymd"].toString());
    ui->label_2week->setText(tomorrowInfo["week"].toString());
    ui->label_2weather->setText(tomorrowInfo["type"].toString());
    ui->label_2notice->setText(tomorrowInfo["notice"].toString());

    QJsonObject futureInfo = arry[2].toObject();//后天的天气预报详细信息
    qDebug()<<futureInfo["high"].toString()<<futureInfo["low"].toString()<<futureInfo["ymd"].toString()<<futureInfo["week"].toString();
    qDebug()<<futureInfo["type"].toString()<<futureInfo["notice"].toString();
    ui->label_3high->setText(futureInfo["high"].toString());
    ui->label_3low->setText(futureInfo["low"].toString());
    ui->label_3date->setText(futureInfo["ymd"].toString());
    ui->label_3week->setText(futureInfo["week"].toString());
    ui->label_3weather->setText(futureInfo["type"].toString());
    ui->label_3notice->setText(futureInfo["notice"].toString());

    if(movie->state() == QMovie::Running)// 先判断一下状态，如果运行就关闭，防止刷新查询重复启动。
        movie->stop();
    //进行动图切换
    if(ui->label_1weather->text() == "小雨" || ui->label_1weather->text() == "中雨" || ui->label_1weather->text() == "暴雨")
        movie->setFileName(":/pictur/rain.gif");//设置路径
    else
        if(ui->label_1weather->text() == "多云" || ui->label_1weather->text() == "阴天")
            movie->setFileName(":/pictur/cloud.gif");//设置路径
        else
            if(ui->label_1weather->text() == "晴天")
                movie->setFileName(":/pictur/sun.gif");//设置路径

    ui->label_gif->clear();//清理上一次的图像
    ui->label_gif->setMovie(movie);//绑定对象
    ui->label_gif->setScaledContents(true);//自适应图片
    movie->start();//启动动图

}

void Myweather::slot_request()
{
    QString city = ui->comboBox->currentText();     //获取城市
    QString url = "http://t.weather.sojson.com/api/weather/city/"+ city_id[city];//请求数据的地址
    http->get(QNetworkRequest(QUrl(url)));      //请求数据
}
