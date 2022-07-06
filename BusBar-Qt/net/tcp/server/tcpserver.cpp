#include "tcpserver.h"

#define ANDROID_TCP_PORT	11283  // 案桌TCP端口号

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    m_tcpServer = new QTcpServer(this);
    //设置最大允许连接数，不设置的话默认为30
    m_tcpServer->setMaxPendingConnections(2000);
    connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnectSlot()));
    mThr = new ThrNetData;
    mBuf = (uchar *)malloc(RTU_BUF_SIZE); //申请内存  -- 随便用
    //    mSerial = new Serial_Trans(this); //串口线程
    mShm = get_share_mem(); // 获取共享内存
}

void TcpServer::init(int port, bool isVerify)
{
    if(m_tcpServer->listen(QHostAddress::AnyIPv4, port)){
        qDebug() << "listen OK!";
    }else{
        qDebug() << "listen error!";
    }
    mIsVerify = isVerify;
    mBox = nullptr;
}

void TcpServer::newConnectSlot()
{
    QTcpSocket *tcp = m_tcpServer->nextPendingConnection();
    qDebug() <<"newConnectSlot"<<endl;
    if(mIsVerify) {
        connect(tcp,SIGNAL(readyRead()),this,SLOT(readMessage()));
        mIsConnect = false;
    } else {
        mIsConnect = true;
    }
    m_mapClient.insert(tcp->peerAddress().toString(), tcp);
    connect(tcp,SIGNAL(disconnected()),this,SLOT(removeUserFormList()));
    mIP = tcp->peerAddress().toString();

    //    m_pMsgHandler->devOnline(tcp->peerAddress().toString());
}

bool TcpServer::isConnect()
{
    bool ret = false;
    if(m_mapClient.contains(mIP))
        ret = mIsConnect;
    return ret;
}

void TcpServer::sendData(char *data)
{
    if(isConnect())
        m_mapClient.value(mIP)->write(QByteArray(data));
}

void TcpServer::sendData(uchar *data, int len)
{
    if(isConnect())
        m_mapClient.value(mIP)->write((char *)data, len);
}

void TcpServer::sendData(const QByteArray &data)
{
    if(isConnect())
        m_mapClient.value(mIP)->write(data);
}

int TcpServer::readData(QString &ip, char *data)
{
    int ret=0;

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    if(socket) {
        ip = socket->peerAddress().toString();
        ret = socket->read(data, 256);
    }
    return ret;
}

/**
 * 首先进行身份验证
 */
void TcpServer::landVerify(QTcpSocket *socket)
{
    qDebug() <<socket->peerAddress().toString()<<m_mapClient.value(socket->peerAddress().toString());

    uchar *inbuf = mBuf;
    mBox = nullptr;
    QByteArray by = socket->readAll();
    memset(mBuf , 0 , sizeof(mBuf));
    //QByteArray by = QByteArray::fromHex(byte);
    int rtn = 0;
    for(int i = 0 ; i < by.length() ; i++){
        mBuf[rtn++] = by.at(i);
    }
    transData(inbuf , rtn);
    if(mBox != nullptr){
        int n = socket->write((char*)mBox->rtuArray, mBox->rtuLen);
        socket->flush();
    }
    mIsConnect = true;
}

void TcpServer::readMessage()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    landVerify(socket);

    //disconnect(socket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    //    m_pMsgHandler->recvFromServer(socket->peerAddress().toString(), QString(socket->readAll()));
}

void TcpServer::removeUserFormList()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QMap<QString, QTcpSocket *>::iterator it;
    qDebug() <<"removeUserFormList"<<endl;
    for(it=m_mapClient.begin();it!=m_mapClient.end();it++)
    {
        if(socket->peerAddress().toString() == it.key())
        {
            m_mapClient.erase(it);
            //            m_pMsgHandler->devOffline(socket->peerAddress().toString());

            break;
        }
    }
}

