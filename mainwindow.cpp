#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);  //新建QNetworkAccessManager对象
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));//关联信号和槽
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onGetWeatherButtonClicked()
{
    QString local_city = ui->city->text().trimmed(); //获得需要查询天气的城市名称
    char quest_array[256]="http://portalweather.comsys.net.cn/weather03/api/weatherService/getDailyWeather?cityName=";
    QNetworkRequest quest;
    sprintf(quest_array,"%s%s",quest_array,local_city.toUtf8().data());
    quest.setUrl(QUrl(quest_array));
    quest.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
    /*发送get网络请求*/
    manager->get(quest);
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    qDebug()<<"recv weather data!!";
    QString all = reply->readAll();
    ui->resultEdit->setText(all); //将接收到的数据显示出来

    QJsonParseError err;
    QJsonDocument json_recv = QJsonDocument::fromJson(all.toUtf8(),&err);//解析json对象
    qDebug() << err.error;
    if(!json_recv.isNull())
    {
        QJsonObject object = json_recv.object();

        if(object.contains("results"))
        {
            QJsonValue value = object.value("results");  // 获取指定 key 对应的 value
            if(value.isArray())
            {
                QJsonObject data = value.toArray().at(0).toObject();
                if(data.contains("daily"))
                {
                    QJsonValue weather_data_array = data.value("daily");
                    if(weather_data_array.isArray())
                    {
                        QJsonObject today_weather = weather_data_array.toArray().at(0).toObject();
                        weather_type = today_weather.value("text_day").toString();

                        QString low = today_weather.value("low").toString();
                        QString high = today_weather.value("high").toString();
                        wendu = low.mid(low.length()-3,4) +"~"+ high.mid(high.length()-3,4);
                        QString strength = today_weather.value("wind_scale").toString();
                        strength.remove(0,8);
                        strength.remove(strength.length()-2,2);
                        fengli = today_weather.value("wind_direction").toString() + strength;
                        ui->type->setText(weather_type); //显示天气类型
                        ui->wendu->setText(wendu);   //显示温度
                        ui->fengli->setText(fengli); //显示风力
                    }

                }
            }

        }else
        {
            qDebug()<<"json_recv is NULL or is not a object !!";
        }
        reply->deleteLater(); //销毁请求对象
    }
}

