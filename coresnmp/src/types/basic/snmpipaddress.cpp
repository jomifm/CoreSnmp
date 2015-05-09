#include "snmpipaddress.h"

#include <QDataStream>
#include <QHostAddress>
#include <qendian.h>

#ifdef QT_SNMP_DEBUG
#include <QDebug>
#endif

#define HIGH_BIT 0x80

IpAddress::IpAddress(QObject *parent) :
    SnmpBasicAbstractType(Type::IpAddress, parent),
    value(0)
{
}

IpAddress::IpAddress(const QString &value, QObject *parent) :
    SnmpBasicAbstractType(Type::IpAddress, parent),
    value(qToBigEndian(QHostAddress(value).toIPv4Address()))
{
}

IpAddress::IpAddress(const qint32 &value, QObject *parent) :
    SnmpBasicAbstractType(Type::IpAddress, parent),
    value(value)
{
}

qint32 IpAddress::getValue() const
{
    return value;
}

QString IpAddress::toString() const
{
    return QHostAddress(qFromBigEndian(getValue())).toString();
}

quint32 IpAddress::getDataLength() const
{
    int valueSize = 4;
    return valueSize;
}

QByteArray IpAddress::encodeData() const
{
    QByteArray code;

    qint32 x = getValue();
    code.append((const char*)&x, sizeof(qint32));

    return code;
}

void IpAddress::decodeData(QDataStream &inputStream, quint32 length)
{
    quint32 bytesRead = 0;

    quint8 byte = 0;
    inputStream >> byte;
    bytesRead++;

    value = (byte & HIGH_BIT) == 0 ? 0 : -1;

    forever {
        value <<= 8;
        value |= byte;

        if (bytesRead >= length)
            break;

        inputStream >> byte;
        bytesRead++;
    }
}
