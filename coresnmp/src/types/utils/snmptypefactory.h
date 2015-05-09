#pragma once

#include "snmp/snmpdatatypes.h"

class QObject;
class SnmpBasicAbstractType;
class SnmpTypeFactory
{    
    typedef QHash<Type::AbstractType, QString> TypeMap;
    static TypeMap typeMap();
public:
    SnmpTypeFactory();

    static SnmpBasicAbstractType *createType(Type::AbstractType type, QObject *parent = 0);   
    static QString getTypeName(Type::AbstractType type);
};
