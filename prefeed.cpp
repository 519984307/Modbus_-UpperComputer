#include "prefeed.h"
#include "ui_prefeed.h"
#include <QSettings>
#include "common.h"
#include <QDebug>
#include <QThread>
#include <QNetworkReply>


#include<qjsondocument.h>
#include<qjsonarray.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>
preFeed::preFeed(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::preFeed)
{
    init();
}

preFeed::~preFeed()
{
   // monitorThread.wait();
    delete ui;
}

void preFeed::on_quit_initset_clicked()
{
    mousecount++;
}

void preFeed::on_bset_clicked()
{
    if(ui->stackedWidget->currentIndex() == 0){
        ui->stackedWidget->setCurrentIndex(1);
    }else{
        ui->stackedWidget->setCurrentIndex(0);
    }

}

bool preFeed::importconfig(){
    QSettings configfile("C:/prefeed/prefeed.config", QSettings::IniFormat);
//    infoStruct.deviceCount = configfile.value("/COMMON/DEVICECOUNT").toInt();
    infoStruct.addr = configfile.value("/COMMON/ADDR").toString();
    infoStruct.pigOld = configfile.value("/COMMON/PIGOLD").toInt();
    infoStruct.pigCount = configfile.value("/COMMON/PIGCOUNT").toInt();
    infoStruct.curMonthEatT = configfile.value("/COMMON/CURMONTHEATT").toFloat();
    infoStruct.curWeekEatT = configfile.value("/COMMON/CURWEEKEATT").toFloat();
    infoStruct.curDayAllEatTimes = configfile.value("/COMMON/CURDAYALLEATTIMES").toInt();
    infoStruct.curDayEatPigCount = configfile.value("/COMMON/CURDAYEATPIGCOUNT").toInt();
    infoStruct.curDayEatT = configfile.value("/COMMON/CURDAYEATT").toFloat();
    infoStruct.url = configfile.value("/COMMON/URL").toString();
    infoStruct.urlplclist = configfile.value("/COMMON/URLPLCLIST").toString();
    infoStruct.urldetail = configfile.value("/COMMON/URLDETAIL").toString();
    infoStruct.urldisplay = configfile.value("/COMMON/URLDISPLAY").toString();
    infoStruct.urlplan = configfile.value("/COMMON/URLPLAN").toString();
    infoStruct.id = configfile.value("/COMMON/id").toString();

    for(int i = 1; i < 51 ;i++){
        int com = configfile.value("/DEVICE" + QString::number(i) + "/COM").toInt();
//        qDebug() << "import com=" << com;
        if(com > 0){
            if(com < 16){
                comDEV[com].devCount++;
                if(comDEV[com].devCount > 8){
                    ui->label_log->setText("Err same Com dev 8 Com=" + com);
                    return false;
                }else{
                    comDEV[com].deviceAddrS[comDEV[com].devCount - 1] = i;
                    if(comDEV[com].devCount == 1){
                        if(!initModbus(com)){
                            return false;
                        }
                        comDEV[com].exist = true;
                    }
                    infoStruct.deviceStruct[i].exist = true;
                    infoStruct.deviceStruct[i].com = com;
                    infoStruct.deviceCount++;
                }
            }else{
                ui->label_log->setText("Err Com max 16 now Com=" + com);
                return false;
            }
        }
    }

    qDebug() << "import config over!"<< QThread::currentThreadId();
    importSuccess = true;

    return true;
}

