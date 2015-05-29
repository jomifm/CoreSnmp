#pragma once

#include <QObject>
#include <QDataStream>
#include <QHash>
#include <QMap>

namespace Type
{
	enum Version {
		SNMPv1 = 0,
		SNMPv2c = 1,
		SNMPv3 = 3
	};

	typedef enum {
		Unknown = 0x00,

		//Object types
		Integer = 0x02,
		OctetString = 0x04,
		Null = 0x05,
		ObjectIdentifier = 0x06,
		Sequence = 0x30,
		IpAddress = 0x40,
		Counter = 0x41,
		Gauge = 0x42,
		TimerTicks = 0x43,
		Opaque = 0x44,
		NullObject = 0xff,

		//PDU types
		GetRequestPDU = 0xa0,
		GetNextRequestPDU = 0xa1,
		GetResponsePDU = 0xa2,
		SetRequestPDU = 0xa3,
		TrapPDU = 0xa4,
		GetBulkRequestPDU = 0xa5,
		InformRequestedPDU = 0xa6,
		NotificationPDU = 0xa7
	} AbstractType;

	typedef enum {
		ReadOnly,
		ReadWrite,
		WriteOnly,
		NotAccessible,
	} AccessType;

	typedef enum {
		Mandatory,
		Optional,
		Obsolete,
		Deprecated,
	} StatusType;

	typedef enum {
		//Standard snmp errors
		NoError = 0,
		TooBig = 1,
		NoSuchName = 2,
		BadValue = 3,
		ReadOnlyError = 4,
		GenErr = 5,
		NoAccess = 6,
		WrongType = 7,
		WrongLenght = 8,
		WrongEncoding = 9,
		WrongValue = 10,
		NoCreation = 11,
		InconsistentValue = 12,
		ResourceUnavailable = 13,
		CommitFailed = 14,
		UndoFailed = 15,
		AuthorizationError = 16,
		NoWritable = 17,
		InconsistentName = 18,

		//Custom errors
		RequestUnknown = 98,
		RequestTimeout = 99,
	} ErrorMessage;

	typedef QMap<QString, QObject*> MSnmpObject;
	typedef MSnmpObject::const_iterator ISnmpObject;

	class MibObject : public QObject
	{
		Q_OBJECT

	public:
		MibObject(QObject *parent = NULL) :  QObject(parent),
			strName_(""),
			strOid_(""),
			strParent_(""),
			type_(Unknown),
			access_(ReadOnly),
			status_(Mandatory),
			strDescription_("") {}

		MibObject &operator=(const MibObject &);

		QString getName() const { return strName_; }
		QString getOid() const { return strOid_; }
		QString getParent() const { return strParent_; }
		AbstractType getType() const { return type_; }
		AccessType getAccess() const { return access_; }
		StatusType getStatus() const { return status_; }
		QString getDescription() const { return strDescription_; }

		void setName(const QString & value) { strName_ = value; }
		void setOid(const QString & value) { strOid_ = value; }
		void setParent(const QString & value) { strParent_ = value; }
		void setType(const AbstractType & value) { type_ = value; }
		void setAccess(const AccessType & value) { access_ = value; }
		void setStatus(const StatusType & value) { status_ = value; }
		void setDescription(const QString & value) { strDescription_ = value; }

	private:
		QString strName_;
		QString strOid_;
		QString strParent_;
		AbstractType type_;
		AccessType access_;
		StatusType status_;
		QString strDescription_;
	};
	typedef QMap<QString, MibObject*> MMibObject;
	typedef MMibObject::const_iterator IMibObject;
}

inline QDataStream &operator >>(QDataStream &inputStream, Type::AbstractType &type) {
	quint8 byte = 0;
	inputStream >> byte;
	type = static_cast<Type::AbstractType>(byte);
	return inputStream;
}
