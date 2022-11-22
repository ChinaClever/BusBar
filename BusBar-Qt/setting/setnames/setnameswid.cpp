#include "setnameswid.h"
#include "ui_setnameswid.h"
#include "interfacechangesig.h"
extern void set_box_num(int id, int num);

SetNamesWid::SetNamesWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetNamesWid)
{
    ui->setupUi(this);
    mIndex = 0;
    mSetShm = new SetShm;
    mSetNameDlg = new SetNameDlg(this);
    QTimer::singleShot(7650,this,SLOT(initFunSLot()));
    initScrollArea(); // 开启滑动功能
    connect(InterfaceChangeSig::get(), SIGNAL(typeSig(int)), this,SLOT(interfaceChangedSlot(int)));
    isRun = false;
}

void SetNamesWid::interfaceChangedSlot(int id)
{
    if(id == 5) {
        isRun = true;
    } else {
        isRun = false;
    }
}

void SetNamesWid::initScrollArea()
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

bool SetNamesWid::eventFilter(QObject *obj, QEvent *event)
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

            if(m_scrollBarH != NULL)
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
    if(isRun){
        updateWid();
    }
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
    mSetNameDlg->move(0,0);

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