void preFeed::init()
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    systemTimer = startTimer(1000);
    logClearTimer.start(infoStruct.logClearMS);
    connect(&logClearTimer,SIGNAL(timeout()),this,SLOT(logClearTimeUpdate()));



    {
        infoStruct.deviceStruct[1].lable_dev = ui->label_dev1;
        infoStruct.deviceStruct[1].lable_pig = ui->label_pig1;
        infoStruct.deviceStruct[2].lable_dev = ui->label_dev2;
        infoStruct.deviceStruct[2].lable_pig = ui->label_pig2;
        infoStruct.deviceStruct[3].lable_dev = ui->label_dev3;
        infoStruct.deviceStruct[3].lable_pig = ui->label_pig3;
        infoStruct.deviceStruct[4].lable_dev = ui->label_dev4;
        infoStruct.deviceStruct[4].lable_pig = ui->label_pig4;
        infoStruct.deviceStruct[5].lable_dev = ui->label_dev5;
        infoStruct.deviceStruct[5].lable_pig = ui->label_pig5;
        infoStruct.deviceStruct[6].lable_dev = ui->label_dev6;
        infoStruct.deviceStruct[6].lable_pig = ui->label_pig6;
        infoStruct.deviceStruct[7].lable_dev = ui->label_dev7;
        infoStruct.deviceStruct[7].lable_pig = ui->label_pig7;
        infoStruct.deviceStruct[8].lable_dev = ui->label_dev8;
        infoStruct.deviceStruct[8].lable_pig = ui->label_pig8;
        infoStruct.deviceStruct[9].lable_dev = ui->label_dev9;
        infoStruct.deviceStruct[9].lable_pig = ui->label_pig9;
        infoStruct.deviceStruct[10].lable_dev = ui->label_dev10;
        infoStruct.deviceStruct[10].lable_pig = ui->label_pig10;
        infoStruct.deviceStruct[11].lable_dev = ui->label_dev11;
        infoStruct.deviceStruct[11].lable_pig = ui->label_pig11;
        infoStruct.deviceStruct[12].lable_dev = ui->label_dev12;
        infoStruct.deviceStruct[12].lable_pig = ui->label_pig12;
        infoStruct.deviceStruct[13].lable_dev = ui->label_dev13;
        infoStruct.deviceStruct[13].lable_pig = ui->label_pig13;
        infoStruct.deviceStruct[14].lable_dev = ui->label_dev14;
        infoStruct.deviceStruct[14].lable_pig = ui->label_pig14;
        infoStruct.deviceStruct[15].lable_dev = ui->label_dev15;
        infoStruct.deviceStruct[15].lable_pig = ui->label_pig15;
        infoStruct.deviceStruct[16].lable_dev = ui->label_dev16;
        infoStruct.deviceStruct[16].lable_pig = ui->label_pig16;
        infoStruct.deviceStruct[17].lable_dev = ui->label_dev17;
        infoStruct.deviceStruct[17].lable_pig = ui->label_pig17;
        infoStruct.deviceStruct[18].lable_dev = ui->label_dev18;
        infoStruct.deviceStruct[18].lable_pig = ui->label_pig18;
        infoStruct.deviceStruct[19].lable_dev = ui->label_dev19;
        infoStruct.deviceStruct[19].lable_pig = ui->label_pig19;
        infoStruct.deviceStruct[20].lable_dev = ui->label_dev20;
        infoStruct.deviceStruct[20].lable_pig = ui->label_pig20;
        infoStruct.deviceStruct[21].lable_dev = ui->label_dev21;
        infoStruct.deviceStruct[21].lable_pig = ui->label_pig21;
        infoStruct.deviceStruct[22].lable_dev = ui->label_dev22;
        infoStruct.deviceStruct[22].lable_pig = ui->label_pig22;
        infoStruct.deviceStruct[23].lable_dev = ui->label_dev23;
        infoStruct.deviceStruct[23].lable_pig = ui->label_pig23;
        infoStruct.deviceStruct[24].lable_dev = ui->label_dev24;
        infoStruct.deviceStruct[24].lable_pig = ui->label_pig24;
        infoStruct.deviceStruct[25].lable_dev = ui->label_dev25;
        infoStruct.deviceStruct[25].lable_pig = ui->label_pig25;
        infoStruct.deviceStruct[26].lable_dev = ui->label_dev26;
        infoStruct.deviceStruct[26].lable_pig = ui->label_pig26;
        infoStruct.deviceStruct[27].lable_dev = ui->label_dev27;
        infoStruct.deviceStruct[27].lable_pig = ui->label_pig27;
        infoStruct.deviceStruct[28].lable_dev = ui->label_dev28;
        infoStruct.deviceStruct[28].lable_pig = ui->label_pig28;
        infoStruct.deviceStruct[29].lable_dev = ui->label_dev29;
        infoStruct.deviceStruct[29].lable_pig = ui->label_pig29;
        infoStruct.deviceStruct[30].lable_dev = ui->label_dev30;
        infoStruct.deviceStruct[30].lable_pig = ui->label_pig30;
        infoStruct.deviceStruct[31].lable_dev = ui->label_dev31;
        infoStruct.deviceStruct[31].lable_pig = ui->label_pig31;
        infoStruct.deviceStruct[32].lable_dev = ui->label_dev32;
        infoStruct.deviceStruct[32].lable_pig = ui->label_pig32;
        infoStruct.deviceStruct[33].lable_dev = ui->label_dev33;
        infoStruct.deviceStruct[33].lable_pig = ui->label_pig33;
        infoStruct.deviceStruct[34].lable_dev = ui->label_dev34;
        infoStruct.deviceStruct[34].lable_pig = ui->label_pig34;
        infoStruct.deviceStruct[35].lable_dev = ui->label_dev35;
        infoStruct.deviceStruct[35].lable_pig = ui->label_pig35;
        infoStruct.deviceStruct[36].lable_dev = ui->label_dev36;
        infoStruct.deviceStruct[36].lable_pig = ui->label_pig36;
        infoStruct.deviceStruct[37].lable_dev = ui->label_dev37;
        infoStruct.deviceStruct[37].lable_pig = ui->label_pig37;
        infoStruct.deviceStruct[38].lable_dev = ui->label_dev38;
        infoStruct.deviceStruct[38].lable_pig = ui->label_pig38;
        infoStruct.deviceStruct[39].lable_dev = ui->label_dev39;
        infoStruct.deviceStruct[39].lable_pig = ui->label_pig39;
        infoStruct.deviceStruct[40].lable_dev = ui->label_dev40;
        infoStruct.deviceStruct[40].lable_pig = ui->label_pig40;
        infoStruct.deviceStruct[41].lable_dev = ui->label_dev41;
        infoStruct.deviceStruct[41].lable_pig = ui->label_pig41;
        infoStruct.deviceStruct[42].lable_dev = ui->label_dev42;
        infoStruct.deviceStruct[42].lable_pig = ui->label_pig42;
        infoStruct.deviceStruct[43].lable_dev = ui->label_dev43;
        infoStruct.deviceStruct[43].lable_pig = ui->label_pig43;
        infoStruct.deviceStruct[44].lable_dev = ui->label_dev44;
        infoStruct.deviceStruct[44].lable_pig = ui->label_pig44;
        infoStruct.deviceStruct[45].lable_dev = ui->label_dev45;
        infoStruct.deviceStruct[45].lable_pig = ui->label_pig45;
        infoStruct.deviceStruct[46].lable_dev = ui->label_dev46;
        infoStruct.deviceStruct[46].lable_pig = ui->label_pig46;
        infoStruct.deviceStruct[47].lable_dev = ui->label_dev47;
        infoStruct.deviceStruct[47].lable_pig = ui->label_pig47;
        infoStruct.deviceStruct[48].lable_dev = ui->label_dev48;
        infoStruct.deviceStruct[48].lable_pig = ui->label_pig48;
        infoStruct.deviceStruct[49].lable_dev = ui->label_dev49;
        infoStruct.deviceStruct[49].lable_pig = ui->label_pig49;
        infoStruct.deviceStruct[50].lable_dev = ui->label_dev50;
        infoStruct.deviceStruct[50].lable_pig = ui->label_pig50;
    }

    if(!importconfig()){
        ui->label_log->setText(ui->label_log->text() + " soft stoped!");
        return ;
    }


    ui->lineEdit_id->setText(infoStruct.id);
    ui->lineEdit_url->setText(infoStruct.url);

    for(int i = 1;i < 51;i++){
        if(infoStruct.deviceStruct[i].exist){
            infoStruct.deviceStruct[i].lable_dev->setVisible(true);
            infoStruct.deviceStruct[i].lable_pig->setVisible(true);
        }else{
            infoStruct.deviceStruct[i].lable_dev->setVisible(false);
            infoStruct.deviceStruct[i].lable_pig->setVisible(false);
        }
    }

    ui->label_addr->setText(infoStruct.addr);
    ui->label_count_old->setText(QString::number(infoStruct.pigOld) + "d/" + QString::number(infoStruct.pigCount));
    ui->label_mountheat->setText(QString::number(infoStruct.curMonthEatT,'f', 2));
    ui->label_weekeat->setText(QString::number(infoStruct.curWeekEatT,'f', 2));
    ui->label_todayeatcount->setText(QString::number(infoStruct.curDayAllEatTimes));
    ui->label_todayeatpigcount->setText(QString::number(infoStruct.curDayEatPigCount));
    ui->label_todayeat->setText(QString::number(infoStruct.curDayEatT,'f', 2));
    ui->label_eatcount->setText("(0/" + QString::number(infoStruct.deviceCount) + ")");
    ui->label_errcount->setText("(0)");
    ui->label_nullcount->setText("(" + QString::number(infoStruct.deviceCount) + ")");


    for(int i = 1;i< 17;i++){
        if(comDEV[i].exist){
            comDEV[i].modbusSlave->start();
        }
    }

    manager = new QNetworkAccessManager();
    request = new QNetworkRequest();

    connectRedis();
    connect(&monitorThread,SIGNAL(errorlog(int)),this,SLOT(error(int)));
    connect(&monitorThread,SIGNAL(noerrorlog(int)),this,SLOT(noerror(int)));
    connect(&monitorThread, SIGNAL(stopCom(int)),this, SLOT(stopCom(int)));
    connect(&monitorThread, SIGNAL(reStartCom(int)),this, SLOT(reStartCom(int)));
    monitorThread.start();
}

