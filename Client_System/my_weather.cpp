#include "my_weather.h"
#include "ui_my_weather.h"

My_weather::My_weather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::My_weather)
{
    ui->setupUi(this);
}

My_weather::~My_weather()
{
    delete ui;
}
