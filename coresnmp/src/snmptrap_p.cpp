#include "snmptrap_p.h"

#include <snmp/snmptrap.h>
#include <snmp/exceptions/snmpexception.h>

#include "configuration/configuration.h"
#include "types/snmpversion.h"
#include "types/snmpmessage.h"
#include "types/basic/snmpipaddress.h"
#include "types/basic/snmpnull.h"
#include "types/basic/snmpobjectid.h"
#include "types/basic/snmpoctetstring.h"
#include "types/basic/snmpobjectid.h"
#include "types/basic/snmpsequence.h"
#include "types/basic/snmptimerticks.h"
#include "types/pdu/snmpgetrequestpdu.h"
#include "types/pdu/snmptrappdu.h"
#include "types/pdu/snmpvarbind.h"
#include "types/pdu/snmpvarbindlist.h"
#include "types/utils/snmptypefactory.h"
#include "utils/logger/utilslogger.h"

#include <QUdpSocket>

SnmpTrap *SnmpTrap::instance(QObject *parent)
{
	//Instancia los parametros de configuracion
	Configuration::instance();

	static SnmpTrap *snmpRequest = new SnmpTrapPriv(parent);
	return snmpRequest;
}

SnmpTrapPriv::SnmpTrapPriv(QObject *parent) :
    SnmpTrap(parent)
{    
	LogDebug << "Creating SnmpTrapPriv object";

	//Create new udp socket
	socket_ = new QUdpSocket(this);
    socket_->bind(QHostAddress::Any, Configuration::get(Params::TrapAgentPort).toInt(),
    		QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(socket_,SIGNAL(readyRead()),SLOT(readPendingDatagram()));

	//Custom configuration of udp socket
    socket_->setSocketOption(QAbstractSocket::MulticastTtlOption, 10);
    socket_->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 1);

    LogDebug << "Socket state:" << socket_->state() << socket_->errorString();
    if (socket_->error() != QAbstractSocket::UnknownSocketError)
    	throw SnmpException(QString("Error opening or configuring socket"));
}

void SnmpTrapPriv::trap(const QString &strTrapId,
		const QString &addressSrc, const Type::MSnmpObject &objectMap)
{
	this->trap (Configuration::get(Params::TrapNmsAddress).split(","), strTrapId, addressSrc, objectMap);
}

void SnmpTrapPriv::trap(const QStringList &peerList, const QString &strTrapId,
		const QString &addressSrc, const Type::MSnmpObject &objectMap)
{
	//Variables declaration
    SnmpVersion *version = NULL;
    OctetString *community = NULL;
    ObjectIdentifier *trapId = NULL;
    IpAddress *agentAddr = NULL;
    Integer *genericTrap = NULL;
    Integer *specificTrap = NULL;
    TimerTicks *timeStamp = NULL;
    ObjectIdentifier *objectIdentifier = NULL;
    VarbindList *varbindList = NULL;
    SequenceData seq;
    ProtocolDataUnit *pdu = NULL;

    //Message initialization
    version = new SnmpVersion(Type::SNMPv1);
    community = new OctetString("Acc51oC!");

    //Setting header values
    trapId = new ObjectIdentifier(strTrapId);
    agentAddr = new IpAddress(addressSrc);
    genericTrap = new Integer(6);
    specificTrap = new Integer(0); //TODO
    timeStamp = new TimerTicks(1); //TODO

    //Processing object list
    for (Type::ISnmpObject iterObj = objectMap.begin(); iterObj != objectMap.end(); ++iterObj)
    {
    	//Create object identifier
    	objectIdentifier = new ObjectIdentifier(iterObj.key());

    	//Append object identifier in varbind list with null value
    	seq << new Varbind(objectIdentifier, dynamic_cast<SnmpBasicAbstractType*>(iterObj.value()));
    }

    //Create PDU element with necesary elements for trap
	varbindList = new VarbindList(seq);
    pdu = new SnmpTrapPdu(trapId, agentAddr, genericTrap, specificTrap, timeStamp, varbindList);

    //Create complete snmp message and datagram
    SnmpMessage snmpMessage(version, community, pdu);
    QByteArray datagram = snmpMessage.encode();

    //Processing destination peer list
    for (QStringList::const_iterator iterPeer = peerList.begin();
    		iterPeer != peerList.end(); ++iterPeer) {

    	//Send datagram to destination peer
    	socket_->writeDatagram(datagram, QHostAddress(*iterPeer),
    			Configuration::get(Params::TrapNmsPort).toInt());
    }

    LogInfo << "SNMP Trap sent" << datagram.toHex().data();
    LogInfo << snmpMessage.toString();
}

void SnmpTrapPriv::readPendingDatagram()
{
	//Variables declaration
    QByteArray datagram;
    QHostAddress sender;
    quint16 senderPort;
    QStringList responseList_;

	//Process while exist pending datagrams
    while (socket_->hasPendingDatagrams())
    {
    	//Resize datagram variable with size pending
    	datagram.clear();
        datagram.resize(socket_->pendingDatagramSize());

        //Reading socket datagrams
        socket_->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QDataStream stream(&datagram, QIODevice::ReadOnly);
        Type::AbstractType type;
        stream >> type;

        if (type != Type::Sequence)
            throw SnmpException("Wrong response");

        quint32 bytesRead = 0;
        SnmpMessage snmpMessage;
        bytesRead += snmpMessage.decode(stream);

        //Get response information in PDU sequence
        Type::MSnmpObject mapObjects;
        VarbindList *varbindList = snmpMessage.getProtocolDataUnit()->getVarbindList();
        foreach (Varbind *varbind, varbindList->getVarbinds()) {
//            responses += QString("%1 = %2 : %3")
//                    .arg(varbind->getObjectIdentifier()->toString())
//                    .arg(SnmpTypeFactory::getTypeName(varbind->getValue()->getType()))
//                    .arg(varbind->getValue()->toString());
            mapObjects.insert(varbind->getObjectIdentifier()->toString(), varbind->getValue());
        }

        LogInfo << "SNMP Trap received" << datagram.toHex().data();
        LogInfo << snmpMessage.toString();

        //Create trap information data
        SnmpTrapDataPriv *data = new SnmpTrapDataPriv();
        data->setVersion(snmpMessage.getSnmpVersion()->getVersion());
        data->setCommunity(snmpMessage.getCommunity()->toString());
        data->setEnterpriseOid(snmpMessage.getProtocolDataUnit()->getTrapId()->toString());
        data->setAgentAddr(snmpMessage.getProtocolDataUnit()->getAgentAddr()->toString());
        data->setSpecificTrap(snmpMessage.getProtocolDataUnit()->getSpecificTrap()->getValue());

        //Setting variables bindings
        data->setValueList(mapObjects);

        //Emit response event for registered listeners
        emit eventSnmpReceiveTrap(QSharedPointer<SnmpTrapData>(data));

        //Clean response list
        responseList_.clear();
    }
}
