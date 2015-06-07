#include "types/utils/snmptypefactory.h"

#include "types/basic/snmpbasicabstracttype.h"
#include "types/basic/snmpinteger.h"
#include "types/basic/snmpipaddress.h"
#include "types/basic/snmpnull.h"
#include "types/basic/snmpnullobject.h"
#include "types/basic/snmpoctetstring.h"
#include "types/basic/snmpobjectid.h"
#include "types/basic/snmpsequence.h"
#include "types/basic/snmptimerticks.h"
#include "types/pdu/snmpgetrequestpdu.h"
#include "types/pdu/snmpgetresponsepdu.h"
#include "types/pdu/snmpsetrequestpdu.h"
#include "types/pdu/snmptrappdu.h"

SnmpTypeFactory::SnmpTypeFactory()
{
}

SnmpTypeFactory::TypeMap SnmpTypeFactory::typeMap()
{
    static TypeMap typeMap;
    if (typeMap.isEmpty()) {
        typeMap.insert(Type::Integer, "Integer");
        typeMap.insert(Type::OctetString, "String");
        typeMap.insert(Type::NullObject, "Unknown type");
    }
    return typeMap;
}

SnmpBasicAbstractType *SnmpTypeFactory::createType(Type::AbstractType type, QObject *parent)
{
    switch (type) {
    case Type::Integer: return new Integer(parent);
    case Type::OctetString: return new OctetString(parent);
    case Type::Null: return new Null(parent);
    case Type::ObjectIdentifier: return new ObjectIdentifier(parent);
    case Type::Sequence: return new Sequence(parent);
    case Type::IpAddress: return new IpAddress(parent);
    case Type::Counter: return new Integer(parent);
    case Type::Gauge: return new Integer(parent);
    case Type::TimerTicks: return new TimerTicks(parent);
    case Type::Opaque: return new Integer(parent);
    case Type::GetRequestPDU: return new SnmpGetRequestPdu(parent);
    case Type::GetResponsePDU: return new GetResponsePDU(parent);
    case Type::SetRequestPDU: return new SnmpSetRequestPdu(parent);
    case Type::TrapPDU: return new SnmpTrapPdu(parent);
    default:
        Q_ASSERT_X(false, "factory method", QString("Unknown type: %1").arg(type).toLatin1());
    };

    return NullObject::Instance();
}

QString SnmpTypeFactory::getTypeName(Type::AbstractType type)
{
    return typeMap().value(type);
}
