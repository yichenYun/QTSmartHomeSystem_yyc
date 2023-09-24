#ifndef MYHOUSE_H
#define MYHOUSE_H

#include <QWidget>
namespace Ui {
class Myhouse;
}

class Myhouse : public QWidget
{
    Q_OBJECT

public:
    explicit Myhouse(QWidget *parent = nullptr);
    ~Myhouse();

private:
    Ui::Myhouse *ui;

    //定义状态标志符
    bool state_lamp1;
    bool state_lamp2;
    bool state_fan1;
    bool state_fan2;
    bool state_air;
    bool flag;//将value值转换为bool值

signals:
    void signal_sendCommondToClient(QString key,QString value);//发送给客户端需要的信号函数

public:
    void CommondFromClient(QString key,QString value);//接收客户端发来的消息，并执行对应的操作
};

#endif // MYHOUSE_H
