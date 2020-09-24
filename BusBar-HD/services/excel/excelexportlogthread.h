#ifndef EXCELEXPORTLOGTHREAD_H
#define EXCELEXPORTLOGTHREAD_H

#include <QThread>
#include <QtCore>

struct sExcelExportStr
{
    QString path; // 导出路径
    QString file; // 导出文件名
    QString db; // 数据库表
    QString start; // 开始时间
    QString end; // 结束时间
    QStringList title; // 标头
};


class ExcelExportLogThread : public QThread
{
    Q_OBJECT
public:
    explicit ExcelExportLogThread(QObject *parent = 0);
    ~ExcelExportLogThread();

    void set(const QString &file, const QList<QStringList> &list);

signals:
    void overSig(bool);
    void progressSig(int);

protected:
    void run();
    int getProgress();
    virtual void exportMsg(QList<QStringList> &list);

protected slots:
    void progressSlot();

protected:
    int mSize, mAllSize;
    QList<QStringList> mList;
    QString mFile; // 导出文件
};

#endif // EXCELEXPORTLOGTHREAD_H
