#include "types/basic/snmptimerticks.h"

#include <QDataStream>
#include <QDate>

#define HIGH_BIT 0x80

TimerTicks::TimerTicks(QObject *parent) :
    SnmpBasicAbstractType(Type::TimerTicks, parent)
{
	QDateTime setTime = QDateTime::fromString (QString("1970-07-18T14:15:09"), Qt::ISODate);
	QDateTime current = QDateTime::currentDateTime();
	value = (qint32)setTime.time().msecsTo(current.time());
}

TimerTicks::TimerTicks(const qint32 &value, QObject *parent) :
    SnmpBasicAbstractType(Type::TimerTicks, parent),
    value(value)
{
}

qint32 TimerTicks::getValue() const
{
    return value;
}

QString TimerTicks::toString() const
{
    return QString::number(value);
}

quint32 TimerTicks::getDataLength() const
{
    int value = this->value;
    int valueSize = 0;

    do {
        value >>= 7;
        valueSize++;
    } while (value != 0);

    return valueSize;
}

QByteArray TimerTicks::encodeData() const
{
    QByteArray code;

    int TimerTicks = value;
    int mask;
    int intsize = 4;

    /*
     * Truncate "unnecessary" bytes off of the most significant end of this
     * 2's complement TimerTicks.  There should be no sequence of 9
     * consecutive 1's or 0's at the most significant end of the
     * TimerTicks.
     */
    mask = 0x1FF << ((8 * 3) - 1);
    /* mask is 0xFF800000 on a big-endian machine */
    while ((((TimerTicks & mask) == 0) || ((TimerTicks & mask) == mask))
          && intsize > 1){
        intsize--;
        TimerTicks <<= 8;
    }
    //encodeHeader(os, type, intsize);
    mask = 0xFF << (8 * 3);
    /* mask is 0xFF000000 on a big-endian machine */
    while ((intsize--) > 0){
        code.append(((TimerTicks & mask) >> (8 * 3)));
        TimerTicks <<= 8;
    }

    return code;
}

void TimerTicks::decodeData(QDataStream &inputStream, quint32 length)
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
