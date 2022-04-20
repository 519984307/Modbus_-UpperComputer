#ifndef PREFEED_H
#define PREFEED_H

#include <QDialog>
#include <QTimer>
#include <QDateTime>
#include "common.h"


namespace Ui {
class preFeed;
}


class preFeed : public QDialog
{
    Q_OBJECT

public:
    explicit preFeed(QWidget *parent = 0);
    ~preFeed();
    void init();
    bool importconfig();
    bool clickFlag;
    bool importSuccess;
    bool redisSuccess;
    void connectRedis();
    void safeQuit();
    QString getPlclistParameter();
    QString getRegisterParameter();
    QString getDetailParameter();
    QString getDisplayParameter();
    QString getPlanParameter();

private slots:
    void on_bset_clicked();
    void logClearTimeUpdate();
    void message(QString msg);
    void modbusRead(int com);
    void modbusWrite(int com);

    void registerFinished();
    void plclistFinished();
    void detailFinished();
    void displayFinished();
    void planFinished();

    void httpRequest(QString url,QString requestInfo);
    void stopCom(int com);
    void reStartCom(int com);
    void error(int com);
    void noerror(int com);

    void on_initialize_initset_clicked();
    void on_bsynclow_initset_clicked();
    void on_breturn_initset_clicked();
    void on_quit_initset_clicked();
    void on_bregister_clicked();
    void on_syn_initset_clicked();
    void on_display_initset_clicked();
    void on_detail_initset_clicked();
    void on_plan_initset_clicked();
    void on_localCach_initset_clicked();
    void on_redis_initset_clicked();

private:
    void timerEvent(QTimerEvent *event);
    int mousecount=0;
    Ui::preFeed *ui;
    int systemTimer;
    QDateTime   dt;
    QTime       time;
    QDate       date;    
    QTimer logClearTimer,modBusTimer;
    MonitorThread monitorThread;
    bool initModbus(int com);
};

#endif // PREFEED_H
