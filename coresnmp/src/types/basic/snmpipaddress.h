#pragma once

#include "types/basic/snmpbasicabstracttype.h"

//@Signed IpAddress32
class IpAddress : public SnmpBasicAbstractType
{
    Q_OBJECT

    qint32 value;
public:
    IpAddress(QObject *parent = 0);
    IpAddress(const QString &value, QObject *parent = 0);
    IpAddress(const qint32 &value, QObject *parent = 0);

    qint32 getValue() const;
    QString toString() const;

private:
    quint32 getDataLength() const;
    QByteArray encodeData() const;
    void decodeData(QDataStream &inputStream, quint32 length);
};
