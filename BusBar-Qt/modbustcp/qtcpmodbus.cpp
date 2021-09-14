/***********************************************************************************************************************
* QTcpModbus imlpementation.                                                                                          *
***********************************************************************************************************************/
#include "qtcpmodbus.h"
//#include <QtNetwork>

/*** Qt includes ******************************************************************************************************/
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>


/*** Class implementation *********************************************************************************************/
QTcpModbus::QTcpModbus() : _timeout( 500 ) , _connectTimeout( 5000 )
{
    // Initialize random number generator to use for transaction ID generation.
    qsrand( time( NULL ) );

    mShm = get_share_mem(); // 获取共享内存
    mIP = "";
    mIsConnect = false;

}



void QTcpModbus::init(int port)
{
    _socket = new ModbusTcpServer(port);
//    if(_socket->listen(QHostAddress::Any, port)){
//        qDebug() << "listen OK!"<<port;
//    }else{
//        qDebug() << "listen error!";
//    }
    _socket->run();
}


void QTcpModbus::writeSlot()
{
    QString str = "123456789012";
    QString str2 = str.replace(" ","");

    QList<quint16> data;
    for(int i = 0 ;i < 20;i++)
    {
        int j = 2*i;
        QString str1 = str2.mid(j,2);
        bool ok;
        quint16 hex = str1.toInt(&ok,16);
//        qDebug("%d",hex);
        data.push_back(hex);
    }
    quint8* status = nullptr;
    // Are we connected ?
    if ( !isConnected() )
    {
        if ( status ) *status = NoConnection;
        return ;
    }

    bool ret = false;
    // Create a transaction number.
    quint16 rxTransactionId , rxStartingAddress , rxQuantityOfRegisters;
    quint8 rxDeviceAddress,rxFunctionCode;

    // Create modbus write multiple registers pdu (Modbus uses Big Endian).
    QByteArray pdu;
    pdu.clear();
    pdu = reinterpret_cast<QTcpSocket*>(sender())->readAll();
    qDebug()<<"pdu.size() "<<pdu.size()<<endl;
    // Check data and return them on success.
    if ( pdu.size() == 12 )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint16 rxProtocolId, rxLength ;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxTransactionId >> rxProtocolId >> rxLength >> rxDeviceAddress >> rxFunctionCode
                >> rxStartingAddress >> rxQuantityOfRegisters;

        qDebug()<<rxTransactionId << rxStartingAddress << rxQuantityOfRegisters << rxDeviceAddress << rxFunctionCode <<endl;
        // Control values of the fields.
        if ( rxProtocolId == 0 && rxLength == 6 && rxFunctionCode == 0x03 )
        {
            // Ok, done.
            if ( status ) *status = Ok;
            ret = true;
        }
        else
        {
            if ( status ) *status = UnknownError;
        }
    }
    else
    {
        // What was wrong ?
        if ( pdu.size() == 9 )
        {
            if ( status ) *status = pdu[8];
        }
        else
        {
            if ( status ) *status = Timeout;
        }
    }
    bool flag = false;
    if(ret){
        uchar id = rxDeviceAddress / 0x20;
        uchar addr = rxDeviceAddress % 0x20;
        if(id >=BUS_NUM || addr >= BOX_NUM) return ;
        sBoxData *box = &(mShm->data[id].box[addr-1]); //共享内存
        //if(box->offLine < 1) return false;
        flag= writeMultipleRegisters(rxQuantityOfRegisters ,rxTransactionId , rxDeviceAddress , rxFunctionCode, data , ret);
    }
    return ;
}

QAbstractModbus::~QAbstractModbus()
{

}

QTcpModbus::~QTcpModbus()
{
    // Finaly disconnect.
    disconnect();
    mIsConnect = false;
}



bool QTcpModbus::isConnected( void ) const
{
    // Ask the socket if it is connected.
    bool ret = false;
    if(m_mapClient.contains(mIP))
        ret = mIsConnect;
    return ret;
}

