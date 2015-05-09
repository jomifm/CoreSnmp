#include "types/snmpversion.h"

SnmpVersion::SnmpVersion(Type::Version version, QObject *parent) :
    Integer(static_cast<quint32>(version), parent)
{
}

Type::Version SnmpVersion::getVersion() const
{
    return static_cast<Type::Version>(getValue());
}
