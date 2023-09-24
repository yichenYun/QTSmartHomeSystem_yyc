#ifndef MY_WEATHER_H
#define MY_WEATHER_H

#include <QWidget>

namespace Ui {
class My_weather;
}

class My_weather : public QWidget
{
    Q_OBJECT

public:
    explicit My_weather(QWidget *parent = nullptr);
    ~My_weather();

private:
    Ui::My_weather *ui;
};

#endif // MY_WEATHER_H
