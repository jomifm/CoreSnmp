#include "snmprequest_p.h"

#include <snmp/snmprequest.h>

#include "configuration/parameters.h"
#include "types/snmpversion.h"
#include "types/snmpmessage.h"
#include "types/basic/snmpnull.h"
#include "types/basic/snmpoctetstring.h"
#include "types/basic/snmpobjectid.h"
#include "types/basic/snmpsequence.h"
#include "types/pdu/snmpgetrequestpdu.h"
#include "types/pdu/snmpgetresponsepdu.h"
#include "types/pdu/snmpsetrequestpdu.h"
#include "types/pdu/snmpvarbind.h"
#include "types/pdu/snmpvarbindlist.h"
#include "types/utils/snmptypefactory.h"

#include <QUdpSocket>
#include <sys/socket.h>
#include <unistd.h>

#ifdef QT_SNMP_DEBUG
#include <QDebug>
#endif

#define BUFFER_SIZE					512*1024

SnmpRequest *SnmpRequest::instance(QObject *parent)
{
	//Instancia los parametros de configuracion
	Parameters::create();
	static SnmpRequest *snmpRequest = new SnmpRequestPriv(parent);
	return snmpRequest;
}

SnmpRequestPriv::SnmpRequestPriv(QObject *parent) :
    SnmpRequest(parent)
{    
	qDebug() << "Creating SnmpRequestPriv object";
	Type::registerMetatypes();

	//Create new udp socket
	socket_ = new QUdpSocket(this);
    socket_->bind(Parameters::getSnmpPortAgent());

	//actualizamos la opcion TTL a 10
    socket_->setSocketOption(QAbstractSocket::MulticastTtlOption, 10);

	//activamos el loopback
    socket_->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 1);

    quint32 len = BUFFER_SIZE;
    setsockopt(socket_->socketDescriptor(), SOL_SOCKET, SO_SNDBUF, &len, sizeof(len));
    setsockopt(socket_->socketDescriptor(), SOL_SOCKET, SO_RCVBUF, &len, sizeof(len));

    connect(socket_,SIGNAL(readyRead()),SLOT(readPendingDatagram()));

    qDebug() << "Socket state:" << socket_->state() << socket_->errorString();
    if (socket_->error() != QAbstractSocket::UnknownSocketError)
    	throw SnmpException(QString("Error opening or configuring socket"));
}

void SnmpRequestPriv::get(const QStringList &peerList, const QStringList &objectList)
{
	SnmpData *snmpData = NULL;
	snmpData = SnmpData::create(Type::GetRequestPDU, peerList, objectList);
	request(QSharedPointer<SnmpData>(snmpData));
}

void SnmpRequestPriv::set(const QStringList &peerList, const Type::MSnmpObject &objectMap)
{
	SnmpData *snmpData = NULL;
	snmpData = SnmpData::create(Type::SetRequestPDU, peerList, objectMap);
	request(QSharedPointer<SnmpData>(snmpData));
}
void SnmpRequestPriv::bulk(const QStringList &peerList, const QStringList &objectList)
{
	SnmpData *snmpData = NULL;
	snmpData = SnmpData::create(Type::GetBulkRequestPDU, peerList, objectList);
	snmpData->setVersion(Type::SNMPv2c);
	snmpData->setRetries(10);
	request(QSharedPointer<SnmpData>(snmpData));
}

void SnmpRequestPriv::walk(const QStringList &peerList, const QString &objectId)
{
	SnmpData *snmpData = NULL;
	QStringList objectList = QStringList() << objectId;
	snmpData = SnmpData::create(Type::GetNextRequestPDU, peerList, objectList);
	snmpData->setObject(objectId);
	request(QSharedPointer<SnmpData>(snmpData));
}

