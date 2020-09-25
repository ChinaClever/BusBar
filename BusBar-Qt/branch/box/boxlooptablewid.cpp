#include "boxlooptablewid.h"
#include "ui_boxlooptablewid.h"

BoxLoopTableWid::BoxLoopTableWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoxLoopTableWid)
{
    ui->setupUi(this);

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

BoxLoopTableWid::~BoxLoopTableWid()
{
    delete ui;
}


void BoxLoopTableWid::initLine(int bus, int box)
{
    sDataPacket *shm = get_share_mem();
    mBoxData = &(shm->data[bus].box[box]);
    mDc = shm->data[bus].box[box].dc;

    mData = &(mBoxData->data);
    mEnvData = &(mBoxData->env);
    updateData();

    QTimer::singleShot(100,this,SLOT(initFunSLot())); //延时初始化
}

void BoxLoopTableWid::initFunSLot()
{
    timer = new QTimer(this);
    timer->start(2500);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}



/**
 * @brief 初始化窗口
 */
void BoxLoopTableWid::initTableWidget()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header <<tr("回路") <<tr("名称") <<tr("断路器") << tr("电压") << tr("电流") << tr("功率") << tr("功率因素") << tr("电能");// << tr("温度");
    ui->tableWidget->setColumnCount(header.size());    //设置列数
    ui->tableWidget->setHorizontalHeaderLabels(header);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //关闭横向滑条




    // ui->tableWidget->setColumnHidden(0, true); // 隐藏母线名称
    // ui->tableWidget->setColumnWidth(0,200);
}


/**
 * @brief 增加Item
 * @param row 行
 * @param column 列
 * @param content 信息
 */
void BoxLoopTableWid::addItemContent(int row, int column, const QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    item->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(row, column, item);
}

void BoxLoopTableWid::addRowContent(QStringList &list)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    // ui->tableWidget->setRowHeight(row, 40);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);

    list.clear();
}

/**
 * @brief 初始化一行表格
 * @param str
 */
void BoxLoopTableWid::initTable()
{
    QStringList list;
    int column = ui->tableWidget->columnCount();
    for(int j=0; j<column; ++j)
        list << "---";
    addRowContent(list);
}


/**
 * @brief 初始化表格窗口
 */
void BoxLoopTableWid::initTableWid()
{
    initTableWidget();
    //checkTable();

    for(int i=0; i<mData->lineNum; ++i)
        initTable();
}

/**
 * @brief 表格检查 当输出位与表格行数不相等时，需要重新建立表
 * @return true 表示需要重新建立表格
 */
bool BoxLoopTableWid::checkTable()
{
    bool ret = false;

#if 1
    int size = mData->lineNum;
    if(size < 0 || size > 9) {
        mData->lineNum = size = 3;
    }
#else
    int size = LINE_NUM;
#endif

    int row = ui->tableWidget->rowCount();
    if(size != row)
        ret = true;

    return ret;
}

void BoxLoopTableWid::setTableItem(int id, int column, const QString &str)
{
    QTableWidgetItem *item = ui->tableWidget->item(id, column);
    item->setText(str);
}

/**
 * @brief 清空表格
 */
void BoxLoopTableWid::clearTable()
{
    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();

    for(int i=0; i<row; ++i)
    {
        for(int j=0; j<column; ++j)
            setTableItem(i, j, "---");
    }
}


void BoxLoopTableWid::setLoop(int id, int column)
{
    QString name;
    if(mDc){
        name = QString((char)('A' + id%3))+ QString("%1").arg(id/3 + 1);
    }else{
        name = "D" + QString("%1").arg(id + 1);
    }

    setTableItem(id, column, name);
}

void BoxLoopTableWid::setName(int id, int column)
{
    QString name = mBoxData->loopName[id];
    if(!name.isEmpty())
    setTableItem(id, column, name);
}

void BoxLoopTableWid::setAlarmStatus(int id, int column, int alarm, int crAlarm)
{
    QTableWidgetItem *item = ui->tableWidget->item(id, column);

    if(alarm) { // 告警
        item->setTextColor(QColor(Qt::red));
    } else  if(crAlarm) { // 预警
        item->setTextColor(QColor("#CD7E80"));
    } else {
        item->setTextColor(QColor(Qt::black));
    }
}

void BoxLoopTableWid::setVol(int id, int column)
{
    QString str = "---";
    sDataUnit *unit = &(mData->vol);

    double value = unit->value[id] / COM_RATE_VOL;
    if(value >= 0)
        str = QString::number(value) + "V";

    setTableItem(id, column, str);
    setAlarmStatus(id, column,unit->alarm[id], unit->crAlarm[id]);
}

void BoxLoopTableWid::setSw(int id, int column)
{
    QString str = "---";

    int sw = mData->sw[id];
    if(sw)
        str = "闭合";
    else
        str = "断开";

    setTableItem(id, column, str);
    setAlarmStatus(id, column,1-sw, 0);
}

void BoxLoopTableWid::setCur(int id, int column)
{
    QString str = "---";
    sDataUnit *unit = &(mData->cur);

    double value = unit->value[id] / COM_RATE_CUR;
    if(value >= 0)
        str = QString::number(value, 'f', 1) + "A";
    setTableItem(id, column, str);
    setAlarmStatus(id, column,unit->alarm[id], unit->crAlarm[id]);
}

void BoxLoopTableWid::setPow(int id, int column)
{
    QString str = "---";

    double value = mData->pow[id] / COM_RATE_POW;
    if(value >= 0)
        str = QString::number(value, 'f', 3) + "kW";
    setTableItem(id, column, str);
}

void BoxLoopTableWid::setPf(int id, int column)
{
    QString str = "---";

    double value = mData->pf[id] / COM_RATE_PF;
    if(value >= 0)
        str = QString::number(value, 'f', 2);
    setTableItem(id, column, str);
}

void BoxLoopTableWid::setEle(int id, int column)
{
    QString str = "---";

    double value = mData->ele[id] / COM_RATE_ELE;
    if(value >= 0)
        str = QString::number(value, 'f', 1) + "kWh";
    setTableItem(id, column, str);
}

void BoxLoopTableWid::setTemp(int id, int column)
{
    QString str = "---";
    sDataUnit *unit = &(mEnvData->tem);

    double value = unit->value[id]/COM_RATE_TEM;
    if(value >= 0)
        str = QString::number(value) + "°C";
    setTableItem(id, column, str);

    setAlarmStatus(id, column,unit->alarm[id], unit->crAlarm[id]);
}

/**
 * @brief 数据更新入口函数
 */
void BoxLoopTableWid::updateData()
{
    bool ret = checkTable();
    if(ret) initTableWid(); // 重新建立表格

    if(mBoxData->offLine)
    {
        int row = ui->tableWidget->rowCount();
        for(int i=0; i<row; ++i)
        {
            int k=0;
            setLoop(i, k++);
            setName(i, k++); // 设置输出位名称

            setSw(i, k++);
            setVol(i, k++); // 设置电压
            setCur(i, k++); // 设置电流值

            setPow(i, k++); // 功率
            setPf(i, k++); // 功率因素
            setEle(i, k++);
           // setTemp(i, k++); //温度
        }
    } else {
        clearTable();
    }
}

void BoxLoopTableWid::timeoutDone()
{
    updateData();
}