void preFeed::timerEvent(QTimerEvent *event){
    if(event->timerId() == systemTimer){
       //qDebug() << "mousePressEvent:" << mousecount<<"clickFlag:"<<clickFlag;
        dt.setTime(time.currentTime());
        dt.setDate(date.currentDate());
        ui->label_time->setText(dt.toString("yyyy-MM-dd hh:mm:ss"));

        for(int i = 1;i < 51;i++){
            if(infoStruct.deviceStruct[i].exist){
             //   if(infoStruct.deviceStruct[i].devState == waiting){
              //      infoStruct.deviceStruct[i].lable_pig->setStyleSheet("outline: none;image:url(\"C:/prefeed/img/noeat.png\");");
              //  }
                switch(infoStruct.deviceStruct[i].devState){
                case(waiting):infoStruct.deviceStruct[i].lable_pig->setStyleSheet("outline: none;image:url(\"C:/prefeed/img/noeat.png\");");break;
                case(dev_err):infoStruct.deviceStruct[i].lable_pig->setStyleSheet("outline: none;image:url(\"C:/prefeed/img/eaterr.png\");");break;
                default:;
                }
            }
        }

       {ui->label_addr->setText(infoStruct.addr);
        ui->label_count_old->setText(QString::number(infoStruct.pigOld) + "d/" + QString::number(infoStruct.pigCount));
        ui->label_mountheat->setText(QString::number(infoStruct.curMonthEatT,'f', 2));
        ui->label_weekeat->setText(QString::number(infoStruct.curWeekEatT,'f', 2));
        ui->label_todayeatcount->setText(QString::number(infoStruct.curDayAllEatTimes));
        ui->label_todayeatpigcount->setText(QString::number(infoStruct.curDayEatPigCount));
        ui->label_todayeat->setText(QString::number(infoStruct.curDayEatT,'f', 2));
        ui->label_eatcount->setText("(0/" + QString::number(infoStruct.deviceCount) + ")");
        ui->label_errcount->setText("("+QString::number(infoStruct.errordeviceCount)+ ")");
        ui->label_nullcount->setText("(" + QString::number(infoStruct.deviceCount) + ")");

        }


        if(clickFlag == false&&mousecount > 0){
           clickFlag = true;
         if(clickFlag == true) {            
                 if(mousecount >= 3)
                 {
                  safeQuit();
                }
         }
      }
    }
    clickFlag = false;
      mousecount = 0;
}

void preFeed::logClearTimeUpdate(){
    ui->label_log->clear();
}

void preFeed::message(QString msg){
    logClearTimer.stop();
    ui->label_log->setText(msg);
    logClearTimer.start(infoStruct.logClearMS);
}

bool preFeed::initModbus(int com){
    QString sCom = "com" + QString::number(com);
    comDEV[com].modbusClient = new QModbusRtuSerialMaster(this);
    connect(comDEV[com].modbusClient, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        ui->label_log->setText(QString("Err init com"));
    });

    ComSettings comSettings;
    comDEV[com].modbusClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter,sCom);
    comDEV[com].modbusClient->setConnectionParameter(QModbusDevice::SerialParityParameter,comSettings.parity);
    comDEV[com].modbusClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,comSettings.baud);
    comDEV[com].modbusClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,comSettings.dataBits);
    comDEV[com].modbusClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,comSettings.stopBits);
