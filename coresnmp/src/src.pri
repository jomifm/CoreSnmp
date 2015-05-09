HEADERS += \
    include/snmp/snmpdata.h \
    include/snmp/snmpdatatypes.h \
    include/snmp/snmprequest.h \
    include/snmp/snmptrap.h


HEADERS += \
    src/snmpdata_p.h \
    src/snmprequest_p.h \
    src/snmptrap_p.h

SOURCES += \
    src/snmpdata_p.cpp \
    src/snmprequest_p.cpp \
    src/snmptrap_p.cpp
    
  
HEADERS += \
    src/configuration/parameters.h \
    src/exceptions/snmpexception.h \
    src/types/basic/snmpbasicabstracttype.h \
    src/types/basic/snmpinteger.h \
    src/types/basic/snmpipaddress.h \
    src/types/basic/snmpnull.h \
    src/types/basic/snmpobjectid.h \
    src/types/basic/snmpnullobject.h \
    src/types/basic/snmpoctetstring.h \
    src/types/basic/snmpsequence.h \
    src/types/basic/snmptimerticks.h \
    src/types/pdu/snmpgetrequestpdu.h \
    src/types/pdu/snmpgetresponsepdu.h \
    src/types/pdu/snmppdu.h \
    src/types/pdu/snmpsetrequestpdu.h \
    src/types/pdu/snmptrappdu.h \
    src/types/pdu/snmpvarbind.h \
    src/types/pdu/snmpvarbindlist.h \
    src/types/utils/snmptypefactory.h \
    src/types/snmpmessage.h \
    src/types/snmpversion.h

SOURCES += \
    src/configuration/parameters.cpp \
    src/types/basic/snmpbasicabstracttype.cpp \
    src/types/basic/snmpinteger.cpp \
    src/types/basic/snmpipaddress.cpp \
    src/types/basic/snmpnull.cpp \
    src/types/basic/snmpnullobject.cpp \
    src/types/basic/snmpobjectid.cpp \
    src/types/basic/snmpoctetstring.cpp \
    src/types/basic/snmpsequence.cpp \
    src/types/basic/snmptimerticks.cpp \
    src/types/pdu/snmpgetrequestpdu.cpp \
    src/types/pdu/snmpgetresponsepdu.cpp \
    src/types/pdu/snmppdu.cpp \
    src/types/pdu/snmpsetrequestpdu.cpp \
    src/types/pdu/snmptrappdu.cpp \
    src/types/pdu/snmpvarbind.cpp \
    src/types/pdu/snmpvarbindlist.cpp \
    src/types/utils/snmptypefactory.cpp \
    src/types/snmpmessage.cpp \
    src/types/snmpversion.cpp