void SnmpRequestPriv::request (QSharedPointer<SnmpData> data)
{
	//Variables declaration
	SnmpDataPriv *snmpData = static_cast<SnmpDataPriv*>(data.data());
    SnmpVersion *version = NULL;
    OctetString *community = NULL;
    Integer *requestId = NULL;
    Integer *error = NULL;
    Integer *errorIndex = NULL;
    ObjectIdentifier *objectIdentifier = NULL;
    SnmpBasicAbstractType *value = NULL;
    VarbindList *varbindList = NULL;
    SequenceData seq;
    ProtocolDataUnit *pdu = NULL;
    qint64 bytesSent = 0;

    //Message initialization
    version = new SnmpVersion(snmpData->getVersion());
    community = new OctetString(snmpData->getCommunity());

    //Setting header values
    if (snmpData->getIdRequest())
    	requestId = new Integer(snmpData->getIdRequest());
    else requestId = new Integer(SnmpData::getNextRequestId());
    error = new Integer(0);

    //Set values related from bulk request
    if (snmpData->getType() == Type::GetBulkRequestPDU)
    	errorIndex = new Integer(snmpData->getRetries());
    else errorIndex = new Integer(0);


    //Check request type
    if (snmpData->getType() == Type::SetRequestPDU)
    {
		//Processing object list
		Type::MSnmpObject valueList = snmpData->getValueList();
		for (Type::ISnmpObject iterVal = valueList.begin();
				iterVal != valueList.end(); ++iterVal)
		{
			//Create object identifier
			objectIdentifier = new ObjectIdentifier(iterVal.key());
			value = static_cast<SnmpBasicAbstractType*>(iterVal.value());

			//Append object identifier in varbind list with null value
			seq << new Varbind(objectIdentifier, value);
		}
    }
    else
    {
		//Processing value list
		QStringList objectList = snmpData->getObjectList();
		for (QStringList::const_iterator iterObj = objectList.begin();
				iterObj != objectList.end(); ++iterObj)
		{
			//Create object identifier
			objectIdentifier = new ObjectIdentifier(*iterObj);
			value = new Null();

			//Append object identifier in varbind list with null value
			seq << new Varbind(objectIdentifier, value);
		}
    }

    //Create PDU element with necesary elements for get-resquest
	varbindList = new VarbindList(seq);
	if (snmpData->getType() == Type::GetRequestPDU)
		pdu = new SnmpGetRequestPdu(requestId, error, errorIndex, varbindList);
	else if (snmpData->getType() == Type::SetRequestPDU)
		pdu = new SnmpSetRequestPdu(requestId, error, errorIndex, varbindList);
	else if (snmpData->getType() == Type::GetNextRequestPDU)
		pdu = new SnmpGetNextRequestPdu(requestId, error, errorIndex, varbindList);
	else if (snmpData->getType() == Type::GetBulkRequestPDU)
		pdu = new SnmpGetBulkRequestPdu(requestId, error, errorIndex, varbindList);
	else {
		throw SnmpException(QString("Request with PDU type wrong."));
	}

    //Create complete snmp message and datagram
    SnmpMessage snmpMessage (version, community, pdu);
    QByteArray datagram = snmpMessage.encode();

    //Add request in pending list
    mapRequests.insert(requestId->getValue(), data);

    //Processing destination peer list
    QStringList peerList = snmpData->getPeerList();
    for (QStringList::const_iterator iterPeer = peerList.begin();
    		iterPeer != peerList.end(); ++iterPeer) {
    	//Send datagram to destination peer
    	bytesSent = socket_->writeDatagram(datagram, QHostAddress(*iterPeer), Parameters::getSnmpPortNms());
    	if (bytesSent != (qint64)datagram.size())
    		throw SnmpException("Error sending datagram in UDP socket");
    }

    qDebug() << "SNMP request sent" << datagram.toHex().data();
    qDebug() << snmpMessage.toString();
}

void SnmpRequestPriv::readPendingDatagram()
{
	//Variables declaration
	SnmpDataPriv *snmpData = NULL;
    QByteArray datagram;
    QHostAddress sender;
    quint16 senderPort;
    QStringList responseList;
    quint32 intRequestId = 0;
    IRequestObject iterRequest;

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

        qDebug() << "SNMP Response received (size:"
        		<< datagram.size() << "): " << datagram.toHex().data();

        quint32 bytesRead = 0;
        SnmpMessage snmpMessage;
        bytesRead += snmpMessage.decode(stream);

        //Get response information in PDU sequence
        Type::MSnmpObject mapValues;
        Type::ErrorMessage errMsg = (Type::ErrorMessage)snmpMessage.getProtocolDataUnit()->getError()->getValue();
        VarbindList *varbindList = snmpMessage.getProtocolDataUnit()->getVarbindList();
        foreach (Varbind *varbind, varbindList->getVarbinds()) {
        	//Getting text information
            //QString("%1 = %2 : %3")
            //        .arg(varbind->getObjectIdentifier()->toString())
            //        .arg(SnmpTypeFactory::getTypeName(varbind->getValue()->getType()))
            //        .arg(varbind->getValue()->toString());
            responseList.append(varbind->getValue()->toString());

            //Getting object values
            mapValues.insert(varbind->getObjectIdentifier()->toString(), varbind->getValue());
        }

        qDebug() << snmpMessage.toString();

        //Check pending request list
        intRequestId = snmpMessage.getProtocolDataUnit()->getRequestId()->getValue();
        iterRequest = mapRequests.find(intRequestId);
        if (iterRequest == mapRequests.end()) {

        	//Not received response related with any pending request
        	qDebug() << "SNMP Response not delivered without pending request operation.";

			//Emit response event for registered listeners
			emit eventSnmpResponse(QSharedPointer<SnmpDataPriv>(new SnmpDataPriv(Type::RequestUnknown)));
        }
        else {

    		//Get object request data
        	snmpData = static_cast<SnmpDataPriv*>(iterRequest->data());

        	//Check when request is a walk
        	if (snmpData->getType() == Type::GetNextRequestPDU)
        	{
        		QString strResponseOid = (mapValues.begin() == mapValues.end())? "" : mapValues.begin().key();

        		//Check errors
        		if (errMsg) {
        			strResponseOid.clear();
        			snmpData->insertValueList(strResponseOid, mapValues.begin().value());
        		}

        		//Check next object id like previous OID in all octects left last
        		if (!strResponseOid.isEmpty() &&
        				strResponseOid.startsWith(snmpData->getObject()))
        		{
        			//Setting new object id for next request and object values received
        			SnmpBasicAbstractType *value = static_cast<SnmpBasicAbstractType *>(mapValues.begin().value());
        			snmpData->setObjectList(QStringList() << mapValues.begin().key());
            		snmpData->insertValueList(strResponseOid, value);

        			//Performing next request with get type
        			request(*iterRequest);
        		}
        		else
        		{
            		//Emit response event for registered listeners
            		emit eventSnmpResponse(*iterRequest);

        			//Removing responsed request
        			mapRequests.erase(iterRequest);
        		}
        	}
        	else
        	{
        		//Setting object values
        		snmpData->setValueList(mapValues);

        		//Emit response event for registered listeners
        		emit eventSnmpResponse(*iterRequest);

    			//Removing responsed request
    			mapRequests.erase(iterRequest);
        	}
        }

        //Clean response list
        responseList.clear();
    }
}
