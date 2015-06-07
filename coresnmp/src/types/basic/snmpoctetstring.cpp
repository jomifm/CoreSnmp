#include "types/basic/snmpoctetstring.h"

#include "utils/logger/utilslogger.h"

#include <QDataStream>

OctetString::OctetString(QObject *parent) :
    SnmpBasicAbstractType(Type::OctetString, parent)
{
}

OctetString::OctetString(const QString &value, QObject *parent) :
    SnmpBasicAbstractType(Type::OctetString, parent),
    value(value)
{
}

QString OctetString::getValue() const
{
    return value;
}

void OctetString::setValue(const QString & strVal)
{
    value = strVal;
}

QString OctetString::toString() const
{
    return getValue();
}

quint32 OctetString::getDataLength() const
{
    return value.length();
}

QByteArray OctetString::encodeData() const
{
    QByteArray code;
    code.append(value);
    return code;
}

void OctetString::decodeData(QDataStream &inputStream, quint32 length)
{
    quint32 bytesRead = 0;
    QByteArray buffer;

    while (bytesRead++ < length) {
        quint8 byte = 0;
        inputStream >> byte;
        buffer.append(byte);
    }
    LogDebug << "OctetString::decodeData:" << buffer.data();

    value = buffer;
}

SnmpBasicAbstractType *OctetString::clone()
{
	SnmpBasicAbstractType *abstract = NULL;
	OctetString *clon = new OctetString(value);
	abstract = static_cast<SnmpBasicAbstractType*>(clon);
	return abstract;
}
