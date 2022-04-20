#include "monitorthread.h"
#include <QDebug>


MonitorThread::MonitorThread(QObject *parent) : QThread(parent)
{
    isrun = true;

}

void MonitorThread::run()
{
    qDebug() << "MonitorThread::run";
    while(isrun){
        msleep(3000);
    qDebug() <<  QThread::currentThreadId();
//        for(int i = 0 ;i < 50;i++){
//            if(infoStruct.deviceStruct[i].exist){

//            }
//        }

        confirmComErr();
    }
}

void MonitorThread::stopMonitorThread(){
    isrun=false;
}

void MonitorThread::confirmComErr(){
    for(int i = 1; i < 17; i++){
        if(comDEV[i].devCount > 0){
            if(comDevErr(i)){
                if(comDEV[i].modbusSlave->canRun){
                    comDEV[i].modbusSlave->canRun = false;
                    qDebug() << "comDEV " << i << "canRun = false";
                    sleep(10);
                }else{
                    emit errorlog(i);
                    //deal com err restart
                    emit stopCom(i);
                    sleep(3);
                    emit reStartCom(i);
                    sleep(3);
                }
            }  else{
                emit noerrorlog(i);
            }
                                }

               }
            }

bool MonitorThread::comDevErr(int com){
    for(int i = 0; i < comDEV[com].devCount; i++){
        if(infoStruct.deviceStruct[comDEV[com].deviceAddrS[i]].comErrCount < 10){
            return false;
        }
    }

    qDebug() << "-comDevErr com=com" << com;
    return true;
}