//    comDEV[com].modbusClient->setConnectionParameter(QModbusDevice::se,comSettings.stopBits);

//    QSerialPort::NoFlowControl
    comDEV[com].modbusClient->setTimeout(comSettings.responseTime);
    comDEV[com].modbusClient->setNumberOfRetries(comSettings.numberOfRetries);
    if (!comDEV[com].modbusClient->connectDevice()) {
        ui->label_log->setText("Err com connect " + sCom + " " + comDEV[com].modbusClient->errorString());
        return false;
    }

    connect(comDEV[com].modbusClient, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        qDebug() << "QModbusClient::errorOccurred " << comDEV[4].modbusClient->errorString();
    });

    comDEV[com].modbusSlave = new ModbusSlave();
    comDEV[com].modbusSlave->com = com;
    connect(comDEV[com].modbusSlave, SIGNAL(message(QString)),this, SLOT(message(QString)));
    connect(comDEV[com].modbusSlave, SIGNAL(modbusRead(int)),this, SLOT(modbusRead(int)));
    connect(comDEV[com].modbusSlave, SIGNAL(modbusWrite(int)),this, SLOT(modbusWrite(int)));

    return true;
}

void preFeed::modbusRead(int com){
    if (auto *reply = comDEV[com].modbusClient->sendReadRequest(comDEV[com].modbusUnit.writeUnit, comDEV[com].modbusUnit.adds)) {
        if (!reply->isFinished()){
            connect(reply, SIGNAL(finished()), comDEV[com].modbusSlave, SLOT(readReady()));
        }else{
            delete reply; // broadcast replies return immediately
        }
    } else {
        message(QString::number(comDEV[com].modbusUnit.adds) + " read err! "  + comDEV[com].modbusClient->errorString());
        qDebug() << "rrrrrrrrrrrrrrrrr " << comDEV[com].modbusClient->errorString();
        comDEV[com].modbusSlave->setComErr(comDEV[com].modbusUnit.adds);
    }
}


void preFeed::modbusWrite(int com){
    QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters,comDEV[com].modbusUnit.writeUnit.startAddress(),3);
    writeUnit.setValue(0, comDEV[com].modbusUnit.feedValue);
    writeUnit.setValue(1, comDEV[com].modbusUnit.A);
    writeUnit.setValue(2, comDEV[com].modbusUnit.B);

    if (auto *reply = comDEV[com].modbusClient->sendWriteRequest(writeUnit, comDEV[com].modbusUnit.adds)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if(reply->error() != QModbusDevice::NoError){
                    message(QString::number(reply->serverAddress()) + " write err---!");
                }else{
                    comDEV[infoStruct.deviceStruct[reply->serverAddress()].com].modbusSlave->commanding = false;
                }

                reply->deleteLater();
            });
        } else {
            reply->deleteLater();
        }
    } else {
        message(QString::number(comDEV[com].modbusUnit.adds) + " write err! "  + comDEV[com].modbusClient->errorString());
        qDebug() << "wwwwwwwwwwwwwwwwww " << comDEV[com].modbusClient->errorString();
        comDEV[com].modbusSlave->setComErr(comDEV[com].modbusUnit.adds);
    }
}


void preFeed::on_breturn_initset_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void preFeed::on_initialize_initset_clicked(){
    ui->stackedWidget_2->setCurrentIndex(0);
}

void preFeed::on_bsynclow_initset_clicked(){
    ui->stackedWidget_2->setCurrentIndex(1);
}

void preFeed::on_localCach_initset_clicked(){
    ui->stackedWidget_2->setCurrentIndex(2);

}
void preFeed::on_bregister_clicked()
{   
            QSettings configfile("C:/prefeed/prefeed.config", QSettings::IniFormat);
            infoStruct.url = ui->lineEdit_url->text();
            infoStruct.id = ui->lineEdit_id->text();
         //   configfile.setValue("/COMMON/URL",infoStruct.url);
         //   configfile.setValue("/COMMON/ID",infoStruct.id);
            //参数设定
             if(importSuccess == true){
            QString parameter = getRegisterParameter();
            qDebug()<<parameter;
            httpRequest(infoStruct.url,parameter);
            message("register over!!");
             }else{QString failmessage = "NO URL--IMPORTCONFIG FAILED";
                 message(failmessage);}


}

void preFeed::on_syn_initset_clicked(){
    if(importSuccess == true){
           QString parameter = getPlclistParameter();
           qDebug()<<parameter;
           httpRequest(infoStruct.urlplclist,parameter);

    }
    else{QString failmessage = "NO URL--IMPORTCONFIG FAILED";
        message(failmessage);}
}

void preFeed::on_display_initset_clicked(){
     QString parameter = getDisplayParameter();
     httpRequest(infoStruct.urldisplay,parameter);
}

void preFeed::on_detail_initset_clicked(){
     QString parameter = getDetailParameter();
     httpRequest(infoStruct.urldetail,parameter);
}

void preFeed::on_plan_initset_clicked(){
     QString parameter = getPlanParameter();
     httpRequest(infoStruct.urlplan,parameter);
}

void preFeed::on_redis_initset_clicked(){

}

void preFeed::registerFinished()
{  // qDebug() << "plclistFinished run";
    if(replyregister->error() == QNetworkReply::NoError) {
    QByteArray all = replyregister->readAll();
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(all,&jsonError);
  if(!document.isNull()&&(jsonError.error == QJsonParseError::NoError)){
   if (document.isObject()){
      QJsonObject obj = document.object();
      if(obj.contains("msg"))
      {   QString  msg = obj.value("msg").toString();
          qDebug() << "msg:"<<msg;
      }    
      if(obj.contains("code"))
      {  QString  code = obj.value("code").toString();
          qDebug() << "code:"<<code;
      }
     }
    }
      replyregister->deleteLater();

  }
     else{  qDebug() <<"REPLY ERROR:"<< replyregister->errorString();}


}

