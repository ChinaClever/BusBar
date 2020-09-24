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
}

LogBranchEleWid::~LogBranchEleWid()
{
    delete ui;
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
