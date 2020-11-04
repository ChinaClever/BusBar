/*
 * logbranchelewid.cpp
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "logbranchelewid.h"
#include "ui_logbranchelewid.h"
#include "dbbranchele.h"
#include "logbrancheleexportdlg.h"

LogBranchEleWid::LogBranchEleWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogBranchEleWid)
{
    ui->setupUi(this);
    QTimer::singleShot(100,this,SLOT(initFunSLot())); //延时初始化
    mCount = 0;
    initScrollArea(); // 开启滑动功能
}

LogBranchEleWid::~LogBranchEleWid()
{
    delete ui;
}

void LogBranchEleWid::initScrollArea()
{
    m_scrollBarV = ui->tableView->verticalScrollBar();
    QObjectList objectList = ui->tableView->children();
    for(int i = 0; i < objectList.count(); i++) {
        if(objectList.at(i)->objectName() == "qt_scrollarea_viewport") {
            objectList.at(i)->installEventFilter(this);
        }
    }
}

bool LogBranchEleWid::eventFilter(QObject *obj, QEvent *event)
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
            animation->setEasingCurve(QEasingCurve::OutQuad);
            animation->start();
            return true;
        }
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


void LogBranchEleWid::initFunSLot()
{
    for(int i=0; i<BUS_NUM; ++i) getTableName(i);

    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);//
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁用编辑功能

    model = new SqlTableModel(ui->tableView);
    ui->tableView->setModel(model->model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleSlot(QModelIndex)));

    initBtnBar();
    initTableSlot(0);
}

void LogBranchEleWid::initBtnBar()
{
    mBtnBar = new LogBtnBar(ui->widget);
    //    connect(mBtnBar, SIGNAL(busNumSig(int)), this, SLOT(initTableSlot(int)));
    connect(mBtnBar,SIGNAL(querySig(QString)),model,SLOT(queryFilter(QString)));
    connect(mBtnBar,SIGNAL(clearSig()),this,SLOT(clearTableSlot()));
    connect(mBtnBar,SIGNAL(refreshSig()),this,SLOT(refreshSlot()));
    connect(mBtnBar,SIGNAL(exportSig(int)),this,SLOT(exportSlot(int)));
}

QString LogBranchEleWid::getTableName(int id)
{
    return db_branchEle_obj(id)->tableName();
}

/**
 * @brief 隐藏列
 */
void LogBranchEleWid::updateColumnHidden()
{
    switch (LINE_NUM) {
    case 3:
        ui->tableView->setColumnHidden(7, true);
        ui->tableView->setColumnHidden(8, true);
        ui->tableView->setColumnHidden(9, true);
    case 6:
        ui->tableView->setColumnHidden(10, true);
        ui->tableView->setColumnHidden(11, true);
        ui->tableView->setColumnHidden(12, true);
        break;
    default:
        break;
    }
    ui->tableView->sortByColumn(0, Qt::DescendingOrder); // 降序排列
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(13, true);
    //ui->tableView->resizeColumnsToContents();
}

void LogBranchEleWid::initTableSlot(int id)
{
    mid = id;
    m_table = getTableName(id);
    this->refreshTable(m_table);

    mHeadList << tr("编号") << tr("日期") << tr("时间") << tr("插接箱");
    /* for(int i=0; i<9; ++i)
        mHeadList << "L" + QString::number(i+1);*/
    //--------------------[由于界面不好修改——By_MW 2018.3.21]-----------
    mHeadList << tr("A1/D1") << tr("B1/D2") << tr("C1/D3") << tr("A2/D4") << tr("B2")
              << tr("C2") << tr("A3") << tr("B3") << tr("C3");
    //---------------------------------------------------------------
    mHeadList << tr("合计");
    model->setHeaders(mHeadList);
}


bool LogBranchEleWid::refreshTable(const QString &table)
{
    bool ret = model->refreshTable(table);
    if(ret) {
        m_table = table;
        updateColumnHidden();
    }
    return  ret;
}

void LogBranchEleWid::clearTableSlot()
{
#if SQL_DEL_MODE

    int row = model->model->rowCount();
    DbBranchEle* db = db_branchEle_obj(mid);
    if(mCount++ % 2 ==0)
    {
        model->model->setTable("markingtable");
        db->clear();
        QTimer::singleShot(row*8,this,SLOT(clearTableSlot()));
    } else {
        db->createTable();
        initTableSlot(mid);
    }
#else
    if(model->removeRow(0))
        QTimer::singleShot(1,this,SLOT(clearTableSlot()));
#endif
}

void LogBranchEleWid::refreshSlot()
{
    initTableSlot(mid);
}

void LogBranchEleWid::doubleSlot(QModelIndex)
{
    QString str = tr("是否删除这条记录?");
    QuMsgBox box(this, str);
    bool ret = box.Exec();
    if(ret)
    {
        int curRow = ui->tableView->currentIndex().row();
        model->removeRow(curRow);
    }
}


/**
 * @brief 记录导出 1日报表 ，2月报表  3 年报表
 */
void LogBranchEleWid::exportSlot(int mode)
{
    if(mode == 0) {
        LogBranchEleExportDlg dlg(this);
        dlg.set(mid);
        dlg.exec();
    }
}
