#include "types/snmpmessage.h"

#include "types/snmpversion.h"
#include "types/basic/snmpoctetstring.h"
#include "types/pdu/snmppdu.h"

#ifdef QT_SNMP_DEBUG
#include <QDebug>
#endif

enum DataPosition {
    SnmpVersionPosition = 0,
    CommunityPosition = 1,
    ProtocolDataUnitPosition = 2
};

SnmpMessage::SnmpMessage(QObject *parent) :
    Sequence(parent)
{
}

SnmpMessage::SnmpMessage(SnmpVersion *version, OctetString *community, ProtocolDataUnit *protocolDataUnit, QObject *parent) :
    Sequence(SequenceData() << version << community << protocolDataUnit, parent)
{
}

SnmpVersion *SnmpMessage::getSnmpVersion() const
{
    return static_cast<SnmpVersion *>(getDataType(SnmpVersionPosition));
}

OctetString *SnmpMessage::getCommunity() const
{
    return static_cast<OctetString *>(getDataType(CommunityPosition));
}

ProtocolDataUnit *SnmpMessage::getProtocolDataUnit() const
{    
    return static_cast<ProtocolDataUnit *>(getDataType(ProtocolDataUnitPosition));
}

QString SnmpMessage::toString()
{
	QString str;
	QString strVersion;

	if (getSnmpVersion()->getValue() == Type::SNMPv3) strVersion = "snmpv3";
	else if (getSnmpVersion()->getValue() == Type::SNMPv2c) strVersion = "v2c";
	else if (getSnmpVersion()->getValue() == Type::SNMPv1) strVersion = "version-1";
	else strVersion = "version-unknown";

	str.append(QString("\n"));
	str.append(QString("Simple Network Management Protocol\n"));
	str.append(QString("  version: %1 (%2)\n").arg(strVersion).arg(getSnmpVersion()->toString()));
	str.append(QString("  community: %1\n").arg(getCommunity()->toString()));
	str.append(QString("%1\n").arg(getProtocolDataUnit()->toString()));

    return str;
}