void QTcpModbus::disconnect( void )
{
    // Close the socket's connection.
    _socket->close();
}

int QTcpModbus::connectTimeout( void ) const
{
    return _connectTimeout;
}

void QTcpModbus::setConnectTimeout( const int timeout )
{
    _connectTimeout = timeout;
}

unsigned int QTcpModbus::timeout( void ) const
{
    return _timeout;
}

void QTcpModbus::setTimeout( const unsigned int timeout )
{
    _timeout = timeout;
}

QList<bool> QTcpModbus::readCoils( const quint8 deviceAddress , const quint16 startingAddress ,
                                   const quint16 quantityOfCoils , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isConnected() )
    {
        if ( status ) *status = NoConnection;
        return QList<bool>();
    }

    // Create a transaction number.
    quint16 transactionId = qrand();

    // Create tcp/modbus read coil status pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << transactionId << (quint16)0 << (quint16)6
              << deviceAddress << (quint8)0x01 << startingAddress << quantityOfCoils;

    // Clear the RX buffer before making the request.
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->readAll();


    // Send the pdu.
    reinterpret_cast<QTcpSocket*>(sender())->write( pdu );

    // Await response.
    quint16 neededRxBytes = quantityOfCoils / 8;
    if ( quantityOfCoils % 8 ) neededRxBytes++;
    pdu.clear();
    while ( pdu.size() < neededRxBytes + 9 )
    {
        if ( ( pdu.size() >= 9 && pdu[7] & 0x80 ) || !socket->waitForReadyRead( _timeout ) ) break;
        pdu += socket->read( neededRxBytes + 9 - pdu.size() );
    }


    // Check data and return them on success.
    if ( pdu.size() == neededRxBytes + 9 )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint16 rxTransactionId , rxProtocolId, rxLength;
        quint8 rxDeviceAddress , rxFunctionCode , byteCount;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxTransactionId >> rxProtocolId >> rxLength >> rxDeviceAddress >> rxFunctionCode >> byteCount;

        // Control values of the fields.
        if ( rxTransactionId == transactionId && rxProtocolId == 0 && rxLength == neededRxBytes + 3 &&
             rxDeviceAddress == deviceAddress && rxFunctionCode == 0x01 && byteCount == neededRxBytes )
        {
            // Convert data.
            QList<bool> list;
            quint8 tmp;
            for ( int i = 0 ; i < quantityOfCoils ; i++ )
            {
                if ( i % 8 == 0 ) rxStream >> tmp;
                list.append( tmp & ( 0x01 << ( i % 8 ) ) );
            }
            if ( status ) *status = Ok;
            return list;
        }
        else
        {
            if ( status ) *status = UnknownError;
        }
    }
    else
    {
        // What was wrong ?
        if ( pdu.size() == 9 )
        {
            if ( status ) *status = pdu[8];
        }
        else
        {
            if ( status ) *status = Timeout;
        }
    }

    return QList<bool>();
}

