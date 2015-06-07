#include "types/pdu/snmpvarbind.h"

#include "types/basic/snmpobjectid.h"

Varbind::Varbind(ObjectIdentifier *objectIdentifier, SnmpBasicAbstractType *value, QObject *parent) :
    Sequence(SequenceData() << objectIdentifier << value, parent)
{
}

ObjectIdentifier *Varbind::getObjectIdentifier() const
{
    return static_cast<ObjectIdentifier *>(getDataType(0));
}

SnmpBasicAbstractType *Varbind::getValue() const
{
    return getDataType(1);
}
