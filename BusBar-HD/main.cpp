#include "mainwindow.h"
#include <QApplication>
#include "frminput.h"
#include "frmnum.h"

int main(int argc, char *argv[])
{
    //    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QApplication a(argc, argv);

    // qApp->setFont(QFont("Microsoft YaHei", 10));
    //在main函数中就加载输入法面板,保证整个应用程序可以使用输入法
    //frmInput::Instance()->Init("bottom", "black", 10, 10);  //打开中文输入法
    //     frmNum::Instance()->Init("lightgray",10); // 打开数字键盘

    MainWindow w;
    w.show();

    return a.exec();
}
