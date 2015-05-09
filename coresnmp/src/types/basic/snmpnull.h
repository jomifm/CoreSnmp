#pragma once

#include "types/basic/snmpbasicabstracttype.h"

class Null : public SnmpBasicAbstractType
{
    Q_OBJECT
public:
    Null(QObject *parent = 0);

    QString toString() const;

private:
    quint32 getDataLength() const;
    QByteArray encodeData() const;
    void decodeData(QDataStream &inputStream, quint32 length);
};
