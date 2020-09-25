#include "setnameswid.h"
#include "ui_setnameswid.h"
extern void set_box_num(int id, int num);

SetNamesWid::SetNamesWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetNamesWid)
{
    ui->setupUi(this);
    mIndex = 0;
    mSetShm = new SetShm;
    mSetNameDlg = new SetNameDlg(this);
    mIndex = 0;
    QTimer::singleShot(100,this,SLOT(initFunSLot()));
}

void SetNamesWid::initFunSLot()
{
    indexChanged(mIndex);
    mTimer = new QTimer(this);
    mTimer->start(3*1000);
    connect(mTimer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}


SetNamesWid::~SetNamesWid()
{
    delete ui;
}


void SetNamesWid::initTableWidget()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);

    QStringList horHead;
    horHead<< tr("插接箱");

    int dc = mPacket ? mPacket->box[0].dc : 1;
    if(dc){ //交流9个
        for(int i = 0; i < LINE_NUM; ++i)
            horHead << QString((char)('A' + i%3))+ QString("%1").arg(i/3 + 1);
    }else{ //直流4个
        for(int i = 0; i < 4; i++)
            horHead << "D" + QString("%1").arg(i + 1);
    }

    ui->tableWidget->setColumnCount(horHead.size());
    ui->tableWidget->setHorizontalHeaderLabels(horHead);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可编辑
    //    ui->tableWidget->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(itemDoubleClicked(QTableWidgetItem*)));
}


void SetNamesWid::clearWidget()
{
    int row = ui->tableWidget->rowCount();
    for(int i = 0 ; i < row ; i++)
        ui->tableWidget->removeRow(0);
}

void SetNamesWid::resetWidget()
{
    initTableWidget();
    int boxNum = mPacket->boxNum;

    for(int i = 0 ;  i < boxNum ; i++)
    {
        ui->tableWidget->insertRow(i);
        int len = LINE_NUM; //交流9个
        if(!mPacket->box[0].dc) len = 4;

        for(int j=0; j<=len; j++){
            QTableWidgetItem * item = new QTableWidgetItem("---");
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(i, j ,item);
        }
    }
}


void SetNamesWid::checkBus()
{
    int row = ui->tableWidget->rowCount();
    int col = ui->tableWidget->columnCount();

    int dc = mPacket ? mPacket->box[0].dc : 1;
    int len = dc ? LINE_NUM : 4;
    if(mPacket->boxNum != row || col != len+1) { //修改判断条件——  2018.3.21——By>MW
        clearWidget();
        resetWidget();
    }
}

/**
 * @brief 刷新界面
 * @param index 主路源编号
 */
void SetNamesWid::indexChanged(int index)
{
//    if(mIndex == index)  return;

    mIndex = index;
    mPacket = &(get_share_mem()->data[index]);
    initWid(index);
}

void SetNamesWid::updateWid()
{
    checkBus();

    int row = ui->tableWidget->rowCount();
    for(int i = 0 ; i < row ; i++)
    {
        setName(i,0);
        for(int j=1; j<ui->tableWidget->columnCount(); ++j) {
            setTableItem(i, j);
        }
    }
}

void SetNamesWid::timeoutDone()
{
    updateWid();
}


void SetNamesWid::setName(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    QString str = mPacket->box[row+1].boxName;  //第0个为始端箱，所以从第一个开始
    item->setText(str);
}


void SetNamesWid::setTableItem(int row, int column)
{
    QString str = "---";
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    sBoxData *box = &(mPacket->box[row+1]);

    //box->rate 直流的情况下，box->rate代表路数
    if(box->offLine > 0 /* && column <= box->rate */) {
        if(column <= box->loopNum) {
           str = box->loopName[column-1];
        }
    }
    if(!str.isEmpty())
    item->setText(str);
}


void SetNamesWid::itemDoubleClicked(QTableWidgetItem *item)
{
    if(item->text().compare("---") == 0) return;  //为空不设置
    disconnect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(itemDoubleClicked(QTableWidgetItem*)));
    int boxNum = item->row() + 1 ;
    int column = item->column();
    mSetNameDlg->init(mIndex, boxNum, column, item->text());
    mSetNameDlg->show();

    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(itemDoubleClicked(QTableWidgetItem*)));
}

void SetNamesWid::initWid(int index)
{
    sBusData *busData = &(get_share_mem()->data[index]);
    ui->nameEdit->setText(busData->busName);
    ui->boxNumSpin->setValue(busData->boxNum);

    double rateCur = busData->box[0].ratedCur/COM_RATE_CUR;
    ui->rateCurSpin->setValue(rateCur);

    checkBus();//切换通道，更新表格名称  2018-12-20 pmd
    int row = ui->tableWidget->rowCount();
    for(int i = 0 ; i < row ; i++)
    {
        setName(i,0);
        for(int j=1; j<ui->tableWidget->columnCount(); ++j) {
            setTableItem(i, j);
        }
    }
}

bool SetNamesWid::saveBusName()
{
    bool ret = true;
    DbNameItem item;
    item.bus = mIndex;
    item.type = 1; // 名称类型 1 母线名称   2 插接箱名称  3 回路名称
    item.num = 0; // 编号
    QString name = ui->nameEdit->text();
    if( (!name.isEmpty()) && (!(name.size() > NAME_LEN))) {
        item.name = name;
        mSetShm->setName(item);
    }else {
        CriticalMsgBox box(this, tr("母线名称保存失败!!"));
        ret = false;
    }
    return ret;
}


void SetNamesWid::on_saveBtn_clicked()
{
    mSetShm->setLineRatedCur(mIndex,ui->rateCurSpin->value() * COM_RATE_CUR);
    mSetShm->setLineBoxNum(mIndex, ui->boxNumSpin->value());
    if(saveBusName()) {
        set_box_num(mIndex, ui->boxNumSpin->value());
        updateWid();                               //2018-12-17保存插接箱数量的同时，更新名称设置列表 pmd

        BeepThread::bulid()->beep();
        InfoMsgBox box(this, tr("保存成功！"));
    }
}