void preFeed::displayFinished(){
    if(replydisplay->error() == QNetworkReply::NoError) {
    QByteArray all = replydisplay->readAll();
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(all,&jsonError);
      if(!document.isNull()&&(jsonError.error == QJsonParseError::NoError)){
       if (document.isObject()){
          QJsonObject obj = document.object();
          if(obj.contains("msg")) {   QString  msg = obj.value("msg").toString();
              qDebug() << "msg:"<<msg;
                 }
          if(obj.contains("code")){
            QString  code = obj.value("code").toString();
              qDebug() << "code:"<<code;
                 }
          if(obj.contains("data")){
              QJsonValue data = obj.value("data");
              if (data.isObject()){
                  QJsonObject object = data.toObject();
                  if(object.contains("ageDay")){
                       int ageDay = object.value("ageDay").toInt();
                       qDebug()<<"ageDay："<<ageDay;
                       infoStruct.pigOld = ageDay;
                  }
                  if(object.contains("batchCount")){
                       int batchCount = object.value("batchCount").toInt();
                        qDebug()<<"batchCount："<<batchCount;
                       infoStruct.pigCount = batchCount;
                  }
                  if(object.contains("dayFeedPigQty")){
                      float dayFeedPigQty = object.value("dayFeedPigQty").toDouble();
                       qDebug()<<"dayFeedPigQty："<<dayFeedPigQty;
                      infoStruct.curDayEatT = dayFeedPigQty;
                  }
                  if(object.contains("dayFeedPigTimes")){
                      float dayFeedPigTimes = object.value("dayFeedPigTimes").toDouble();
                       qDebug()<<"dayFeedPigTimes："<<dayFeedPigTimes;
                      infoStruct.curDayAllEatTimes = dayFeedPigTimes;
                  }
                  if(object.contains("dayFeedQty")){
                      float dayFeedQty = object.value("dayFeedQty").toDouble();
                       qDebug()<<"dayFeedQty："<<dayFeedQty;
                      infoStruct.curDayEatT = dayFeedQty;
                  }
                  if(object.contains("monthFedQty")){
                      float monthFedQty = object.value("monthFedQty").toDouble();
                       qDebug()<<"monthFedQty："<<monthFedQty;
                      infoStruct.curMonthEatT = monthFedQty;
                  }
                  if(object.contains("weekFedQty")){
                      float weekFedQty = object.value("weekFedQty").toDouble();
                       qDebug()<<"weekFedQty："<<weekFedQty;
                      infoStruct.curWeekEatT = weekFedQty;
                  }
                     qDebug()<<"Load display info success";
              }
          }
          }
         }
      replydisplay->deleteLater();
        }else{  qDebug() <<"REPLY ERROR:"<< replydisplay->errorString();
       }
    }

void preFeed::plclistFinished(){
      // qDebug() << "plclistFinished run";
   if(replyplclist->error() == QNetworkReply::NoError) {
        QByteArray all = replyplclist->readAll();
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(all,&jsonError);
      if(!document.isNull()&&(jsonError.error == QJsonParseError::NoError)){
       if (document.isObject()){
          QJsonObject obj = document.object();
          if(obj.contains("msg"))
          {   QString  msg = obj.value("msg").toString();
              qDebug() << "msg:"<<msg;
          }
          if(obj.contains("data"))
          {   QJsonValue  data = obj.value("data");
              if(redisSuccess != true){
                                  message("错误！请确认redis服务已开启");
                                  qDebug()<<"错误！请确认redis服务已开启";
                                 }
               else{
                  if(data.isArray()&&!data.isNull()){
                  QJsonArray array = data.toArray();
                  for(int i=0;i<array.count();i++){
                    QJsonValue plcdata = array[i];
                    if(plcdata.isObject())
                    {
                    int baudRate;
                    QString comPort;
                    int plcAdd;
                    QJsonObject  plcObject = plcdata.toObject();
                    QString key ="plc"+QString::number(i+1);
                    if(plcObject.contains("baudRate")){
                        baudRate = plcObject.value("baudRate").toInt();
                        redis->hset(key,"baudRate" , QString::number(baudRate));
                        }
                    if(plcObject.contains("comPort")){
                        comPort = plcObject.value("comPort").toString();
                        redis->hset(key,"comPort" ,comPort);
                        }
                    if(plcObject.contains("plcAdd")){
                        plcAdd = plcObject.value("plcAdd").toInt();
                        redis->hset(key,"plcAdd" , QString::number(plcAdd));
                        }




                    }
                }
               qDebug()<<"写入redis成功";

            }
           }

        }

        if(obj.contains("code"))
           {  QString  code = obj.value("code").toString();
              qDebug() << "code:"<<code;}

            }
      }
          replyplclist->deleteLater();
        }

         else{  qDebug() <<"REPLY ERROR:"<< replyplclist->errorString();}

    }

