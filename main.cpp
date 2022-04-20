#include "prefeed.h"
#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QMutex>
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)      //在main函数之前定义回调函数
{
    static QMutex mutex;
        mutex.lock();
    QByteArray localMsg = msg.toUtf8();
     QString strMsg("");
        QString txt;
         switch(type)
             {
             case QtDebugMsg:
                 strMsg = QString("Debug:");
                 break;
             case QtWarningMsg:
                 strMsg = QString("Warning:");
                 break;
             case QtCriticalMsg:
                 strMsg = QString("Critical:");
                 break;
             case QtFatalMsg:
                 strMsg = QString("Fatal:");
                 break;
             }

          QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
             QString strMessage = QString("Message:%1  ")
                     .arg(localMsg.constData());

              QFile file("log.txt");
                 file.open(QIODevice::ReadWrite | QIODevice::Append);
                 QTextStream stream(&file);
                 stream << strMessage << "\r\n";
                 file.flush();
                 file.close();
         mutex.unlock();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    preFeed w;
    w.showFullScreen();
     qInstallMessageHandler(customMessageHandler);
    return a.exec();
}