QList<bool> QTcpModbus::readDiscreteInputs( const quint8 deviceAddress , const quint16 startingAddress ,
                                            const quint16 quantityOfInputs , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isConnected() )
    {
        if ( status ) *status = NoConnection;
        return QList<bool>();
    }

    // Create a transaction number.
    quint16 transactionId = qrand();

    // Create tcp/modbus read input status pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << transactionId << (quint16)0 << (quint16)6
              << deviceAddress << (quint8)0x02 << startingAddress << quantityOfInputs;

    // Clear the RX buffer before making the request.
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->readAll();

    // Send the pdu.
    reinterpret_cast<QTcpSocket*>(sender())->write( pdu );

    // Await response.
    quint16 neededRxBytes = quantityOfInputs / 8;
    if ( quantityOfInputs % 8 ) neededRxBytes++;
    pdu.clear();
    while ( pdu.size() < neededRxBytes + 9 )
    {
        if ( ( pdu.size() >= 9 && pdu[7] & 0x80 ) || !socket->waitForReadyRead( _timeout ) ) break;
        pdu += socket->read( neededRxBytes + 9 - pdu.size() );
    }

    // Check data and return them on success.
    if ( pdu.size() == neededRxBytes + 9 )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint16 rxTransactionId , rxProtocolId, rxLength;
        quint8 rxDeviceAddress , rxFunctionCode , byteCount;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxTransactionId >> rxProtocolId >> rxLength >> rxDeviceAddress >> rxFunctionCode >> byteCount;

        // Control values of the fields.
        if ( rxTransactionId == transactionId && rxProtocolId == 0 && rxLength == neededRxBytes + 3 &&
             rxDeviceAddress == deviceAddress && rxFunctionCode == 0x02 && byteCount == neededRxBytes )
        {
            // Convert data.
            QList<bool> list;
            quint8 tmp;
            for ( int i = 0 ; i < quantityOfInputs ; i++ )
            {
                if ( i % 8 == 0 ) rxStream >> tmp;
                list.append( tmp & ( 0x01 << ( i % 8 ) ) );
            }
            if ( status ) *status = Ok;
            return list;
        }
        else
        {
            if ( status ) *status = UnknownError;
        }
    }
    else
    {
        // What was wrong ?
        if ( pdu.size() == 9 )
        {
            if ( status ) *status = pdu[8];
        }
        else
        {
            if ( status ) *status = Timeout;
        }
    }

    return QList<bool>();
}

QList<quint16> QTcpModbus::readHoldingRegisters( const quint8 deviceAddress , const quint16 startingAddress ,
                                                 const quint16 quantityOfRegisters , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isConnected() )
    {
        if ( status ) *status = NoConnection;
        return QList<quint16>();
    }

    // Create a transaction number.
    quint16 transactionId = qrand();

    // Create tcp/modbus read holding registers pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << transactionId << (quint16)0 << (quint16)6
              << deviceAddress << (quint8)0x03 << startingAddress << quantityOfRegisters;

    // Clear the RX buffer before making the request.
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->readAll();

    // Send the pdu.
    socket->write( pdu );

    // Await response.
    quint16 neededRxBytes = quantityOfRegisters * 2;
    pdu.clear();
    while ( pdu.size() < neededRxBytes + 9 )
    {
        if ( ( pdu.size() >= 9 && pdu[7] & 0x80 ) || !socket->waitForReadyRead( _timeout ) ) break;
        pdu += socket->read( neededRxBytes + 9 - pdu.size() );
    }

    // Check data and return them on success.
    if ( pdu.size() == neededRxBytes + 9 )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint16 rxTransactionId , rxProtocolId, rxLength;
        quint8 rxDeviceAddress , rxFunctionCode , byteCount;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxTransactionId >> rxProtocolId >> rxLength >> rxDeviceAddress >> rxFunctionCode >> byteCount;

        // Control values of the fields.
        if ( rxTransactionId == transactionId && rxProtocolId == 0 && rxLength == neededRxBytes + 3 &&
             rxDeviceAddress == deviceAddress && rxFunctionCode == 0x03 && byteCount == neededRxBytes )
        {
            // Convert data.
            QList<quint16> list;
            quint16 tmp;
            for ( int i = 0 ; i < quantityOfRegisters ; i++ )
            {
                rxStream >> tmp;
                list.append( tmp );
            }
            if ( status ) *status = Ok;
            return list;
        }
        else
        {
            if ( status ) *status = UnknownError;
        }
    }
    else
    {
        // What was wrong ?
        if ( pdu.size() == 9 )
        {
            if ( status ) *status = pdu[8];
        }
        else
        {
            if ( status ) *status = Timeout;
        }
    }

    return QList<quint16>();
}

