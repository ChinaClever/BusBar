#include "tcpthread.h"
static bool isconnecting = false; //是否處於正常鏈接（用戶名密碼都正確，且TCP處於鏈接狀態）
bool getConnectingState()
{
    return isconnecting;
}

//static bool landVerify(QByteArray array);

TcpThread::TcpThread(QObject *parent)
{
    is_run = false;
    mServer = getTcpServer();

    QTimer *timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutdone()));
    timer->start(5*1000);
}

void TcpThread::run()
{
    if(is_run == false)
    {
        is_run  = true;
        if(!isconnecting)  //如果未處於鏈接狀態，那麼需要驗證用戶名及密碼
        {
            if(mServer->isConnecting())
            {
                QByteArray array = mServer->getData();
                bool ret = landVerify(array);
                if(ret == true) //驗證成功，則設置標志位爲1
                {
                    isconnecting = true;
                    reply_to_client();  //回复ok
                }
            }
        }else  //如果處於鏈接，那麼判斷當前服務器是否處於鏈接狀態，是則設置標志位爲0
        {
            if(!mServer->isConnecting())
                isconnecting = false;
            else //服務器處於正常鏈接狀態，等待app設備設置數據
            {
                QByteArray array = mServer->getData();

                //解析數據

            }
        }
        is_run  = false;
    }
}

/*
bool landVerify(QByteArray array)
{
    QString str = QString(array);
    if(str == "admin; admin; 0")
        return true;
    return false;
}
*/

void TcpThread::timeoutdone()
{
    if(!is_run)
    {
        start();
    }
}

