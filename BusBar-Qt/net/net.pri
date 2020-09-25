INCLUDEPATH +=  $$PWD/udp/recv \
                $$PWD/udp/sent \
                $$PWD/tcp/client \
                $$PWD/datadone \
                $$PWD/analyze \

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/udp/sent/udpsentsocket.h \
    $$PWD/udp/sent/udpheartbeat.h \
    $$PWD/tcp/client/tcpclient.h \
    $$PWD/udp/recv/udprecvsocket.h \
    $$PWD/datadone/netanalyzedata.h \
    $$PWD/datadone/netpackdata.h \
    $$PWD/analyze/netdataanalyze.h \
    $$PWD/datadone/netdataformat.h \
    $$PWD/tcp/client/tcpsent.h \
    $$PWD/save/pduhashcom.h \
    $$PWD/save/pduhashdatasave.h \
    $$PWD/send/netsendthread.h \
    $$PWD/tcp/server/tcpserver.h \
    $$PWD/send/apptcpserver.h \
    $$PWD/send/devDataType.h \
    $$PWD/send/devDataSent.h \
    $$PWD/save/pduhashdevdatasave.h \
    $$PWD/network.h \
    $$PWD/send/devsetthread.h
	
SOURCES += \
    $$PWD/udp/recv/udprecvsocket.cpp \
    $$PWD/udp/sent/udpsentsocket.cpp \
    $$PWD/udp/sent/udpheartbeat.cpp \
    $$PWD/tcp/client/tcpclient.cpp \
    $$PWD/datadone/netanalyzedata.cpp \
    $$PWD/datadone/netpackdata.cpp \
    $$PWD/analyze/netdataanalyze.cpp \
    $$PWD/tcp/client/tcpsent.cpp \
    $$PWD/save/pduhashcom.cpp \
    $$PWD/save/pduhashdatasave.cpp \
    $$PWD/send/netsendthread.cpp \
    $$PWD/tcp/server/tcpserver.cpp \
    $$PWD/send/apptcpserver.cpp \
    $$PWD/send/devDataSent.cpp \
    $$PWD/save/pduhashdevdatasave.cpp \
    $$PWD/network.cpp \
    $$PWD/send/devsetthread.cpp


	
