#pragma once

#include "types/basic/snmpinteger.h"

class SnmpVersion : public Integer
{
    Q_OBJECT
public:

    SnmpVersion(Type::Version version, QObject *parent = 0);

    Type::Version getVersion() const;
};
