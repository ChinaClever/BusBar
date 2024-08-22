#include "languagesetting.h"
#include "ui_languagesetting.h"


Languagesetting::Languagesetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Languagesetting)
{
    ui->setupUi(this);
    com_setBackColour(tr("设置界面"), this);
    QPixmap pix(1,60);
    pix.fill(Qt::transparent);
    QIcon icon(pix);
    ui->comboBox->setIconSize(QSize(1,60));
    ui->comboBox->setItemIcon(0 , icon);
    ui->comboBox->setItemIcon(1 , icon);
    init();
}

Languagesetting::~Languagesetting()
{
    delete ui;
}
void Languagesetting::init()
{
//    ui->comboBox->setView();
    ui->comboBox->setCurrentIndex(gLanguage);
    if(gLanguage == 0) ui->comboBox->setItemText(0,"中文简体");
    else ui->comboBox->setItemText(0,"Chinese");
}
void Languagesetting::on_pushButton_clicked()
{
    this->close();
}

void Languagesetting::on_pushButton_2_clicked()
{
    bool ret = true;
    if(gLanguage == 0) ret = MsgBox::question(this, tr("是否重启系统?"));
    else ret = MsgBox::question(this, tr("Do you want to restart the system?"));
    if(ret) {
        sys_configFile_writeParam("language",QString::number(ui->comboBox->currentIndex()));
        system("reboot");
    }
}
