#pragma once

#include "types/basic/snmpbasicabstracttype.h"

typedef QList<SnmpBasicAbstractType *> SequenceData;

class Sequence : public SnmpBasicAbstractType
{    
    Q_OBJECT
    SequenceData sequenceData;
public:
    Sequence(QObject *parent = 0);
    Sequence(const SequenceData &sequenceData, QObject *parent = 0);

    SequenceData getSequenceData() const;

protected:
    SnmpBasicAbstractType *getDataType(int pos) const;

private:
    quint32 getDataLength() const;
    QByteArray encodeData() const;
    void decodeData(QDataStream &inputStream, quint32 length);
};
