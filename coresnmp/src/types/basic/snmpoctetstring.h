#pragma once

#include "types/basic/snmpbasicabstracttype.h"

#include <QString>

class OctetString : public SnmpBasicAbstractType
{
    Q_OBJECT

    QString value;    
public:
    OctetString(QObject *parent = 0);
    OctetString(const QString &value, QObject *parent = 0);

    static OctetString *create(const QString &str, QObject *parent = 0) {
    	return new OctetString(str, parent);
    }

    QString getValue() const;
    QString toString() const;

private:
    quint32 getDataLength() const;
    QByteArray encodeData() const;
    void decodeData(QDataStream &inputStream, quint32 length);
};
