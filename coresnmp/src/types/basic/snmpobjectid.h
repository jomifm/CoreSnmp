#pragma once

#include "types/basic/snmpbasicabstracttype.h"

#include <QVector>

class Integer;
class ObjectIdentifier : public SnmpBasicAbstractType
{
    Q_OBJECT
    QVector<quint32> objectIdentifiers;
public:
    ObjectIdentifier(QObject *parent = 0);
    ObjectIdentifier(const QString &objectIdentifier, QObject *parent = 0);

    QString toString() const;

private:
    quint32 getDataLength() const;
    QByteArray encodeData() const;
    void decodeData(QDataStream &inputStream, quint32 length);
};
