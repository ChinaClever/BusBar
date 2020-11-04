#ifndef CURRENTALARMSDLG_H
#define CURRENTALARMSDLG_H

#include <QDialog>
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QDateTime>

namespace Ui {
class CurrentAlarmsDlg;
}

class CurrentAlarmsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CurrentAlarmsDlg(QWidget *parent = 0);
    ~CurrentAlarmsDlg();

protected:
    void initTableWidget();
    void addItemContent(int row, int column, const QString &content);
    void addRowContent(QStringList &list);
    void updateData();
    void initScrollArea();
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_refreshBtn_clicked();

    void on_quitBtn_clicked();

private:
    Ui::CurrentAlarmsDlg *ui;
    QScrollBar *m_scrollBarV;
};

#endif // CURRENTALARMSDLG_H
