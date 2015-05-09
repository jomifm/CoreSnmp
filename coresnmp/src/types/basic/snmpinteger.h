#pragma once

#include "types/basic/snmpbasicabstracttype.h"

//@Signed Integer32
class Integer : public SnmpBasicAbstractType
{
    Q_OBJECT

    qint32 value;
public:    
    Integer(QObject *parent = 0);
    Integer(const qint32 &value, QObject *parent = 0);

    qint32 getValue() const;
    QString toString() const;

private:
    quint32 getDataLength() const;
    QByteArray encodeData() const;
    void decodeData(QDataStream &inputStream, quint32 length);
};
