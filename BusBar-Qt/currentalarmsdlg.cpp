#include "currentalarmsdlg.h"
#include "ui_currentalarmsdlg.h"
#include "dpalarmslave.h"

CurrentAlarmsDlg::CurrentAlarmsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurrentAlarmsDlg)
{
    ui->setupUi(this);
    com_setBackColour(tr("当前告警内容"),this);

    QTimer::singleShot(100,this,SLOT(on_refreshBtn_clicked())); //延时初始化
    initScrollArea(); // 开启滑动功能
}

CurrentAlarmsDlg::~CurrentAlarmsDlg()
{
    delete ui;
}

void CurrentAlarmsDlg::initScrollArea()
{
    m_scrollBarV = ui->tableWidget->verticalScrollBar();
    QObjectList objectList = ui->tableWidget->children();
    for(int i = 0; i < objectList.count(); i++) {
        if(objectList.at(i)->objectName() == "qt_scrollarea_viewport") {
            objectList.at(i)->installEventFilter(this);
        }
    }
}

bool CurrentAlarmsDlg::eventFilter(QObject *obj, QEvent *event)
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

/**
 * @brief 初始化窗口
 */
void CurrentAlarmsDlg::initTableWidget()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header <<tr("母线名称")<<tr("告警类型") << tr("告警内容");
    ui->tableWidget->setColumnCount(header.size());    //设置列数
    ui->tableWidget->setHorizontalHeaderLabels(header);

//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
      ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
//    ui->tableWidget->setColumnHidden(0, true); // 隐藏母线名称
      ui->tableWidget->setColumnWidth(1,180);
}


/**
 * @brief 增加Item
 * @param row 行
 * @param column 列
 * @param content 信息
 */
void CurrentAlarmsDlg::addItemContent(int row, int column, const QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
//    if(column > 1)
//        item->setTextAlignment(Qt::AlignCenter);
//    else
        item->setTextAlignment(Qt::AlignVCenter);

    ui->tableWidget->setItem(row, column, item);
}

void CurrentAlarmsDlg::addRowContent(QStringList &list)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    // ui->tableWidget->setRowHeight(row, 40);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);

    list.clear();
}


void CurrentAlarmsDlg::updateData()
{
    QStringList alarmStr = get_alarm_str();

    QStringList list;
    for(int i=0; i<alarmStr.size(); i++) {
        list << alarmStr.at(i);
        if(list.size() == 3) {
            addRowContent(list);
        }
    }
}


void CurrentAlarmsDlg::on_refreshBtn_clicked()
{
    BeepThread::bulid()->beep();
    initTableWidget();
    updateData();
}

void CurrentAlarmsDlg::on_quitBtn_clicked()
{
    BeepThread::bulid()->beep();
    this->close();
}
