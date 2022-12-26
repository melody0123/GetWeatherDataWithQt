#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>
#include <QtCore>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSqlTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;  //请求句柄
    QString previous_city; // 上一次保存的城市
    QString fengli;       //风力
    QString wendu;        //温度
    QString weather_type;  //天气类型
    QSqlDatabase m_db;
    QSqlTableModel *model;
    void connectDB();
private slots:
    void onGetWeatherButtonClicked();
    void replyFinished(QNetworkReply *reply);
    void onShowAllButtonClicked();
    void onSaveButtonClicked();
    void onAutoButtonClicked();
    void timerGetWeather();
    void timerUpdate();
};
#endif // MAINWINDOW_H
