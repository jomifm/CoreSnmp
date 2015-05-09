#pragma once

#include "types/basic/snmpbasicabstracttype.h"

class NullObject : public SnmpBasicAbstractType
{
    Q_OBJECT
public: 
    QString toString() const;

    static NullObject *Instance();
private:
    NullObject(QObject *parent = 0);

    quint32 getDataLength() const;
    QByteArray encodeData() const;
    void decodeData(QDataStream &inputStream, quint32 length);
};
