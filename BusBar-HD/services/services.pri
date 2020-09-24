
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/smtp/emailsetdlg.h \
    $$PWD/smtp/email.h \
    $$PWD/smtp/smtp.h \
    $$PWD/smtp/smtpssl.h \
    $$PWD/smtp/emailwid.h \
    $$PWD/smtp/emailthread.h \
    $$PWD/excel/basicExcel/BasicExcel.hpp \
    $$PWD/excel/excelexportlogthread.h \
    $$PWD/excel/excelexportwid.h \
    $$PWD/excel/txtexportlogthread.h \
    $$PWD/datetime/timesettingdlg.h
    
   
	
SOURCES += \
    $$PWD/smtp/emailsetdlg.cpp \
    $$PWD/smtp/email.cpp \
    $$PWD/smtp/smtp.cpp \
    $$PWD/smtp/smtpssl.cpp \
    $$PWD/smtp/emailwid.cpp \
    $$PWD/smtp/emailthread.cpp \
    $$PWD/excel/basicExcel/BasicExcel.cpp \
    $$PWD/excel/execl_demo.cpp \
    $$PWD/excel/excelexportlogthread.cpp \
    $$PWD/excel/excelexportwid.cpp \
    $$PWD/excel/txtexportlogthread.cpp \
    $$PWD/datetime/timesettingdlg.cpp \


    
FORMS += \
    $$PWD/smtp/emailsetdlg.ui \
    $$PWD/smtp/emailwid.ui \
    $$PWD/excel/excelexportwid.ui \
    $$PWD/datetime/timesettingdlg.ui