void preFeed::detailFinished()
{  // qDebug() << "plclistFinished run";
    if(replydetail->error() == QNetworkReply::NoError) {
    QByteArray all = replydetail->readAll();
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(all,&jsonError);
  if(!document.isNull()&&(jsonError.error == QJsonParseError::NoError)){
   if (document.isObject()){
      QJsonObject obj = document.object();
      if(obj.contains("msg"))
      {   QString  msg = obj.value("msg").toString();
          qDebug() << "msg:"<<msg;
      }
      if(obj.contains("data"))
      {   QJsonValue  data = obj.value("data");
        if (data.isArray()&&!data.isNull()){
            QJsonArray array = data.toArray();
            for(int i=0;i<array.count();i++)
            {
                QJsonValue plcdata = array[i];
                if(plcdata.isObject())
                {
                    int baudRate;
                    QString comPort;
                    int plcAdd;
                    QJsonObject  plcObject = plcdata.toObject();
                    QString key ="plc"+QString::number(i+1);
                    if(redisSuccess != true){
                     message("错误！请确认redis服务已开启");
                     qDebug()<<"错误！请确认redis服务已开启";
                    }
                    else{
                        if(plcObject.contains("baudRate")){
                            baudRate = plcObject.value("baudRate").toInt();
                            redis->hset(key,"baudRate" , QString::number(baudRate));
                            }
                        if(plcObject.contains("comPort")){
                            comPort = plcObject.value("comPort").toString();
                            redis->hset(key,"comPort" ,comPort);
                            }
                        if(plcObject.contains("plcAdd")){
                            plcAdd = plcObject.value("plcAdd").toInt();
                            redis->hset(key,"plcAdd" , QString::number(plcAdd));
                            }
                      //qDebug()<<baudRate<<" "<<comPort<<" "<<plcAdd;
                       qDebug()<<"写入redis成功";

                    }
                }
            }
        }

    }

//    if(obj.contains("error_code"))
//        //QJsonValue error_code_value = obj.take("error_code_value");
//       {  QString  error_code = obj.value("error_code").toString();
//          qDebug() << error_code;}

      }
  }
      replydetail->deleteLater();
    }

     else{  qDebug() <<"REPLY ERROR:"<< replydetail->errorString();}

}

void preFeed::planFinished()
{  // qDebug() << "plclistFinished run";
    if(replyplan->error() == QNetworkReply::NoError) {
    QByteArray all = replyplan->readAll();
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(all,&jsonError);
  if(!document.isNull()&&(jsonError.error == QJsonParseError::NoError)){
   if (document.isObject()){
      QJsonObject obj = document.object();
      if(obj.contains("msg"))
      {   QString  msg = obj.value("msg").toString();
          qDebug() << "msg:"<<msg;
      }
      if(obj.contains("data"))
      {   QJsonValue  data = obj.value("data");
          if(data.isObject()){
             QJsonObject object = data.toObject();
             if(object.contains("aratio")){
                 int aratio = object.value("aratio").toInt();
                 qDebug() << "aratio:"<<aratio;
             }
             if(object.contains("bratio")){
                 int bratio = object.value("bratio").toInt();
                  qDebug() << "bratio:"<<bratio;
             }
             if(object.contains("fedQuantity")){
                 int fedQuantity = object.value("fedQuantity").toInt();
                 qDebug() << "fedQuantity:"<<fedQuantity;
             }
             if(object.contains("feedDate")){
                 QString feedDate = object.value("feedDate").toString();
                 qDebug() << "feedDate:"<<feedDate;
             }
             if(object.contains("quantity")){
                 int quantity = object.value("quantity").toInt();
                 qDebug() << "quantity:"<<quantity;
             }

          }

    }
    if(obj.contains("code"))
       {  QString  code = obj.value("code").toString();
          qDebug() << code;}

      }
  }
      replyplan->deleteLater();
    }

     else{  qDebug() <<"REPLY ERROR:"<< replyplan->errorString();}

}

void preFeed::httpRequest(QString url,QString requestInfo){
    request->setUrl(url);
    request->setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    QByteArray sendbyterarray =requestInfo.toLatin1();
    QStringList stringList;
    stringList <<infoStruct.url << infoStruct.urlplclist<<infoStruct.urldetail<<infoStruct.urldisplay<<infoStruct.urlplan;
    switch(stringList.indexOf(request->url().toString())){
    case(0):replyregister = manager->post(*request,sendbyterarray);connect(replyregister, SIGNAL(finished()),this, SLOT(registerFinished())); break;
    case(1):replyplclist = manager->post(*request,sendbyterarray);connect(replyplclist, SIGNAL(finished()),this, SLOT(plclistFinished()));break;
    case(2):replydetail = manager->post(*request,sendbyterarray);connect(replydetail, SIGNAL(finished()),this, SLOT(detailFinished()));break;
    case(3):replydisplay = manager->post(*request,sendbyterarray);connect(replydisplay, SIGNAL(finished()),this, SLOT(displayFinished()));break;
    case(4):replyplan = manager->post(*request,sendbyterarray);connect(replyplan, SIGNAL(finished()),this, SLOT(planFinished()));break;
    }
}


void preFeed::stopCom(int com){
    qDebug() << "stop com:" << com;
    comDEV[com].modbusClient->disconnectDevice();
}

void preFeed::reStartCom(int com){
    if (!comDEV[com].modbusClient->connectDevice()) {
        qDebug() << "reStartCom Err com reconnect " << com << " " << comDEV[com].modbusClient->errorString();
    }else{
        comDEV[com].modbusSlave->canRun = true;

        qDebug() << "reStartCom com ok canRun=true" << com;
    }
}

void preFeed::error(int com){
    QString scom = "Com "+QString::number(com) + " Something Wrong ";
    message(scom);
    for(int i = 0;i < 51;i++){
        if(infoStruct.deviceStruct[i].exist){
        if(infoStruct.deviceStruct[i].com == com && infoStruct.deviceStruct[i].devState != dev_err){
            infoStruct.deviceStruct[i].devState = dev_err;
            infoStruct.errordeviceCount++;
            infoStruct.deviceCount--;
           }
       }
   }
}

void preFeed::noerror(int com){
  //  QString scom = "Com "+QString::number(com) + " Nothing Wrong";
   // message(scom);
    for(int i = 0;i < 51;i++){
        if(infoStruct.deviceStruct[i].exist){
        if(infoStruct.deviceStruct[i].com == com && infoStruct.deviceStruct[i].devState != waiting){
    infoStruct.deviceStruct[i].devState = waiting;
    infoStruct.deviceCount++;
    infoStruct.errordeviceCount--;
           }
        }
   }
}

