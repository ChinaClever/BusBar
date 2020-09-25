#ifndef CURRENTALARMSDLG_H
#define CURRENTALARMSDLG_H

#include <QDialog>

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

private slots:
    void on_refreshBtn_clicked();

    void on_quitBtn_clicked();

private:
    Ui::CurrentAlarmsDlg *ui;
};

#endif // CURRENTALARMSDLG_H
