#include "types/basic/snmpsequence.h"

#include "types/basic/snmpnullobject.h"
#include "types/utils/snmptypefactory.h"

#include <snmp/exceptions/snmpexception.h>

#include <QDataStream>
#include <QDebug>

Sequence::Sequence(QObject *parent) :
    SnmpBasicAbstractType(Type::Sequence, parent)
{
}

Sequence::Sequence(const SequenceData &sequenceData, QObject *parent) :
    SnmpBasicAbstractType(Type::Sequence, parent),
    sequenceData(sequenceData)
{
    foreach (SnmpBasicAbstractType *data, sequenceData)
        data->setParent(this);
}

SequenceData Sequence::getSequenceData() const
{
    return sequenceData;
}

SnmpBasicAbstractType *Sequence::getDataType(int pos) const
{
    if (sequenceData.size() < pos + 1)
        throw SnmpException("No such element");
    return sequenceData.at(pos);
}

quint32 Sequence::getDataLength() const
{
    quint32 totalDataLength = 0;
    foreach (SnmpBasicAbstractType *value, sequenceData)
        totalDataLength += value->getLength();
    return totalDataLength;
}

QByteArray Sequence::encodeData() const
{
    QByteArray code;
    foreach (SnmpBasicAbstractType *value, sequenceData)
        code.append(value->encode());
    return code;
}

void Sequence::decodeData(QDataStream &inputStream, quint32 length)
{
    quint32 bytesRead = 0;
    while (!inputStream.atEnd() && bytesRead++ < length) {
        quint8 type = 0;
        inputStream >> type;
        SnmpBasicAbstractType *asnType = SnmpTypeFactory::createType(static_cast<Type::AbstractType>(type), this);
        bytesRead += asnType->decode(inputStream);

        sequenceData.append(asnType);
    }
}
