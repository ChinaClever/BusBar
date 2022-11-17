#include "serverthread.h"

serverThread::serverThread(int sockDesc, QObject *parent) :
    QThread(parent),
    m_sockDesc(sockDesc)
{
    mThr = new ThrNetData;
    mBuf = (uchar *)malloc(RTU_BUF_SIZE); //申请内存  -- 随便用
    mShm = get_share_mem(); // 获取共享内存
}

serverThread::~serverThread()
{
    m_socket->close();
    if(m_socket) {delete m_socket;m_socket = nullptr;}
    if(mThr) {delete mThr;mThr = nullptr;}
    if(mBuf) {free(mBuf);mBuf = nullptr;}
}

void serverThread::run(void)
{
    m_socket = new MySocket(m_sockDesc);

    if (!m_socket->setSocketDescriptor(m_sockDesc)) {
        return ;
    }

    connect(m_socket, &MySocket::disconnected, this, &serverThread::disconnectToHost);
    connect(m_socket, SIGNAL(dataReady(const QString&, const QByteArray&)),
            this, SLOT(recvDataSlot(const QString&, const QByteArray&)));
    connect(this, SIGNAL(sendData(int, const char * ,int)),
            m_socket, SLOT(sendData(int, const char * ,int)));

    this->exec();
}

void serverThread::sendDataSlot(int sockDesc, const char *data ,int len)//int id, const char *data , int len
{
    if (len == 0) {
        return ;
    }

    emit sendData(sockDesc, data , len);
}

void serverThread::recvDataSlot(const QString &ip, const QByteArray &data)
{
    int rtn = 0;
    memset(mBuf , 0 , sizeof(mBuf));
    for(int i = 0 ; i < data.length() ; i++){
        mBuf[rtn++] = data.at(i);
    }
    transData(mBuf , rtn);
}

void serverThread::disconnectToHost(void)
{
    m_socket->disconnectFromHost();
    disconnect(m_socket, SIGNAL(dataReady(const QString&, const QByteArray&)),
               this, SLOT(recvDataSlot(const QString&, const QByteArray&)));
    this->quit();
}


void serverThread::transData(uchar *buf, int len)
{
    int rtn = len;
//        QByteArray recvarray;
//        QString recvstrArray;
//        recvarray.append((char *)buf, rtn);
//        recvstrArray = recvarray.toHex(); // 十六进制
//        for(int i=0; i<recvarray.size(); ++i)
//            recvstrArray.insert(2+3*i, " "); // 插入空格
//        qDebug()<<"   recv:" << recvstrArray;
//        qDebug()<< "rtn  "<<rtn;
    if(rtn > 2 ) {
        if(!validateData(rtn)) return; //解析并验证数据
        uchar id = mThr->addr / 0x20;
        uchar addr = mThr->addr % 0x20;
        if(id >=BUS_NUM || addr >= BOX_NUM) return;
        //        sBoxData *box = &(mShm->data[id].box[addr]); //共享内存
        if(addr-1 < 0) return;//上海创建
        sBoxData *box = &(mShm->data[id].box[addr-1]);
        if(box->offLine < 1) return;

        if(mThr->fn == Fn_NetGet){ //获取数据 _ [未加长度位0时该回复数据]
            if(box->rtuLen > 0) {
//                memset(mBuf , 0 , sizeof(mBuf));
//                mBuf[0] = mThr->addr;//
//                mBuf[1] = 0x03;
//                mBuf[2] = mThr->data >> 8;
//                mBuf[3] = mThr->data & 0xFF;
//                int pos = mThr->position;
//                for(int i = 4 ; i < mThr->data ; i++){
//                    mBuf[i] = box->rtuArray[pos+i];
//                }
//                setCrc( mBuf, mThr->data + 6 );//


//                                QByteArray sendarray;
//                                QString sendstrArray;
//                                sendarray.append((char *)mBuf, mThr->data + 6);
//                                sendstrArray = sendarray.toHex(); // 十六进制
//                                for(int i=0; i<sendarray.size(); ++i)
//                                    sendstrArray.insert(2+3*i, " "); // 插入空格
//                                qDebug()<<"  send:" << sendstrArray;

//                sendDataSlot(m_sockDesc , (char*)mBuf , mThr->data + 6);

                box->rtuArray[0] = mThr->addr;//
                setCrc(box->rtuArray, box->rtuLen);//
                sendDataSlot(m_sockDesc , (char*)box->rtuArray , box->rtuLen);
            } else {
                //mSerial->sendData(buf, rtn);
            }
        } else if(mThr->fn == Fn_NetSet){ //发送数据
            if(rtu[id] != NULL) {
                buf[0] = addr;
                setCrc(buf, rtn);
                rtu[id]->sendData(buf, rtn, 200); //[最好放入其他线程——暂时放这]
            }
        }else{ //功能码不合法

        }

    }else{

    }
}

bool serverThread::validateData(int rtn)
{
    uchar *buf = mBuf;
    buf = mBuf;
    mThr->addr = *(buf++);
    mThr->fn   = *(buf++);
    ushort ll, hh;
    hh = *buf++;
    ll = *buf++;
    mThr->position = (hh<<8) + ll;
    hh = *buf++;
    ll = *buf++;
    mThr->data = (hh<<8) + ll;
    ll = *buf++;
    hh = *buf++;
    mThr->crc = (ll<<8) + hh;
    buf = mBuf;
    ushort hlcrc = rtu_crc(buf, rtn-2);
    ushort lhcrc = (hlcrc >> 8) + (hlcrc<<8);
    if(mThr->crc != lhcrc ) return false;
    //if( mThr->position + mThr->data > 0x135 ) return false;
    return true;
}

void serverThread::setCrc(uchar *buf, int len)
{
    int rtn = len-2;
    ushort crc =  rtu_crc(buf, rtn);
    buf[rtn++] = 0xff & crc; /*低8位*/
    buf[rtn++] = crc >> 8;
}
