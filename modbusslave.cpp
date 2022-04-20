#include "common.h"
#include <QStandardItemModel>
#include <QDebug>


ModbusSlave::ModbusSlave(QObject *parent) : QThread(parent)
{
    commanding = false;
    canRun = true;
    isrun = true;
}

void ModbusSlave::write(){

}

void ModbusSlave::read(int adds,int starAddr,int valuCount){
    comDEV[com].modbusUnit.adds = adds;
    comDEV[com].modbusUnit.writeUnit.setRegisterType(QModbusDataUnit::HoldingRegisters);
    comDEV[com].modbusUnit.writeUnit.setStartAddress(starAddr);
    comDEV[com].modbusUnit.writeUnit.setValueCount(valuCount);
    commanding = true;
    emit modbusRead(com);
}

void ModbusSlave::readEartag(int adds){
    read(adds,20,16);
}

void ModbusSlave::readState(int adds){
    read(adds,208,1);
}

void ModbusSlave::readEatValue(int adds){
    read(adds,50,3);
}

void ModbusSlave::writeFeed(int adds,float feedValue,int A,int B){
    comDEV[com].modbusUnit.adds = adds;
    comDEV[com].modbusUnit.writeUnit.setRegisterType(QModbusDataUnit::HoldingRegisters);
    comDEV[com].modbusUnit.writeUnit.setStartAddress(50);
    comDEV[com].modbusUnit.writeUnit.setValueCount(3);
    comDEV[com].modbusUnit.feedValue = feedValue;
    comDEV[com].modbusUnit.A = A;
    comDEV[com].modbusUnit.B = B;
    commanding = true;
    emit modbusWrite(com);
}

void ModbusSlave::run()
{
    qDebug() << "ModbusSlave::run com=" << com;
    while(isrun){
        msleep(1000);
            qDebug() <<  QThread::currentThreadId();
    qDebug() << "canRun " << canRun;
        while(canRun){
            msleep(30);

            for(int i = 0;i < comDEV[com].devCount;i++){
                readState(comDEV[com].deviceAddrS[i]);
//                qDebug() << "readState " << i << " wait " << waitComRease(20);
                waitComRease(20);
                readEartag(comDEV[com].deviceAddrS[i]);
//                qDebug() << "readEartag " << i << " wait " << waitComRease(20);
                waitComRease(20);
                readEatValue(comDEV[com].deviceAddrS[i]);
//                qDebug() << "readEatValue " << i << " wait " << waitComRease(20);
                waitComRease(20);
                writeFeed(comDEV[com].deviceAddrS[i],2341.123,49,51);
//                qDebug() << "writeFeed " << i << " wait " << waitComRease(20);
                waitComRease(20);
            }
        }

}
}

void ModbusSlave::readReady()
{
//    qDebug() << "ModbusSlave::readReady";
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        commanding = false;
        const QModbusDataUnit unit = reply->result();
        QString entry;
        qDebug() << "valueCount:" << unit.valueCount() << " startAddress" << unit.startAddress() << " serverAddress"
                 << reply->serverAddress() << "value:" << unit.value(0) <<  " " << unit.value(1);
        clrComErr(reply->serverAddress());
    } else {
        message(QString::number(reply->serverAddress()) + " Read error:" + reply->errorString());
        setComErr(reply->serverAddress());
    }

    reply->deleteLater();
}

void ModbusSlave::setComErr(int adds){
    if(infoStruct.deviceStruct[adds].comErrCount < 0xffff){
        infoStruct.deviceStruct[adds].comErrCount++;
    }

    qDebug() << "com err adds:" << adds << " count:" << infoStruct.deviceStruct[adds].comErrCount;


}

void ModbusSlave::clrComErr(int adds){
    infoStruct.deviceStruct[adds].comErrCount = 0;
}

bool ModbusSlave::waitComRease(int hundredMS){
    while(hundredMS){
        msleep(100);
        hundredMS--;
        if(commanding == false){
            return true;
        }
    }
    return false;
}

void ModbusSlave::stopModbusSlave(){
    isrun = false;
}