QList<quint16> QTcpModbus::readInputRegisters( const quint8 deviceAddress , const quint16 startingAddress ,
                                               const quint16 quantityOfInputRegisters , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isConnected() )
    {
        if ( status ) *status = NoConnection;
        return QList<quint16>();
    }

    // Create a transaction number.
    quint16 transactionId = qrand();

    // Create modbus read input status pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << transactionId << (quint16)0 << (quint16)6
              << deviceAddress << (quint8)0x04 << startingAddress << quantityOfInputRegisters;

    // Clear the RX buffer before making the request.
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->readAll();

    // Send the pdu.
    socket->write( pdu );

    // Await response.
    quint16 neededRxBytes = quantityOfInputRegisters * 2;
    pdu.clear();
    while ( pdu.size() < neededRxBytes + 9 )
    {
        if ( ( pdu.size() >= 9 && pdu[7] & 0x80 ) || !socket->waitForReadyRead( _timeout ) ) break;
        pdu += socket->read( neededRxBytes + 9 - pdu.size() );
    }

    // Check data and return them on success.
    if ( pdu.size() == neededRxBytes + 9 )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint16 rxTransactionId , rxProtocolId, rxLength;
        quint8 rxDeviceAddress , rxFunctionCode , byteCount;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxTransactionId >> rxProtocolId >> rxLength >> rxDeviceAddress >> rxFunctionCode >> byteCount;

        // Control values of the fields.
        if ( rxTransactionId == transactionId && rxProtocolId == 0 && rxLength == neededRxBytes + 3 &&
             rxDeviceAddress == deviceAddress && rxFunctionCode == 0x04 && byteCount == neededRxBytes )
        {
            // Convert data.
            QList<quint16> list;
            quint16 tmp;
            for ( int i = 0 ; i < quantityOfInputRegisters ; i++ )
            {
                rxStream >> tmp;
                list.append( tmp );
            }
            if ( status ) *status = Ok;
            return list;
        }
        else
        {
            if ( status ) *status = UnknownError;
        }
    }
    else
    {
        // What was wrong ?
        if ( pdu.size() == 9 )
        {
            if ( status ) *status = pdu[8];
        }
        else
        {
            if ( status ) *status = Timeout;
        }
    }

    return QList<quint16>();
}

bool QTcpModbus::writeSingleCoil( const quint8 deviceAddress , const quint16 outputAddress ,
                                  const bool outputValue , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isConnected() )
    {
        if ( status ) *status = NoConnection;
        return false;
    }

    // Create a transaction number.
    quint16 transactionId = qrand();

    // Create modbus write single coil pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << transactionId << (quint16)0 << (quint16)6
              << deviceAddress << (quint8)0x05 << outputAddress << ( outputValue ? (quint16)0xFF00 : (quint16)0x0000 );

    // Clear the RX buffer before making the request.
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->readAll();

    // Send the pdu.
    socket->write( pdu );

    // Await response.
    pdu.clear();
    while ( pdu.size() < 12 )
    {
        if ( ( pdu.size() >= 9 && pdu[7] & 0x80 ) || !socket->waitForReadyRead( _timeout ) ) break;
        pdu += socket->read( 12 - pdu.size() );
    }

    // Check data and return them on success.
    if ( pdu.size() == 12 )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint16 rxTransactionId , rxProtocolId, rxLength , rxOutputAddress , rxOutputValue;
        quint8 rxDeviceAddress , rxFunctionCode;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxTransactionId >> rxProtocolId >> rxLength >> rxDeviceAddress >> rxFunctionCode
                >> rxOutputAddress >> rxOutputValue;

        // Control values of the fields.
        if ( rxTransactionId == transactionId && rxProtocolId == 0 && rxLength == 6 &&
             rxDeviceAddress == deviceAddress && rxFunctionCode == 0x05 && rxOutputAddress == outputAddress &&
             ( ( outputValue && rxOutputValue == 0xFF00 ) || ( !outputValue && rxOutputValue == 0x0000 ) ) )
        {
            // Ok, done.
            if ( status ) *status = Ok;
            return true;
        }
        else
        {
            if ( status ) *status = UnknownError;
        }
    }
    else
    {
        // What was wrong ?
        if ( pdu.size() == 9 )
        {
            if ( status ) *status = pdu[8];
        }
        else
        {
            if ( status ) *status = Timeout;
        }
    }

    return false;
}

