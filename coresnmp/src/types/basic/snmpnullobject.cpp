#include "types/basic/snmpnullobject.h"

NullObject::NullObject(QObject *parent) :
    SnmpBasicAbstractType(Type::NullObject, parent)
{
}

QString NullObject::toString() const
{
    throw SnmpException("No such element");
    return QString();
}

NullObject *NullObject::Instance()
{
    static NullObject nullObject;
    return &nullObject;
}

quint32 NullObject::getDataLength() const
{
    throw SnmpException("No such element");
    return 0;
}

QByteArray NullObject::encodeData() const
{
    throw SnmpException("No such element");
    return QByteArray();
}

void NullObject::decodeData(QDataStream &inputStream, quint32 length)
{
    Q_UNUSED(inputStream);
    Q_UNUSED(length);
    throw SnmpException("No such element");
}
