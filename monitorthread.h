#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include <QThread>
#include "common.h"

class MonitorThread : public QThread
{
    Q_OBJECT

public:
    explicit MonitorThread(QObject *parent = nullptr);
    bool comDevErr(int com);
    void confirmComErr();
    void stopMonitorThread();

protected:
   void run();

private:
   bool isrun;
private slots:

signals:
    void stopCom(int com);
    void reStartCom(int com);
    void errorlog(int com);
    void noerrorlog(int com);
};

#endif // MONITORTHREAD_H
