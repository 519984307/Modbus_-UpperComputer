#ifndef MODBUSSLAVE_H
#define MODBUSSLAVE_H

#include <QThread>
#include "common.h"

class QModbusClient;
class QModbusReply;

class ModbusSlave : public QThread
{
    Q_OBJECT

public:
    int com;
    bool commanding;
    bool canRun;
    bool isrun;
    explicit ModbusSlave(QObject *parent = nullptr);
    void write();
    void read(int adds, int starAddr, int valuCount);
    void readState(int adds);
    void readEartag(int adds);
    void readEatValue(int adds);
    void writeFeed(int adds, float feedValue, int A, int B);
    void setComErr(int adds);
    void clrComErr(int adds);
    bool waitComRease(int hundredMS);
    void stopModbusSlave();
protected:
   void run();

private:

private slots:
   void readReady();

signals:
    void message(QString msg);
    void modbusRead(int unit);
    void modbusWrite(int unit);
};

#endif // MODBUSSLAVE_H
