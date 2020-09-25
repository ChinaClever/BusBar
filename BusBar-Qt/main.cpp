#include "mainwindow.h"
#include <QApplication>
#include "frminput.h"
#include "frmnum.h"
//#include "permissions.h"

//#include <QAndroidJniObject>
//#include <QAndroidJniEnvironment>
//#include <QtAndroid>

//bool checkPermission() {
//    QStringList list;
//    list << "android.permission.READ_EXTERNAL_STORAGE";
//    list << "android.permission.WRITE_EXTERNAL_STORAGE";
//    list << "android.permission.MOUNT_UNMOUNT_FILESYSTEMS";

//    for(int i=0; i<list.size(); ++i) {
//        QtAndroid::PermissionResult r = QtAndroid::checkPermission(list.at(i));
//        if(r == QtAndroid::PermissionResult::Denied) {
//            QtAndroid::requestPermissionsSync( QStringList() << list.at(i) );
//            r = QtAndroid::checkPermission(list.at(i));
//            if(r == QtAndroid::PermissionResult::Denied) {
//                return false;
//            }
//        }
//    }

//    return true;
//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    checkPermission();

    MainWindow w;
    w.showFullScreen();
    //    w.show();

    return a.exec();
}
