#include "types/basic/snmpnull.h"

Null::Null(QObject *parent) :
    SnmpBasicAbstractType(Type::Null, parent)
{
}

QString Null::toString() const
{
    return QString("Null");
}

quint32 Null::getDataLength() const
{
    return 0;
}

QByteArray Null::encodeData() const
{
    return QByteArray();
}

void Null::decodeData(QDataStream &inputStream, quint32 length)
{
    Q_UNUSED(inputStream);
    Q_UNUSED(length);
}
