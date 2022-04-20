#ifndef COMMON_H
#define COMMON_H

#include <QLabel>
#include <QSerialPort>
#include <QModbusDataUnit>
#include <QSerialPort>
#include <QModbusDevice>
#include <QModbusRtuSerialMaster>
#include "modbusslave.h"
#include "monitorthread.h"
#include <QModbusClient>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <qtredis.h>

class ModbusSlave;
class QNetworkReply;
class QNetworkAccessManager;
class QNetworkRequest;
enum DevState {
    waiting,
    pigWaiting,
    eating,
    dev_err = 100,
    Communi_err
};

enum ModbuState {
    uninit,
    read = 1,
    reading,
    readover,
    write = 11,
    writeing,
    writover,
    readerr = 100,
    writerr = 200,
};

struct DeviceStruct{
    bool    exist = false;
    QLabel *lable_dev;
    QLabel *lable_pig;
//    int     PLCAddr; //PLCAddr=deviceStruct[XX]
    int     com;
    DevState devState = DevState::waiting;
    int     comErrCount = 0;
};

struct InfoStruct{
    DeviceStruct    deviceStruct[51];   //deviceStruct[0] not use    first addr=1 second addr=2
    QString         addr = "A-2-1-1";
    int             deviceCount = 0;

    int             errordeviceCount = 0;

    int             pigCount = 0;
    int             pigOld = 80;
    float           curMonthEatT = 0;
    float           curWeekEatT = 0;
    float           curDayAllEatTimes = 0;
    float           curDayEatPigCount = 0;
    float           curDayEatT = 0;
    int             logClearMS = 4000;
    QString         url;
    QString         urlplclist;
    QString         urldetail;
    QString         urldisplay;
    QString         urlplan;
    QString         id;
};

struct ComSettings {
    int parity = QSerialPort::NoParity;
    int baud = QSerialPort::Baud9600;
    int dataBits = QSerialPort::Data8;
    int stopBits = QSerialPort::OneStop;
    int responseTime = 150;
    int numberOfRetries = 2;
};

struct ModbusUnit{
    int adds;    
    QModbusDataUnit writeUnit;
    int feedValue;
    int A;
    int B;
};

struct ComDEV{
    bool   exist = false;
    int    devCount = 0;
    QModbusClient *modbusClient;
    ModbusUnit modbusUnit;
    ModbusSlave *modbusSlave;
    ModbuState modbusStat = ModbuState::uninit;
    int deviceAddrS[8];
};

extern InfoStruct infoStruct;
extern ComSettings comSettings;
extern ComDEV comDEV[17];   //com0 not used!
extern QNetworkAccessManager *manager;
extern QNetworkRequest *request;
extern QtRedis *redis;
extern QNetworkReply *replyregister,*replyplclist,*replydetail,*replydisplay,*replyplan;
#endif // COMMON_H
