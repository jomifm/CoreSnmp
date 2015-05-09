#pragma once

#include "types/basic/snmpbasicabstracttype.h"


class TimerTicks : public SnmpBasicAbstractType
{
    Q_OBJECT

    qint32 value;
public:
    TimerTicks(QObject *parent = 0);
    TimerTicks(const qint32 &value, QObject *parent = 0);

    qint32 getValue() const;
    QString toString() const;

private:
    quint32 getDataLength() const;
    QByteArray encodeData() const;
    void decodeData(QDataStream &inputStream, quint32 length);
};
