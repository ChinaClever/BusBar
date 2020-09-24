/*
 * logmainelewid.cpp
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "logmainelewid.h"
#include "ui_logmainelewid.h"
#include "dbmainele.h"
#include "logmaineleexportdlg.h"

LogMainEleWid::LogMainEleWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogMainEleWid)
{
    ui->setupUi(this);
    QTimer::singleShot(100,this,SLOT(initFunSLot())); //延时初始化
    mCount = 0;
}

LogMainEleWid::~LogMainEleWid()
{
    delete ui;
}

void LogMainEleWid::initFunSLot()
{
    for(int i=0; i<BUS_NUM; ++i) getTableName(i); //初始化所有数据库对象

    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);//
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁用编辑功能

    model = new SqlTableModel(ui->tableView);
    ui->tableView->setModel(model->model); //关联表
    ui->tableView->horizontalHeader()->setStretchLastSection(true); //横头 -- 设置最后一节
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleSlot(QModelIndex))); //双击信号

    initBtnBar();
    initTableSlot(0);
}

void LogMainEleWid::initBtnBar()
{
    mBtnBar = new LogBtnBar(ui->widget);
//    connect(mBtnBar, SIGNAL(busNumSig(int)), this, SLOT(initTableSlot(int)));
    connect(mBtnBar,SIGNAL(querySig(QString)),model,SLOT(queryFilter(QString)));
    connect(mBtnBar,SIGNAL(clearSig()),this,SLOT(clearTableSlot()));
    connect(mBtnBar,SIGNAL(refreshSig()),this,SLOT(refreshSlot())); //刷新
    connect(mBtnBar,SIGNAL(exportSig(int)),this,SLOT(exportSlot(int)));
}

QString LogMainEleWid::getTableName(int id)
{
    return db_mainEle_obj(id)->tableName();
}

void LogMainEleWid::initTableSlot(int id)
{
    mid = id;
    m_table = getTableName(id);
    this->refreshTable(m_table);

    mHeadList << tr("编号") << tr("日期") << tr("时间") << tr("A")<< tr("B") << tr("C") << tr("合相");
    model->setHeaders(mHeadList);
}


bool LogMainEleWid::refreshTable(const QString &table)
{
    BeepThread::bulid()->beep();
    bool ret = model->refreshTable(table);
    if(ret) {
        m_table = table;
        ui->tableView->sortByColumn(0, Qt::DescendingOrder); // 降序排列
        ui->tableView->setColumnHidden(0, true);
        ui->tableView->setColumnHidden(6, true);
    }
    return  ret;
}

void LogMainEleWid::clearTableSlot()
{
#if SQL_DEL_MODE

    int row = model->model->rowCount();
    DbMainEle* db = db_mainEle_obj(mid);
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

void LogMainEleWid::refreshSlot()
{
    initTableSlot(mid);
}

void LogMainEleWid::doubleSlot(QModelIndex)
{
    BeepThread::bulid()->beep();
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
 * @brief 记录导出 0按时间段导出 1日报表 ，2月报表  3 年报表
 */
void LogMainEleWid::exportSlot(int mode)
{
   if(mode == 0) {
       LogMainEleExportDlg dlg(this);
       dlg.set(mid);
       dlg.exec();
   }
}
