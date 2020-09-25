#include "mynumberkeyboardwid.h"
#include "ui_mynumberkeyboardwid.h"

MyNumberKeyboardWid::MyNumberKeyboardWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyNumberKeyboardWid)
{
    ui->setupUi(this);

    mWidType = 0;
    initWid();
}

MyNumberKeyboardWid::~MyNumberKeyboardWid()
{
    delete ui;
}

void MyNumberKeyboardWid::initWid()
{
    initButton();

    QList<QPushButton *> btn = this->findChildren<QPushButton *>();
    foreach (QPushButton * b, btn) {
        connect(b, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    }
}

void MyNumberKeyboardWid::initButton()
{    
    ui->pushButton_0->setProperty("number",true);
    ui->pushButton_1->setProperty("number",true);
    ui->pushButton_2->setProperty("number",true);
    ui->pushButton_3->setProperty("number",true);
    ui->pushButton_4->setProperty("number",true);
    ui->pushButton_5->setProperty("number",true);
    ui->pushButton_6->setProperty("number",true);
    ui->pushButton_7->setProperty("number",true);
    ui->pushButton_8->setProperty("number",true);
    ui->pushButton_9->setProperty("number",true);
    ui->pushButton_space->setProperty("nonnumber",true);
    ui->pushButton_delate->setProperty("nonnumber",true);
}

void MyNumberKeyboardWid::buttonClicked()
{
    QPushButton *btn = (QPushButton *)sender();
    QString objectName = btn->objectName();
    QString text = btn->text();
    bool isnum = btn->property("number").toBool();
    BeepThread::bulid()->beep();
    if(isnum)
    {
        insertValue(text);
    }else if(text == "delate")
    {
        delateValue();
    }
    else if(text == "space")
        insertSpace();
}

void MyNumberKeyboardWid::insertValue(QString text)
{
    mEdit->insert(text);
#if 0
    switch (mWidType) {
    case 1:
        currentLineEdit->insert(text);
        break;
    case 2:
        //        QKeyEvent keyPress(QEvent::KeyPress, 0, Qt::NoModifier, text);
        //        QApplication::sendEvent(currentSpinBox, &keyPress);
        break;
    default:
        break;
    }
#endif
}

void MyNumberKeyboardWid::delateValue()
{
    QString text = mEdit->text();
    QString newText ="";
    int len = text.length();
    if(len >1)
        newText = text.remove(len-1,1);
    mEdit->clear();
    mEdit->setText(newText);
}

void MyNumberKeyboardWid::insertSpace()
{

}

void MyNumberKeyboardWid::setCurrentLineEdit(QLineEdit *lineedit)
{
    mEdit = lineedit;
}

void MyNumberKeyboardWid::focusChanged(QWidget *old,QWidget *now)
{
    if(now->inherits("QLineEdit"))
    {
        currentLineEdit = (QLineEdit *)now;
        mWidType = 1;
    } else if(now->inherits("QSpinBox"))
    {
        currentSpinBox = (QSpinBox *)now;
        mWidType = 2;
    }
}

