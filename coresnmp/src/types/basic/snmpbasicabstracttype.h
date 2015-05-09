#pragma once

#include "snmp/snmpdatatypes.h"
#include "exceptions/snmpexception.h"

#include <QObject>
#include <QByteArray>

//class SnmpTypeFactory;
class SnmpBasicAbstractType : public QObject
{
    Q_OBJECT

    Type::AbstractType type;

public:
    SnmpBasicAbstractType(Type::AbstractType type, QObject *parent = 0);

    QByteArray encode() const;
    int decode(QDataStream &inputStream);
    quint32 getLength() const;

    Type::AbstractType getType() const;

    virtual QString toString() const;
    virtual QObject *toObject();

private:
    quint32 intSizeLenght_;
    virtual quint32 getDataLength() const = 0;
    virtual QByteArray encodeData() const = 0;
    virtual void decodeData(QDataStream &inputStream, quint32 length) = 0;
};
