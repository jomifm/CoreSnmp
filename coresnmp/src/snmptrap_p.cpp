#include "snmptrap_p.h"

#include <snmp/snmptrap.h>

#include "configuration/parameters.h"
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

#include <QUdpSocket>

#ifdef QT_SNMP_DEBUG
#include <QDebug>
#endif

SnmpTrap *SnmpTrap::instance(QObject *parent)
{
	//Instancia los parametros de configuracion
	Parameters::create();

	static SnmpTrap *snmpRequest = new SnmpTrapPriv(parent);
	return snmpRequest;
}

SnmpTrapPriv::SnmpTrapPriv(QObject *parent) :
    SnmpTrap(parent)
{    
	qDebug() << "Creating SnmpTrapPriv object";
	Type::registerMetatypes();

	//Create new udp socket
	socket_ = new QUdpSocket(this);
    socket_->bind(Parameters::getSnmpTrapPortAgent());
    connect(socket_,SIGNAL(readyRead()),SLOT(readPendingDatagram()));

    qDebug() << "Socket state:" << socket_->state() << socket_->errorString();
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
    specificTrap = new Integer(300); //TODO
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
    		iterPeer != peerList.end(); ++iterPeer)
    	//Send datagram to destination peer
    	socket_->writeDatagram(datagram, QHostAddress(*iterPeer), Parameters::getSnmpTrapPortNms());

    qDebug() << "SNMP Trap sent" << datagram.toHex().data();
    qDebug() << snmpMessage.toString();
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
        QStringList responses;
        VarbindList *varbindList = snmpMessage.getProtocolDataUnit()->getVarbindList();
        foreach (Varbind *varbind, varbindList->getVarbinds()) {
            responses += QString("%1 = %2 : %3")
                    .arg(varbind->getObjectIdentifier()->toString())
                    .arg(SnmpTypeFactory::getTypeName(varbind->getValue()->getType()))
                    .arg(varbind->getValue()->toString());

            responseList_.append(varbind->getValue()->toString());
        }

        qDebug() << "SNMP Trap received" << datagram.toHex().data();
        qDebug() << snmpMessage.toString();

        //Emit response event for registered listeners
        emit eventSnmpReceiveTrap(responseList_);

        //Clean response list
        responseList_.clear();
    }
}
