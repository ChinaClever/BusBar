/*
 * logalarmwid.cpp
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "logalarmwid.h"
#include "ui_logalarmwid.h"
#include "dbalarm.h"
#include "logalarmeleexportdlg.h"

LogAlarmWid::LogAlarmWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogAlarmWid)
{
    ui->setupUi(this);
    QTimer::singleShot(100,this,SLOT(initFunSLot())); //延时初始化
    mCount = 0;
}

LogAlarmWid::~LogAlarmWid()
{
    delete ui;
}


void LogAlarmWid::initFunSLot()
{
    for(int i=0; i<BUS_NUM; ++i) getTableName(i);

    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);//
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁用编辑功能

    model = new SqlTableModel(ui->tableView);
    ui->tableView->setModel(model->model); //设置模型
    //    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleSlot(QModelIndex)));

    initBtnBar(); // 切换
    initTableSlot(0);
}

void LogAlarmWid::initBtnBar()
{
    mBtnBar = new LogBtnBar(ui->widget);
    //    connect(mBtnBar, SIGNAL(busNumSig(int)), this, SLOT(initTableSlot(int)));
    connect(mBtnBar,SIGNAL(querySig(QString)),model,SLOT(queryFilter(QString))); //条件
    connect(mBtnBar,SIGNAL(clearSig()),this,SLOT(clearTableSlot())); //清空
    connect(mBtnBar,SIGNAL(refreshSig()),this,SLOT(refreshSlot())); //刷新
    connect(mBtnBar,SIGNAL(exportSig(int)),this,SLOT(exportSlot(int))); //导出
}

QString LogAlarmWid::getTableName(int id)
{
    return db_alarm_obj(id)->tableName();
}

void LogAlarmWid::initTableSlot(int id)
{
    mid = id;
    m_table = getTableName(id);
    this->refreshTable(m_table);

    mHeadList << tr("编号") << tr("日期") << tr("时间") << tr("告警项目")<< tr("告警内容") ;
    model->setHeaders(mHeadList);
}


bool LogAlarmWid::refreshTable(const QString &table)
{
    bool ret = model->refreshTable(table); //更新数据
    if(ret) {
        m_table = table;
        ui->tableView->sortByColumn(0, Qt::DescendingOrder); // 降序排列
        ui->tableView->setColumnHidden(0, true); //隐藏列
        ui->tableView->setColumnWidth(4,750); //设置宽度
    }
    return  ret;
}

void LogAlarmWid::clearTableSlot()
{
#if SQL_DEL_MODE
    int row = model->model->rowCount();
    DbAlarm* db = db_alarm_obj(mid);
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

void LogAlarmWid::refreshSlot()
{
    initTableSlot(mid);
}

void LogAlarmWid::doubleSlot(QModelIndex)
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
void LogAlarmWid::exportSlot(int mode)
{
    if(mode == 0) {
        LogAlarmEleExportDlg dlg(this);
        dlg.set(mid);
        dlg.exec();
    }
}