bool QTcpModbus::writeSingleRegister( const quint8 deviceAddress , const quint16 outputAddress ,
                                      const quint16 registerValue , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isConnected() )
    {
        if ( status ) *status = NoConnection;
        return false;
    }

    // Create a transaction number.
    quint16 transactionId = qrand();

    // Create modbus write single coil pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << transactionId << (quint16)0 << (quint16)6
              << deviceAddress << (quint8)0x06 << outputAddress << registerValue;

    // Clear the RX buffer before making the request.
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->readAll();

    // Send the pdu.
    socket->write( pdu );

    // Await response.
    pdu.clear();
    while ( pdu.size() < 12 )
    {
        if ( ( pdu.size() >= 9 && pdu[7] & 0x80 ) || !socket->waitForReadyRead( _timeout ) ) break;
        pdu += socket->read( 12 - pdu.size() );
    }

    // Check data and return them on success.
    if ( pdu.size() == 12 )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint16 rxTransactionId , rxProtocolId, rxLength , rxOutputAddress , rxRegisterValue;
        quint8 rxDeviceAddress , rxFunctionCode;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxTransactionId >> rxProtocolId >> rxLength >> rxDeviceAddress >> rxFunctionCode
                >> rxOutputAddress >> rxRegisterValue;

        // Control values of the fields.
        if ( rxTransactionId == transactionId && rxProtocolId == 0 && rxLength == 6 &&
             rxDeviceAddress == deviceAddress && rxFunctionCode == 0x06 && rxOutputAddress == outputAddress &&
             rxRegisterValue == registerValue )
        {
            // Ok, done.
            if ( status ) *status = Ok;
            return true;
        }
        else
        {
            if ( status ) *status = UnknownError;
        }
    }
    else
    {
        // What was wrong ?
        if ( pdu.size() == 9 )
        {
            if ( status ) *status = pdu[8];
        }
        else
        {
            if ( status ) *status = Timeout;
        }
    }

    return false;
}

bool QTcpModbus::writeMultipleCoils( const quint8 deviceAddress , const quint16 startingAddress ,
                                     const QList<bool> & outputValues , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isConnected() )
    {
        if ( status ) *status = NoConnection;
        return false;
    }

    // Create a transaction number.
    quint16 transactionId = qrand();

    // Create modbus write multiple coil pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    quint8 txBytes = outputValues.count() / 8;
    if ( outputValues.count() % 8 != 0 ) txBytes++;
    pduStream << transactionId << (quint16)0 << (quint16)( txBytes + 7 )
              << deviceAddress << (quint8)0x0F << startingAddress << (quint16)outputValues.count() << txBytes;

    // Encode the binary values.
    quint8 tmp = 0;
    for ( int i = 0 ; i < outputValues.count() ; i++ )
    {
        if ( i % 8 == 0 )
        {
            if ( i != 0 ) pduStream << tmp;
            tmp = 0;
        }
        if ( outputValues[i] ) tmp |= 0x01 << ( i % 8 );
    }
    pduStream << tmp;

    // Clear the RX buffer before making the request.
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->readAll();

    // Send the pdu.
    socket->write( pdu );

    // Await response.
    pdu.clear();
    while ( pdu.size() < 12 )
    {
        if ( ( pdu.size() >= 9 && pdu[7] & 0x80 ) || !socket->waitForReadyRead( _timeout ) ) break;
        pdu += socket->read( 12 - pdu.size() );
    }

    // Check data and return them on success.
    if ( pdu.size() == 12 )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint16 rxTransactionId , rxProtocolId, rxLength , rxStartingAddress , rxQuantityOfOutputs;
        quint8 rxDeviceAddress , rxFunctionCode;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxTransactionId >> rxProtocolId >> rxLength >> rxDeviceAddress >> rxFunctionCode
                >> rxStartingAddress >> rxQuantityOfOutputs;

        // Control values of the fields.
        if ( rxTransactionId == transactionId && rxProtocolId == 0 && rxLength == 6 &&
             rxDeviceAddress == deviceAddress && rxFunctionCode == 0x0F && rxStartingAddress == startingAddress &&
             rxQuantityOfOutputs == outputValues.count() )
        {
            // Ok, done.
            if ( status ) *status = Ok;
            return true;
        }
        else
        {
            if ( status ) *status = UnknownError;
        }
    }
    else
    {
        // What was wrong ?
        if ( pdu.size() == 9 )
        {
            if ( status ) *status = pdu[8];
        }
        else
        {
            if ( status ) *status = Timeout;
        }
    }

    return false;
}

