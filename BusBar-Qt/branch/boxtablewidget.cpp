#include "boxtablewidget.h"
#include "ui_boxtablewidget.h"
#include "box/boxdlg.h"
#include "interfacechangesig.h"
#include "beepthread.h"

BoxTableWidget::BoxTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoxTableWidget)
{
    ui->setupUi(this);
    QTimer::singleShot(1000,this,SLOT(initFunSLot())); //延时初始化
    initScrollArea(); // 开启滑动功能
}

BoxTableWidget::~BoxTableWidget()
{
    delete ui;
}

void BoxTableWidget::initScrollArea()
{
    m_scrollBarV = ui->tableWidget->verticalScrollBar();
    m_scrollBarH = ui->tableWidget->horizontalScrollBar();
    QObjectList objectList = ui->tableWidget->children();
    for(int i = 0; i < objectList.count(); i++) {
        if(objectList.at(i)->objectName() == "qt_scrollarea_viewport") {
            objectList.at(i)->installEventFilter(this);
        }
    }
}

bool BoxTableWidget::eventFilter(QObject *obj, QEvent *event)
{
    static int press_y   = 0;
    static int move_y    = -1;
    static int release_y = 0;
    static QDateTime pressDateTime;
    static QPropertyAnimation *animation = new QPropertyAnimation();
    if("qt_scrollarea_viewport" != obj->objectName()) return false;

    int scrollV_max = m_scrollBarV->maximum();
    int scrollV_min = m_scrollBarV->minimum();

    //根据鼠标的动作——按下、放开、拖动，执行相应的操作
    if(event->type() == QEvent::MouseButtonPress) {  //记录按下的时间、坐标
        pressDateTime = QDateTime::currentDateTime();
        move_y  = QCursor::pos().y();
        press_y = move_y;
        animation->stop();
    } else if(event->type() == QEvent::MouseButtonRelease) {
        //鼠标放开，根据鼠标拖动的垂直距离和持续时间，设置窗口滚动快慢程度和距离
        if(animation->targetObject() != m_scrollBarV) {
            animation->setTargetObject(m_scrollBarV);
            animation->setPropertyName("value");
        }

        move_y = -1;
        release_y = QCursor::pos().y();
        QObject *parent_obj = obj->parent();
        if(parent_obj != 0 || parent_obj->inherits("QAbstractItemView")) {
            QTimer::singleShot(150, (QAbstractItemView *)parent_obj, SLOT(clearSelection()));
        }

        int endValue = 0;
        int pageStep;
        if(release_y - press_y != 0 && qAbs(release_y - press_y) > 45) {
            int mseconds = pressDateTime.msecsTo(QDateTime::currentDateTime());
            int limit = 440;
            pageStep = 240;//scrollBarV->pageStep();
            if(mseconds > limit) { //滑动的时间大于某个值的时候，不再滚动(通过增加分母)
                mseconds = mseconds + (mseconds - limit) * 20;
            }

            if(release_y - press_y > 0) {//.0避免避免强制转换为整形
                endValue = m_scrollBarV->value() - pageStep * (200.0 / mseconds);
                if(scrollV_min > endValue) endValue = scrollV_min;
            } else if(release_y - press_y < 0) {
                endValue = m_scrollBarV->value() + pageStep * (200.0 / mseconds);
                if(endValue > scrollV_max) endValue = scrollV_max;
            }
            if(mseconds > limit) mseconds = 0;//滑动的时间大于某个值的时候，滚动距离变小，减小滑动的时间
            animation->setDuration(mseconds+550);
            animation->setEndValue(endValue);

            if( m_scrollBarH != NULL)
                m_scrollBarH->setValue(0);
            animation->setEasingCurve(QEasingCurve::OutQuad);
            animation->start();
            return true;
        }
        if(m_scrollBarH != NULL)
            m_scrollBarH->setValue(0);
    } else if(event->type() == QEvent::MouseMove && move_y >= 0) {   //窗口跟着鼠标移动
        int move_distance = QCursor::pos().y() - move_y;
        int endValue = m_scrollBarV->value() - move_distance;
        if(scrollV_min > endValue) endValue = scrollV_min;
        if(endValue > scrollV_max)  endValue = scrollV_max;
        m_scrollBarV->setValue(endValue);
        move_y = QCursor::pos().y();
    }

    return false;
}

void BoxTableWidget::initFunSLot()
{
    busChangeSlot(0);
    timer = new QTimer(this);
    timer->start(1500);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    connect(InterfaceChangeSig::get(), SIGNAL(typeSig(int)), this,SLOT(interfaceChangedSlot(int)));
    isRun = false;
}


void BoxTableWidget::busChangeSlot(int id)
{
    mBusID = id;
    sDataPacket *shm = get_share_mem();
    mBoxData = &(shm->data[id]);
    updateData();
}


/**
 * @brief 初始化窗口
 */