void TcpServer::setCrc(uchar *buf, int len)
{
    int rtn = len-2;
    ushort crc =  rtu_crc(buf, rtn);
    buf[rtn++] = 0xff & crc; /*低8位*/
    buf[rtn++] = crc >> 8;
}

void TcpServer::transData(uchar *buf, int len)
{
    //int rtn = mSerial->recvData(buf, 5); //接收数据-
    int rtn = len;
    QByteArray sendarray;
    QString sendstrArray;
    sendarray.append((char *)buf, rtn);
    sendstrArray = sendarray.toHex(); // 十六进制
    for(int i=0; i<sendarray.size(); ++i)
        sendstrArray.insert(2+3*i, " "); // 插入空格
    qDebug()<<"  send:" << sendstrArray;
    qDebug()<< "rtn  "<<rtn;
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
                box->rtuArray[0] = mThr->addr;//
                setCrc(box->rtuArray, box->rtuLen);//
                //mSerial->sendData(box->rtuArray, box->rtuLen);
                mBox = box;
            } else {
                //mSerial->sendData(buf, rtn);
            }
        } else if(mThr->fn == Fn_NetSet){ //发送数据
            //mSerial->sendData(buf, rtn); //先回应同样的数据
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

bool TcpServer::validateData(int rtn)
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
    return true;
}

uint TcpServer::calcZeroCur(sBoxData *box)
{
    ushort v1= box->data.cur.value[0];
    ushort v2= box->data.cur.value[1];
    ushort v3= box->data.cur.value[2];
    if(v1==0 && v2==0 && v2==0) return 0;
    uint a = v1*v1;
    uint b = v2*v2;
    uint c = v3*v3;
    uint d = v1*v2;
    uint e = v2*v3;
    uint f = v1*v3;
    if(a+b+c-d-e-f<0) return 0;
    uint zeroLine = sqrt(a+b+c-d-e-f);
    return zeroLine;
}

/**
  * 功　能：发送始端箱数据打包
  * 入口参数：pkt -> 发送结构体
  * 出口参数：ptr -> 缓冲区
  * 返回值：打包后的长度
  */
uchar TcpServer::rtu_sent_to_input_packet(sBoxData *box)
{
    uchar *ptr = box->rtuArray;
    memset(box->rtuArray,0,sizeof(box->rtuLen));
    *(ptr++) = mThr->addr;  /*地址码*/
    *(ptr++) = Fn_NetGet; /*功能码*/
    *(ptr++) = 0x28; /*功能码*///3

    /*填入输入开关*/
    for(int i = 0 ; i < 3 ; i++)
    {
        *(ptr++) = 0x00; /*高8位*/
        *(ptr++) = (0xff)&(box->data.sw[i]); /*低8位*/
    }//6

    /*填入输入电流*/
    for(int i = 0 ; i < 3 ; i++)
    {
        *(ptr++) = ((box->data.cur.value[i]) >> 8); /*高8位*/
        *(ptr++) = (0xff)&(box->data.cur.value[i]); /*低8位*/
    }//6//12

    /*填入输入零线电流*/
    ushort zeroLine =(ushort) calcZeroCur(box);
    *(ptr++) = ((zeroLine) >> 8); /*高8位*/
    *(ptr++) = (0xff)&(zeroLine); /*低8位*///2//14

    /*填入输入电压*/
    for(int i = 0 ; i < 3 ; i++)
    {
        *(ptr++) = ((box->data.vol.value[i]) >> 8); /*高8位*/
        *(ptr++) = (0xff)&(box->data.vol.value[i]); /*低8位*/
    }//6//20

    /*填入输入有功功率*/
    for(int i = 0 ; i < 3 ; i++)
    {
        *(ptr++) = ((box->data.pow.value[i]) >> 8); /*高8位*/
        *(ptr++) = (0xff)&(box->data.pow.value[i]); /*低8位*/
    }//6//26

    /*填入输入视在功率*/
    for(int i = 0 ; i < 3 ; i++)
    {
        *(ptr++) = ((box->data.apPow[i]) >> 8); /*高8位*/
        *(ptr++) = (0xff)&(box->data.apPow[i]); /*低8位*/
    }//32
    /*填入输入功率因数*/
    for(int i = 0 ; i < 3 ; i++)
    {
        *(ptr++) = ((box->data.pf[i]) >> 8); /*高8位*/
        *(ptr++) = (0xff)&(box->data.pf[i]); /*低8位*/
    }//38
    /*填入输入频率*/
    *(ptr++) = ((box->rate) >> 8); /*高8位*/
    *(ptr++) = (0xff)&(box->rate); /*低8位*///40

    /*填入CRC*/
    ushort crc =  rtu_crc(box->rtuArray, 40+3);
    *(ptr++) = 0xff & crc; /*低8位*/
    *(ptr++) = crc >> 8;
    return 40;
}