bool QTcpModbus::writeMultipleRegisters( const quint16 rxQuantityOfRegisters ,
                                         const quint16 rxTransactionId ,const quint8 rxDeviceAddress,const quint8 rxFunctionCode,
                                         const QList<quint16> & registersValues ,
                                         bool  ret) const
{
    QByteArray pdu;
    if(ret && registersValues.count() >= rxQuantityOfRegisters){
        pdu.clear();
        QDataStream pduStream( &pdu , QIODevice::WriteOnly );
        pduStream.setByteOrder( QDataStream::BigEndian );
        quint8 txBytes = rxQuantityOfRegisters*2;

        pduStream << rxTransactionId << (quint16)0 << (quint16)( txBytes + 3 )
                  << rxDeviceAddress << (quint8)rxFunctionCode  << (quint8)txBytes;

        // Encode the register values.
        for ( int i = 0 ; i <  rxQuantityOfRegisters ; i++ )
        {
            pduStream << registersValues.at(i);
        }

        // Clear the RX buffer before making the request.


        // Send the pdu.
        reinterpret_cast<QTcpSocket*>(sender())->write( pdu );
        reinterpret_cast<QTcpSocket*>(sender())->flush();
    }

    // Await response.


    return ret;
}

bool QTcpModbus::maskWriteRegister( const quint8 deviceAddress , const quint16 referenceAddress ,
                                    const quint16 andMask , const quint16 orMask , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isConnected() )
    {
        if ( status ) *status = NoConnection;
        return false;
    }

    // Create a transaction number.
    quint16 transactionId = qrand();

    // Create modbus mask write register pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << transactionId << (quint16)0 << (quint16)8
              << deviceAddress << (quint8)0x16 << referenceAddress << andMask << orMask;

    // Clear the RX buffer before making the request.
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->readAll();

    // Send the pdu.
    socket->write( pdu );

    // Await response.
    pdu.clear();
    while ( pdu.size() < 14 )
    {
        if ( ( pdu.size() >= 9 && pdu[7] & 0x80 ) || !socket->waitForReadyRead( _timeout ) ) break;
        pdu += socket->read( 14 - pdu.size() );
    }

    // Check data and return them on success.
    if ( pdu.size() == 14 )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint16 rxTransactionId , rxProtocolId, rxLength , rxReferenceAddress , rxAndMask , rxOrMask;
        quint8 rxDeviceAddress , rxFunctionCode;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxTransactionId >> rxProtocolId >> rxLength >> rxDeviceAddress >> rxFunctionCode
                >> rxReferenceAddress >> rxAndMask >> rxOrMask;

        // Control values of the fields.
        if ( rxTransactionId == transactionId && rxProtocolId == 0 && rxLength == 8 &&
             rxDeviceAddress == deviceAddress && rxFunctionCode == 0x16 &&
             rxReferenceAddress == referenceAddress && rxAndMask == andMask && rxOrMask == orMask )
        {
            // Ok, done.
            if ( status ) *status = Ok;
            return true;
        }
        else
        {
            if ( status ) *status = UnknownError;
        }
    }
    else
    {
        // What was wrong ?
        if ( pdu.size() == 9 )
        {
            if ( status ) *status = pdu[8];
        }
        else
        {
            if ( status ) *status = Timeout;
        }
    }

    return false;
}