void BoxTableWidget::initTableWidget()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header <<tr("插接箱") << tr("状态") << tr("A电流") << tr("A电能") << tr("B电流") << tr("B电能") << tr("C电流")   << tr("C电能");
    ui->tableWidget->setColumnCount(header.size());    //设置列数
    ui->tableWidget->setHorizontalHeaderLabels(header);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(getItem(QTableWidgetItem*)));
    // ui->tableWidget->setColumnHidden(0, true); // 隐藏母线名称
    // ui->tableWidget->setColumnWidth(0,200);
}


/**
 * @brief 增加Item
 * @param row 行
 * @param column 列
 * @param content 信息
 */
void BoxTableWidget::addItemContent(int row, int column, const QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    if(column > 0)
        item->setTextAlignment(Qt::AlignCenter);
    else
        item->setTextAlignment(Qt::AlignVCenter);

    ui->tableWidget->setItem(row, column, item);
}

void BoxTableWidget::addRowContent(QStringList &list)
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
void BoxTableWidget::initTable()
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
void BoxTableWidget::initTableWid()
{
    initTableWidget();

    int size = mBoxData->boxNum; // 获取插接箱的数量
    for(int i=0; i<size; ++i)
        initTable();
}

/**
 * @brief 表格检查 当输出位与表格行数不相等时，需要重新建立表
 * @return true 表示需要重新建立表格
 */
bool BoxTableWidget::checkTable()
{
    bool ret = false;

    int size = mBoxData->boxNum;
    int row = ui->tableWidget->rowCount();
    if(size != row)
        ret = true;

    return ret;
}


void BoxTableWidget::setTableItem(int id, int column, const QString &str)
{
    QTableWidgetItem *item = ui->tableWidget->item(id-1, column);
    item->setText(str);
}

/**
 * @brief 清空表格
 */
void BoxTableWidget::clearTable()
{
    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();

    for(int i=1; i<=row; ++i)
    {
        for(int j=0; j<column; ++j)
            setTableItem(i, j, "---");
    }
}

void BoxTableWidget::setName(int id, int column)
{
    QString name = mBoxData->box[id].boxName;
    if(name.isEmpty())
        name = tr("iBox-%1").arg(id);
    setTableItem(id, column, name);
}



void BoxTableWidget::setAlarmStatus(int id, int column)
{
    QString str;
    QTableWidgetItem *item = ui->tableWidget->item(id-1, column);
    if(mBoxData->box[id].offLine)
    {
        int curAlarm = mBoxData->box[id].boxCurAlarm;
        int volAlarm = mBoxData->box[id].boxVolAlarm;
        if((curAlarm == 2)  || (volAlarm == 2)) { // 告警
            str = tr("告警");
            item->setTextColor(QColor(Qt::red));
        } else  if((curAlarm == 1)  || (volAlarm == 1))  { // 预警
            str = tr("预警");
            item->setTextColor(QColor("#CD7E80"));
        } else {
            str = tr("正常");
            item->setTextColor(QColor(Qt::black));
        }
    } else {
        str = tr("离线");
        item->setTextColor(QColor(Qt::red));
    }
    item->setText(str);
}


void BoxTableWidget::setCur(int id, int line, int column)
{
    QString str = "---";
    if(mBoxData->box[id].offLine)
    {
        sLineTgObjData *unit = &(mBoxData->box[id].lineTgBox);
        double value = unit->cur[line] / COM_RATE_CUR;
        if(value >= 0)
            //            str = QString::number(value, 'f', 1) + "A";
            str = QString::number(value, 'f', 2) + "A";
    }
    setTableItem(id, column, str);
}


void BoxTableWidget::setEle(int id, int line, int column)
{
    QString str = "---";

    if(mBoxData->box[id].offLine)
    {
        sLineTgObjData *unit = &(mBoxData->box[id].lineTgBox);
        double value = unit->ele[line] / COM_RATE_ELE;
        if(value >= 0)
            str = QString::number(value, 'f', 1) + "kWh";
    }
    setTableItem(id, column, str);
}


/**
 * @brief 数据更新入口函数
 */
void BoxTableWidget::updateData()
{
    bool ret = checkTable();
    if(ret)
        initTableWid(); // 重新建立表格

    int row = ui->tableWidget->rowCount();
    for(int i=1; i<=row; ++i) //更新数据
    {
        int k=0;
        setName(i, k++); // 设置输出位名称
        setAlarmStatus(i, k++); //设置告警状态

        for(int j=0; j<3; ++j) {
            setCur(i, j, k++); // 设置电流值
            setEle(i, j, k++);
        }
    }
}


void BoxTableWidget::interfaceChangedSlot(int id)
{
    if(id == 3) {
        isRun = true;
    } else {
        isRun = false;
    }
}

void BoxTableWidget::timeoutDone()
{
    if(isRun) {
        updateData();
    }
}

void BoxTableWidget::getItem(QTableWidgetItem*)
{
    disconnect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(getItem(QTableWidgetItem*)));
    BeepThread::bulid()->beep();
    int row = ui->tableWidget->currentRow();

    BoxDlg dlg(0);
    dlg.initBox(mBusID, row+1);
    dlg.exec();
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(getItem(QTableWidgetItem*)));
}
