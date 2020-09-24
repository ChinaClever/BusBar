
INCLUDEPATH += $$PWD

HEADERS += \	
    modbus/serialport/serial_trans.h \
    modbus/serialport/serial_test.h \
    modbus/serialport/serial_portset.h \
    modbus/rtu485/rtu_sent.h \
    modbus/rtu485/rtu_recv.h \
    modbus/rtuthread.h \
    modbus/thirdthread.h
   
	
SOURCES += \	
    modbus/serialport/serial_trans.cpp \
    modbus/serialport/serial_test.cpp \
    modbus/serialport/serial_portset.cpp \
    modbus/rtu485/rtu_sent.cpp \
    modbus/rtu485/rtu_recv.cpp \
    modbus/rtuthread.cpp \
    modbus/thirdthread.cpp
    