QList<quint16> QTcpModbus::writeReadMultipleRegisters( const quint8 deviceAddress ,
                                                       const quint16 writeStartingAddress ,
                                                       const QList<quint16> & writeValues ,
                                                       const quint16 readStartingAddress ,
                                                       const quint16 quantityToRead , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isConnected() )
    {
        if ( status ) *status = NoConnection;
        return QList<quint16>();
    }

    // Create a transaction number.
    quint16 transactionId = qrand();

    // Create modbus read holding registers pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << transactionId << (quint16)0 << (quint16)( writeValues.count() * 2 + 12 )
              << deviceAddress << (quint8)0x17 << readStartingAddress << quantityToRead
              << writeStartingAddress << (quint16)writeValues.count() << (quint16)( writeValues.count() * 2 );

    // Add data.
    foreach ( quint16 reg , writeValues )
    {
        pduStream << reg;
    }

    // Clear the RX buffer before making the request.
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->readAll();

    // Send the pdu.
    socket->write( pdu );

    // Await response.
    quint16 neededRxBytes = quantityToRead * 2;
    pdu.clear();
    while ( pdu.size() < neededRxBytes + 9 )
    {
        if ( ( pdu.size() >= 9 && pdu[7] & 0x80 ) || !socket->waitForReadyRead( _timeout ) ) break;
        pdu += socket->read( neededRxBytes + 9 - pdu.size() );
    }

    // Check data and return them on success.
    if ( pdu.size() == neededRxBytes + 9 )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint16 rxTransactionId , rxProtocolId, rxLength;
        quint8 rxDeviceAddress , rxFunctionCode , byteCount;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxTransactionId >> rxProtocolId >> rxLength >> rxDeviceAddress >> rxFunctionCode >> byteCount;

        // Control values of the fields.
        if ( rxTransactionId == transactionId && rxProtocolId == 0 && rxLength == neededRxBytes + 3 &&
             rxDeviceAddress == deviceAddress && rxFunctionCode == 0x17 && byteCount == neededRxBytes )
        {
            // Convert data.
            QList<quint16> list;
            quint16 tmp;
            for ( int i = 0 ; i < quantityToRead ; i++ )
            {
                rxStream >> tmp;
                list.append( tmp );
            }
            if ( status ) *status = Ok;
            return list;
        }
        else
        {
            if ( status ) *status = UnknownError;
        }
    }
    else
    {
        // What was wrong ?
        if ( pdu.size() == 9 )
        {
            if ( status ) *status = pdu[8];
        }
        else
        {
            if ( status ) *status = Timeout;
        }
    }

    return QList<quint16>();
}