/**
  * 功　能：发送插接箱数据打包
  * 入口参数：pkt -> 发送结构体
  * 出口参数：ptr -> 缓冲区
  * 返回值：打包后的长度
  */
uchar TcpServer::rtu_sent_to_output_packet(sBoxData *box)
{
    uchar *ptr = box->rtuArray;
    memset(box->rtuArray,0,sizeof(box->rtuLen));
    *(ptr++) = mThr->addr;  /*地址码*/
    *(ptr++) = Fn_NetGet; /*功能码*/
    *(ptr++) = 0x1e; /*功能码*///3

    /*填入输出电流*/
    for(int i = 0 ; i < 3 ; i++)
    {
        if(box->loopNum==3){
            *(ptr++) = ((box->data.cur.value[i]) >> 8); /*高8位*/
            *(ptr++) = (0xff)&(box->data.cur.value[i]); /*低8位*/
        }else{
            *(ptr++) = ((box->lineTgBox.cur[i]) >> 8); /*高8位*/
            *(ptr++) = (0xff)&(box->lineTgBox.cur[i]); /*低8位*/
        }
    }//6

    /*填入输出电压*/
    for(int i = 0 ; i < 3 ; i++)
    {
        if(box->loopNum==3){
            *(ptr++) = 0x00; /*高8位*/
            *(ptr++) = (0xff)&(box->data.sw[i]); /*低8位*/
        }else{
            *(ptr++) = 0x00;
            *(ptr++) = (0xff)&(box->data.sw[i*3]); /*低8位*/
        }
    }//6//12

    /*填入输出功率*/
    for(int i = 0 ; i < 3 ; i++)
    {
        if(box->loopNum==3){
            *(ptr++) = ((box->data.pow.value[i]) >> 8); /*高8位*/
            *(ptr++) = (0xff)&(box->data.pow.value[i]); /*低8位*/
        }else{
            *(ptr++) = ((box->lineTgBox.pow[i]) >> 8); /*高8位*/
            *(ptr++) = (0xff)&(box->lineTgBox.pow[i]); /*低8位*/
        }
    }//6//18


    /*填入输出电能*/
    for(int i = 0 ; i < 3 ; i++)
    {
        if(box->loopNum==3){
            *(ptr++) = ((box->data.ele[i]) >> 24); /*8位*/
            *(ptr++) = (0xff)&((box->data.ele[i]) >> 16); /*8位*/
            *(ptr++) = ((0xff)&(box->data.ele[i])>>8); /*8位*/
            *(ptr++) = (0xff)&(box->data.ele[i]); /*8位*/
        }else{
            *(ptr++) = ((box->lineTgBox.ele[i]) >> 24); /*8位*/
            *(ptr++) = (0xff)&((box->lineTgBox.ele[i]) >> 16); /*8位*/
            *(ptr++) = ((0xff)&(box->lineTgBox.ele[i])>>8); /*8位*/
            *(ptr++) = (0xff)&(box->lineTgBox.ele[i]); /*8位*/
        }
    }//6//30

    /*填入CRC*/
    ushort crc =  rtu_crc(box->rtuArray, 30+3);
    *(ptr++) = 0xff & crc; /*低8位*/
    *(ptr++) = crc >> 8;
    return 30;
}
