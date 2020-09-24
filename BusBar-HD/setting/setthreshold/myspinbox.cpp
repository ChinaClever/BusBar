#include "myspinbox.h"
#include <QDebug>

MySpinBox::MySpinBox(QWidget *parent) : QSpinBox(parent)
{

}

void MySpinBox::mouseDoubleClickEvent(QMouseEvent *)
{
    emit getFocus();
}
