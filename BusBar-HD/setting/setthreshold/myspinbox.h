#ifndef MYSPINBOX_H
#define MYSPINBOX_H

#include <QWidget>
#include <QSpinBox>

class MySpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit MySpinBox(QWidget *parent = nullptr);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void getFocus();

public slots:
};

#endif // MYSPINBOX_H
