#include "common.h"

InfoStruct infoStruct;
ComSettings comSettings;
ComDEV comDEV[17];
QNetworkAccessManager *manager;
QNetworkRequest *request;
QtRedis *redis;
QNetworkReply *replyregister,*replyplclist,*replydetail,*replydisplay,*replyplan;
