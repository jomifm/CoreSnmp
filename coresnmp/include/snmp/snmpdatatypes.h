#pragma once

#include <QObject>
#include <QDataStream>
#include <QHash>
#include <QMap>
#include <QMetaType>

namespace Type {

	enum Version {
		SNMPv1 = 0,
		SNMPv2c = 1,
		SNMPv3 = 3
	};

	enum AbstractType {
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
	};

	enum ErrorMessage {
		//Standard snmp errors
		NoError = 0,
		TooBig = 1,
		NoSuchName = 2,
		BadValue = 3,
		ReadOnly = 4,
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
	};

    typedef QMap<QString, QObject*> MSnmpObject;
    typedef MSnmpObject::const_iterator ISnmpObject;

    static void registerMetatypes() {
    	qRegisterMetaType<Type::MSnmpObject>("Type::MSnmpObject");
    	qRegisterMetaType<Type::ISnmpObject>("Type::ISnmpObject");
    }
}

inline QDataStream &operator >>(QDataStream &inputStream, Type::AbstractType &type) {
    quint8 byte = 0;
    inputStream >> byte;
    type = static_cast<Type::AbstractType>(byte);
    return inputStream;
}
