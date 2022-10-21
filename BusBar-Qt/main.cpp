#include "mainwindow.h"
#include <QApplication>
#include "frminput.h"
#include "frmnum.h"
//#include "permissions.h"

#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>

bool checkPermission() {
    QStringList list;
    list << "android.permission.READ_EXTERNAL_STORAGE";
    list << "android.permission.WRITE_EXTERNAL_STORAGE";
    list << "android.permission.MOUNT_UNMOUNT_FILESYSTEMS";
    for(int i=0; i<list.size(); ++i) {
        QtAndroid::PermissionResult r = QtAndroid::checkPermission(list.at(i));
        if(r == QtAndroid::PermissionResult::Denied) {
            QtAndroid::requestPermissionsSync( QStringList() << list.at(i) );
            r = QtAndroid::checkPermission(list.at(i));
            if(r == QtAndroid::PermissionResult::Denied) {
                return false;
            }
        }
    }

    return true;
}

void addWatchdog() {
    QAndroidJniObject runtime = QAndroidJniObject::callStaticObjectMethod(
                "java/lang/Runtime","getRuntime","()Ljava/lang/Runtime;");
    if(!runtime.isValid())
    {
        qDebug()<<"runtime.noValid()!"<<endl;
        return;
    }
    QAndroidJniObject suStr = QAndroidJniObject::fromString("su");
    QAndroidJniObject process = runtime.callObjectMethod(
                "exec","(Ljava/lang/String;)Ljava/lang/Process;",suStr.object());
    if(!process.isValid())
    {
        qDebug()<<"process.noValid()!"<<endl;
        return;
    }
    QAndroidJniObject outputStream = process.callObjectMethod(
                "getOutputStream","()Ljava/io/OutputStream;");
    if(!outputStream.isValid())
    {
        qDebug()<<"outputStream.noValid()!"<<endl;
        return;
    }
    QAndroidJniObject dataOutputStream = QAndroidJniObject(
                "java/io/DataOutputStream","(Ljava/io/OutputStream;)V",outputStream.object());
    if(!dataOutputStream.isValid())
    {
        qDebug()<<"dataOutputStream.noValid()!"<<endl;
        return;
    }
    QString strStartWatchdog3 = QString("mount -o remount,rw /system\n");
    QAndroidJniObject str3 = QAndroidJniObject::fromString(strStartWatchdog3);
    dataOutputStream.callMethod<void>("writeBytes" , "(Ljava/lang/String;)V",str3.object());
    QString strStartWatchdog4 = QString("rm /system/bin/bootclone.sh\n");
    QAndroidJniObject str4 = QAndroidJniObject::fromString(strStartWatchdog4);
    dataOutputStream.callMethod<void>("writeBytes" , "(Ljava/lang/String;)V",str4.object());

    QString strStartWatchdog = QString("insmod /system/vendor/modules/sunxi_wdt.ko\n");
    QAndroidJniObject str1 = QAndroidJniObject::fromString(strStartWatchdog);
    dataOutputStream.callMethod<void>("writeBytes" , "(Ljava/lang/String;)V",str1.object());
    QAndroidJniObject str2 = QAndroidJniObject::fromString("exit\n");
    dataOutputStream.callMethod<void>("writeBytes" , "(Ljava/lang/String;)V",str2.object());
    dataOutputStream.callMethod<void>("flush","()V");
}

bool appendFile(const QString &msg)
{
    QString fn = "/sdcard/process_log.txt";
    QFile file(fn);
    if(file.open(QIODevice::Append | QIODevice::Text)) {
        QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz\t");
        QByteArray array = t.toUtf8() + msg.toUtf8()+"\n";
        file.write(array);
    } file.close();

    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    checkPermission();
    appendFile("monitor");
    addWatchdog();
    MainWindow w;
    w.showFullScreen();
    //    w.show();

    return a.exec();
}