QString preFeed::getRegisterParameter(){
   QString apikey =  "1acc43f4-6efd-4ac8-9dd9-59a9b19879f5";
   //QDateTime current_date_time = QDateTime::currentDateTime();
   QString time = dt.toString("yyyy-MM-dd hh:mm::ss.zzz");
   //QString timestamp = "1594950855";
   QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch() / 1000);

    QString bizData = "{'companyId': 'string','farmId': 'string','houseId': 'string','mac':'1111','mcCode': 'e27c924e-c578-11ea-9821-00163e044e5a','penId':'string','unitId':'string'}";
    QString key = "precise_feed";
    QByteArray byteArray;
    QString text = apikey.append(timestamp).append(bizData).append(key);
     byteArray.append(text);
    QString md5 = QCryptographicHash::hash(byteArray, QCryptographicHash::Md5).toHex();
    QString L1="{'apikey':'1acc43f4-6efd-4ac8-9dd9-59a9b19879f5',";
    QString para = L1.append("'timestamp':").append(timestamp).append(",")
.append("'sign':").append("'" + md5 + "'").append(",")
.append("'data':").append("\""+bizData+"\"").append("}");
return para;

}

QString preFeed::getPlclistParameter(){
   QString apikey =  "1acc43f4-6efd-4ac8-9dd9-59a9b19879f5";
  // QDateTime current_date_time = QDateTime::currentDateTime();
   QString time = dt.toString("yyyy-MM-dd hh:mm::ss.zzz");
   //QString timestamp = "1594950855";
   QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch() / 1000);

    QString bizData = "{'mcCode': 'e27c924e-c578-11ea-9821-00163e044e5a','mac': '1111112222233333','penId': '1'}";
    QString key = "precise_feed";
    QByteArray byteArray;
    QString text = apikey.append(timestamp).append(bizData).append(key);
    byteArray.append(text);
    QString md5 = QCryptographicHash::hash(byteArray, QCryptographicHash::Md5).toHex();
    QString L1="{'apikey':'1acc43f4-6efd-4ac8-9dd9-59a9b19879f5',";
    QString para = L1.append("'timestamp':").append(timestamp).append(",")
.append("'sign':").append("'" + md5 + "'").append(",")
.append("'data':").append("\""+bizData+"\"").append("}");
return para;
//qDebug()<<request;
}

QString preFeed::getDetailParameter(){
   QString apikey =  "1acc43f4-6efd-4ac8-9dd9-59a9b19879f5";
  // QDateTime current_date_time = QDateTime::currentDateTime();
   QString time = dt.toString("yyyy-MM-dd hh:mm::ss.zzz");
   //QString timestamp = "1594950855";
   QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch() / 1000);

    QString bizData = "[{'electronicEarnock': 'e27c924e-c578-11ea-9821-00163e044e5a','beginTime':'2020-08-22 16:20:20','masterId':'1','slaveId':'1','quantity':100,'aQuantity':60,'bQuantity':40,'endTime':'2020-08-22 16:20:20'},"
                      "{'electronicEarnock': 'e27c924e-c578-11ea-9821-00163e044e5a','beginTime':'2020-08-22 16:20:20','masterId':'1','slaveId':'1','quantity':100,'aQuantity':60,'bQuantity':40,'endTime':'2020-08-22 16:20:20'},"
                      "{'electronicEarnock': 'e27c924e-c578-11ea-9821-00163e044e5a','beginTime':'2020-08-22 16:20:20','masterId':'1','slaveId':'1','quantity':100,'aQuantity':60,'bQuantity':40,'endTime':'2020-08-22 16:20:20'},"
                      "{'electronicEarnock': 'e27c924e-c578-11ea-9821-00163e044e5a','beginTime':'2020-08-22 16:20:20','masterId':'1','slaveId':'1','quantity':100,'aQuantity':60,'bQuantity':40,'endTime':'2020-08-22 16:20:20'}]";
    QString key = "precise_feed";
    QByteArray byteArray;
    QString text = apikey.append(timestamp).append(bizData).append(key);
    byteArray.append(text);
    QString md5 = QCryptographicHash::hash(byteArray, QCryptographicHash::Md5).toHex();
    QString L1="{'apikey':'1acc43f4-6efd-4ac8-9dd9-59a9b19879f5',";
    QString para = L1.append("'timestamp':").append(timestamp).append(",")
.append("'sign':").append("'" + md5 + "'").append(",")
.append("'data':").append("\""+bizData+"\"").append("}");

    return para;
//qDebug()<<request;
}

QString preFeed::getDisplayParameter(){
   QString apikey =  "1acc43f4-6efd-4ac8-9dd9-59a9b19879f5";
  // QDateTime current_date_time = QDateTime::currentDateTime();
   QString time = dt.toString("yyyy-MM-dd hh:mm::ss.zzz");
   //QString timestamp = "1594950855";
   QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch() / 1000);

    QString bizData = "{'masterId': 'e27c924e-c578-11ea-9821-00163e044e5a','penId':'1'}";
    QString key = "precise_feed";
    QByteArray byteArray;
    QString text = apikey.append(timestamp).append(bizData).append(key);
    byteArray.append(text);
    QString md5 = QCryptographicHash::hash(byteArray, QCryptographicHash::Md5).toHex();
    QString L1="{'apikey':'1acc43f4-6efd-4ac8-9dd9-59a9b19879f5',";
    QString para = L1.append("'timestamp':").append(timestamp).append(",")
.append("'sign':").append("'" + md5 + "'").append(",")
.append("'data':").append("\""+bizData+"\"").append("}");

return para;
//qDebug()<<request;
}

