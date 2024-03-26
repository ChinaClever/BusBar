/*
 * msgbox.cpp
 * 信息提示框
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "msgbox.h"

extern void com_setBackColour(const QString &str,QWidget *target);

bool MsgBox::critical(QWidget *parent,  const QString &str)
{
    bool ret = false;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::critical(parent,  QObject::tr("Error message"), str);
    if (reply == QMessageBox::Abort) ret = true;
    else if (reply == QMessageBox::Retry) ret = false;

    return ret;
}

bool MsgBox::information(QWidget *parent,  const QString &str)
{
    bool ret = false;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::information(parent,  QObject::tr("Message notification"), str);
    if (reply == QMessageBox::Ok) ret = true;

    return ret;
}

bool MsgBox::question(QWidget *parent,  const QString &str)
{
    bool ret = false;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(parent,  QObject::tr("Information inquiry"), str);
    if (reply == QMessageBox::Yes) ret = true;

    return ret;
}

bool MsgBox::warning(QWidget *parent,  const QString &str)
{
    bool ret = false;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(parent,  QObject::tr("Warning message"), str);
    if (reply == QMessageBox::Ok) ret = true;

    return ret;
}



QuMsgBox::QuMsgBox(QWidget *parent,QString strContext) : QMessageBox(parent)
{    
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("Prompt information"));
    com_setBackColour(tr("Prompt information"),this);
    setWindowIcon(QIcon(":/images/logo.jpg"));
    resize(600, 400);

    setIcon(QMessageBox::Question);
    setText(strContext);

    confirmBut = addButton(tr("Confirm"), QMessageBox::AcceptRole);
    cancelBut = addButton(tr("Cancel"), QMessageBox::AcceptRole);
    confirmBut->setFixedSize(60,25);
    cancelBut->setFixedSize(60,25);
    setDefaultButton(confirmBut);

    cancelBut->setFocus();
    confirmBut->setFocus();

    setStyleSheet( "QPushButton:hover{background-color:rgb(91, 237, 238);}" );
}

QuMsgBox::~QuMsgBox()
{
}

bool QuMsgBox::Exec(void)
{
    exec();
    if( clickedButton() == dynamic_cast<QAbstractButton *>(confirmBut))
        return true ;
    else if(clickedButton() == dynamic_cast<QAbstractButton *>(cancelBut))
        return false ;
    return false ;
}


WaringMsgBox::WaringMsgBox(QWidget *parent,QString strContext) : QMessageBox(parent)
{
    //    setWindowFlags(Qt::WindowStaysOnTopHint);
    //    setWindowTitle(tr("警告信息"));
    com_setBackColour(tr("Warning message"),this);
    setWindowIcon(QIcon(":/images/logo.jpg"));
    resize(500, 400);

    setIcon(QMessageBox::Warning);
    setText(strContext);

    confirmBut = addButton(tr("Confirm "), QMessageBox::AcceptRole);
    cancelBut = addButton(tr("Cancel"), QMessageBox::AcceptRole);
    confirmBut->setMinimumSize(75,29);
    cancelBut->setMinimumSize(75,29);

    setDefaultButton(confirmBut);

    confirmBut->setFocus();
    cancelBut->setFocus();
    setStyleSheet( "QPushButton:hover{background-color:rgb(91, 237, 238);}" );
}

WaringMsgBox::~WaringMsgBox()
{
}

bool WaringMsgBox::Exec(void)
{
    exec();
    if( clickedButton() == dynamic_cast<QAbstractButton *>(confirmBut))
        return true ;
    else if(clickedButton() == dynamic_cast<QAbstractButton *>(cancelBut))
        return false ;
    return false ;
}


InfoMsgBox::InfoMsgBox(QWidget *parent,QString strContext) :
    QMessageBox(parent)
{
    //    setWindowFlags(Qt::WindowStaysOnTopHint);
    //    setWindowTitle(tr("信息提示"));
    com_setBackColour(tr("Message notification"),this);
    setWindowIcon(QIcon(":/images/logo.jpg"));
    resize(500, 400);

    setIcon(QMessageBox::Information);
    setText(strContext);

    confirmBut = addButton(tr("Confirm"), QMessageBox::AcceptRole);
    setDefaultButton(confirmBut);
    confirmBut->setMinimumSize(75,29);

    confirmBut->setFocus();
    setStyleSheet( "QPushButton:hover{background-color:rgb(91, 237, 238);}" );

    //QTimer::singleShot(AutoOutTime*1000,this,SLOT(close())); 自动关闭

    exec(); /* 自动 运行*/
}

InfoMsgBox::~InfoMsgBox()
{
}


CriticalMsgBox::CriticalMsgBox(QWidget *parent,QString strContext) :
    QMessageBox(parent)
{
    //    setWindowFlags(Qt::WindowStaysOnTopHint);
    //    setWindowTitle(tr("错误提示"));
    com_setBackColour(tr("Error message"),this);
    setWindowIcon(QIcon(":/images/logo.jpg"));
    resize(500, 400);

    setIcon(QMessageBox::Critical);
    setText(strContext);

    confirmBut = addButton(tr("Confirm"), QMessageBox::AcceptRole);
    setDefaultButton(confirmBut);
    confirmBut->setMinimumSize(75,29);

    confirmBut->setFocus();
    setStyleSheet( "QPushButton:hover{background-color:rgb(91, 237, 238);}" );

    //QTimer::singleShot(AutoOutTime*1000,this,SLOT(close())); 自动关闭

    exec(); /* 自动 运行*/
}

CriticalMsgBox::~CriticalMsgBox()
{
}

