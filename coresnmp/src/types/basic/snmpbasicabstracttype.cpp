#include "types/basic/snmpbasicabstracttype.h"

#include "types/utils/snmptypefactory.h"

#include <QDataStream>

#include <QDebug>

static const quint32 TYPE_SIZE = 1;
//static const quint8 LENGTH_SIZE = 1;

SnmpBasicAbstractType::SnmpBasicAbstractType(Type::AbstractType type, QObject *parent) :
    QObject(parent),
    type(type)
{
    intSizeLenght_ = TYPE_SIZE;
}

QByteArray SnmpBasicAbstractType::encode() const
{
    quint32 dataLength = getDataLength();

    QByteArray code;
    code.append(type);
    code.append(dataLength);
    code.append(encodeData());

    return code;
}

int SnmpBasicAbstractType::decode(QDataStream &inputStream)
{
	quint8 hightBite = 0x80;
    quint8 dataByte = 0;
    quint8 dataLength8 = 0;
    quint16 dataLength16 = 0;
    quint32 dataLength32 = 0;

    inputStream >> dataByte;

    //Check size of decode inputstream
    if (dataByte > hightBite)
    	intSizeLenght_ += (dataByte - hightBite);

    //Check lenght size
    if (intSizeLenght_ - 1 == 0) {
        dataLength32 = dataByte;
    }
    else if (intSizeLenght_ - 1 == 1) {
        inputStream >> dataLength8;
        dataLength32 = dataLength8;
    }
    else if (intSizeLenght_ - 1 == 2) {
        inputStream >> dataLength16;
        dataLength32 = dataLength16;
    }
    else inputStream >> dataLength32;

    qDebug() << "Start decoding:" << this;
    decodeData(inputStream, dataLength32);
    qDebug() << "Decoded:" << this;

    return intSizeLenght_ + dataLength32;
}

quint32 SnmpBasicAbstractType::getLength() const
{
    return TYPE_SIZE + intSizeLenght_ + getDataLength();
}

Type::AbstractType SnmpBasicAbstractType::getType() const
{
    return type;
}

QString SnmpBasicAbstractType::toString() const
{
    return QString();
}

QObject *SnmpBasicAbstractType::toObject()
{
    return dynamic_cast<QObject*>(this);
}