QList<quint16> QTcpModbus::readFifoQueue( const quint8 deviceAddress , const quint16 fifoPointerAddress ,
                                          quint8 *const status ) const
{
    // Are we connected ?
    if ( !isConnected() )
    {
        if ( status ) *status = NoConnection;
        return QList<quint16>();
    }

    // Create a transaction number.
    quint16 transactionId = qrand();

    // Create modbus read FIFO registers pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << transactionId << (quint16)0 << (quint16)4
              << deviceAddress << (quint8)0x18 << fifoPointerAddress;

    // Clear the RX buffer before making the request.
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->readAll();

    // Send the pdu.
    socket->write( pdu );

    // Await response.
    pdu.clear();
    if ( !socket->waitForReadyRead( _timeout ) )
    {
        if ( status ) *status = Timeout;
        return QList<quint16>();
    }
    pdu = socket->readAll();

    // Check data and return them on success.
    if ( pdu.size() >= 12 && !( pdu[7] & 0x80 ) )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint16 rxTransactionId , rxProtocolId, rxLength , byteCount , fifoCount;
        quint8 rxDeviceAddress , rxFunctionCode;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxTransactionId >> rxProtocolId >> rxLength >> rxDeviceAddress >> rxFunctionCode
                >> byteCount >> fifoCount;

        // Control values of the fields.
        if ( rxTransactionId == transactionId && rxProtocolId == 0 && rxLength == byteCount - 2 &&
             rxDeviceAddress == deviceAddress && rxFunctionCode == 0x18 && byteCount == fifoCount * 2 &&
             pdu.size() == rxLength + 6 )
        {
            // Convert data.
            QList<quint16> list;
            quint16 tmp;
            for ( int i = 0 ; i < fifoCount ; i++ )
            {
                rxStream >> tmp;
                list.append( tmp );
            }
            if ( status ) *status = Ok;
            return list;
        }
        else
        {
            if ( status ) *status = UnknownError;
        }
    }
    else
    {
        // What was wrong ?
        if ( pdu.size() == 9 )
        {
            if ( status ) *status = pdu[8];
        }
        else
        {
            if ( status ) *status = Timeout;
        }
    }

    return QList<quint16>();
}

QByteArray QTcpModbus::executeCustomFunction( const quint8 deviceAddress , const quint8 modbusFunction ,
                                              QByteArray &data , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isConnected() )
    {
        if ( status ) *status = NoConnection;
        return QByteArray();
    }

    // Create a transaction number.
    quint16 transactionId = qrand();

    // Create modbus pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << transactionId << (quint16)0 << (quint16)( data.size() + 2 )
              << deviceAddress << modbusFunction;
    pdu += data;

    // Clear the RX buffer before making the request.
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->readAll();

    // Send the pdu.
    socket->write( pdu );

    // Await response.
    pdu.clear();
    if ( !socket->waitForReadyRead( _timeout ) )
    {
        if ( status ) *status = Timeout;
        return QByteArray();
    }
    pdu = socket->readAll();

    // Check data and return them on success.
    if ( pdu.size() >= 9 && !( pdu[7] & 0x80 ) )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint16 rxTransactionId , rxProtocolId, rxLength;
        quint8 rxDeviceAddress , rxFunctionCode;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxTransactionId >> rxProtocolId >> rxLength >> rxDeviceAddress >> rxFunctionCode;

        // Control values of the fields.
        if ( rxTransactionId == transactionId && rxProtocolId == 0 && rxDeviceAddress == deviceAddress &&
             rxFunctionCode == modbusFunction && pdu.size() == rxLength + 6 )
        {
            // Convert data.
            return pdu.right( pdu.size() - 8 );
        }
        else
        {
            if ( status ) *status = UnknownError;
        }
    }
    else
    {
        // What was wrong ?
        if ( pdu.size() == 9 )
        {
            if ( status ) *status = pdu[8];
        }
        else
        {
            if ( status ) *status = Timeout;
        }
    }

    return QByteArray();
}

QByteArray QTcpModbus::executeRaw( QByteArray &data , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isConnected() )
    {
        if ( status ) *status = NoConnection;
        return QByteArray();
    }

    // Clear the RX buffer before making the request.
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->readAll();

    // Send the data.
    socket->write( data );

    // Await response.
    if ( !socket->waitForReadyRead( _timeout ) )
    {
        if ( status ) *status = Timeout;
        return QByteArray();
    }
    return socket->readAll();
}

QByteArray QTcpModbus::calculateCheckSum( QByteArray &data ) const
{
    Q_UNUSED( data );
    return QByteArray();
}