QString preFeed::getPlanParameter(){
   QString apikey =  "1acc43f4-6efd-4ac8-9dd9-59a9b19879f5";
  // QDateTime current_date_time = QDateTime::currentDateTime();
   QString time = dt.toString("yyyy-MM-dd hh:mm::ss.zzz");
   //QString timestamp = "1594950855";
   QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch() / 1000);

    QString bizData = "{'electronicEarnock': 'e27c924e-c578-11ea-9821-00163e044e5a','penId':'1'}";
    QString key = "precise_feed";
    QByteArray byteArray;
    QString text = apikey.append(timestamp).append(bizData).append(key);
    byteArray.append(text);
    QString md5 = QCryptographicHash::hash(byteArray, QCryptographicHash::Md5).toHex();
    QString L1="{'apikey':'1acc43f4-6efd-4ac8-9dd9-59a9b19879f5',";
    QString para = L1.append("'timestamp':").append(timestamp).append(",")
.append("'sign':").append("'" + md5 + "'").append(",")
.append("'data':").append("\""+bizData+"\"").append("}");

return para;
//qDebug()<<request;
}

void preFeed::connectRedis()
{
    redis = new QtRedis("localhost",6379);
 //   redis->start();
//    connect(redis, SIGNAL(returnData(QtRedis::Reply)), this, SLOT(slotMessage(QtRedis::Reply)));
    QtRedis::Reply reply;

    if (!redis->openConnection())
    {
        qDebug() << "Could not connect to server...";
    redisSuccess = false;
    }
    redisSuccess = true;
    qDebug() << "Connected to server...";

    //qDebug() << "AUTH:" << redis->auth("redisZzZ");

    // Set and Get example
//    qDebug() << "SET:" << redis->set("key", "\"Hello World\"");
//    qDebug() << "GET:" << redis->get("key");

//    // Append to Key example
//    qDebug() << "SET:" << redis->set("key", "\"Hello\"");
//    qDebug() << "EXISTS:" << redis->exists("key");
//    qDebug() << "GET:" << redis->get("key");
//    qDebug() << "APPEND:" << redis->append("key", "\" World\"");
//    qDebug() << "GET:" << redis->get("key");

//    // Multi Set and Get example
//    QMap<QString,QVariant> keypairs;
//    keypairs["key1"] = QString("\"Hello\"");
//    keypairs["key2"] = QString("\" world\"");
//    qDebug() << "MSET:" << redis->mset(keypairs);
//    qDebug() << "MGET:" << redis->mget("key1 key2 nonexisting");

//    // Incr, incrby decr, decrby example.
//    qDebug() << "SET:" << redis->set("count", "10");
//    qDebug() << "INCR:" << redis->incr("count");
//    qDebug() << "GET:" << redis->get("count");

//    qDebug() << "INCRBY:" << redis->incrby("count",5);
//    qDebug() << "GET:" << redis->get("count");

//    qDebug() << "DECR:" << redis->decr("count");
//    qDebug() << "GET:" << redis->get("count");

//    qDebug() << "DECRBY:" << redis->decrby("count",5);
//    qDebug() << "GET:" << redis->get("count");

//    // SET and GET Range examples
//    qDebug() << "SETRANGE:" << redis->setrange("key",6 ,"Redis");
//    qDebug() << "GET:" << redis->get("key");

//    qDebug() << "GETRANGE:" << redis->getrange("key",-5 ,-1);
//    qDebug() << "GETRANGE:" << redis->getrange("key",0 ,4);
//    qDebug() << "GETRANGE:" << redis->getrange("key",0 ,-1);


//    //Hashmap example
//    qDebug() << "HSET:" << redis->hset("hashmap","key1" ,"value1");
//    qDebug() << "HSET:" << redis->hset("hashmap","key2" ,"value2");
//    qDebug() << "HSET:" << redis->hset("hashmap","key3" ,"value3");
//    qDebug() << "HGETALL:";

//    QMap<QString,QVariant> hashmap = redis->hgetall("hashmap");
//    QMapIterator<QString, QVariant> mi(hashmap);
//    while (mi.hasNext())
//    {
//        mi.next();
//        qDebug() << mi.key() << "=" << mi.value().toString();
//    }

//    qDebug() << "HVALS:" << redis->hvals("hashmap");


//    // Raw Command example
//    reply = redis->command("GET key");
//    qDebug() << "RAW:" << "("<< reply.type << ")" << reply.value.toString();

//    redis->subscribe("notifications");

//   redis->psubscribe("news.*");

//    //reply = redis->command("SUBSCRIBE notifications");
//    //qDebug() << "("<< reply.type << ")" << reply.value.toStringList();

}

void preFeed::safeQuit(){
 qDebug() << "mousePressEvent:" << mousecount;
    monitorThread.stopMonitorThread();
    if(!monitorThread.wait(3000)) //Wait until it actually has terminated (max. 3 sec)
    {
        monitorThread.terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
        monitorThread.wait(); //We have to wait again here!
    }

    for(int i = 1; i < 17; i++){
         if(comDEV[i].exist){
             comDEV[i].modbusSlave->stopModbusSlave();
             if(!comDEV[i].modbusSlave->wait(3000)) //Wait until it actually has terminated (max. 3 sec)
             {
                comDEV[i].modbusSlave->terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
                 comDEV[i].modbusSlave->wait(); //We have to wait again here!
             }
           }
         }
        qApp->quit();
//     monitorThread.stopMonitorThread();
//     monitorThread.wait();
//      for(int i = 1; i < 17; i++){
//     if(comDEV[i].exist){
//         comDEV[i].modbusSlave->stopModbusSlave();
//         comDEV[i].modbusSlave->wait();
//       }
//     }
//      redis->stopRedis();
//      redis->wait();

}
