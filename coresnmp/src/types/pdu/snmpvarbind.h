#pragma once

#include "types/basic/snmpsequence.h"

class ObjectIdentifier;
class Varbind : public Sequence
{
    Q_OBJECT
public:
    Varbind(ObjectIdentifier *objectIdentifier, SnmpBasicAbstractType *value, QObject *parent = 0);

    ObjectIdentifier *getObjectIdentifier() const;
    SnmpBasicAbstractType *getValue() const;
};
