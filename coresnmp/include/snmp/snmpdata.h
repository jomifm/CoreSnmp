#pragma once

#include <snmp/snmpdatatypes.h>

#include <QObject>
#include <QString>
#include <QStringList>

class SnmpData : public QObject
{
	Q_OBJECT

	static quint32 snmpRequestId_;
	static Type::Version snmpVersion_;
	static QString snmpRoCommunity_;
	static QString snmpRwCommunity_;
	static QString snmpTrapCommunity_;

public:
	static quint32 getNextRequestId() {
		if (snmpRequestId_ > 65535) snmpRequestId_ = 0;
		return ++snmpRequestId_;
	}
	static Type::Version getSnmpVersion() { return snmpVersion_; }
	static void setSnmpVersion(const Type::Version & idVersion) {
		snmpVersion_ = idVersion;
	}
	static QString getSnmpRoCommunity() { return snmpRoCommunity_; }
	static void setSnmpRoCommunity(const QString & strRoCommunity) {
		snmpRoCommunity_ = strRoCommunity;
	}
	static QString getSnmpRwCommunity() { return snmpRwCommunity_; }
	static void setSnmpRwCommunity(const QString & strRwCommunity) {
		snmpRwCommunity_ = strRwCommunity;
	}
	static QString getSnmpTrapCommunity() { return snmpTrapCommunity_; }
	static void setSnmpTrapCommunity(const QString & snmpTrapCommunity) {
		snmpTrapCommunity_ = snmpTrapCommunity;
	}

	static SnmpData *create(QObject *parent = 0);
	static SnmpData *create(const Type::AbstractType & idType,
			const QStringList & peerList,
			const QStringList & objectList,
			QObject *parent = 0);
	static SnmpData *create(const Type::Version & idVersion,
			const Type::AbstractType & idType,
			const QStringList & peerList,
			const QStringList & objectList,
			QObject *parent = 0);
	static SnmpData *create(const Type::Version & idVersion,
			const QString & strCommunity,
			const Type::AbstractType & idType,
			const QStringList & peerList,
			const QStringList & objectList,
			QObject *parent = 0);
	static SnmpData *create(const Type::Version & idVersion,
			const QString & strCommunity,
			const Type::AbstractType & idType,
			const quint32 & intTimeout,
			const quint32 & intRetries,
			const QStringList & peerList,
			const QStringList & objectList,
			QObject *parent = 0);

	static SnmpData *create(const Type::AbstractType & idType,
			const QStringList & peerList,
			const Type::MSnmpObject & valueList,
			QObject *parent = 0);


	virtual quint32 getIdRequest() const = 0;

	virtual Type::Version getVersion() const = 0;
	virtual void setVersion(const Type::Version & idVersion) = 0;

	virtual QString getCommunity() const = 0;
	virtual void setCommunity(const QString & strCommunity) = 0;

	virtual Type::AbstractType getType() const = 0;
	virtual void setType(const Type::AbstractType & idType) = 0;

	virtual quint32 getTimeout() const = 0;
	virtual void setTimeout(const quint32 & intTimeout) = 0;

	virtual quint32 getRetries() const = 0;
	virtual void setRetries(const quint32 & intRetries) = 0;

	virtual QStringList getPeerList() const = 0;
	virtual void setPeerList(const QStringList & addressList) = 0;

	virtual QString getObject() const = 0;
	virtual void setObject(const QString & strObject) = 0;

	virtual QStringList getObjectList() const = 0;
	virtual void setObjectList(const QStringList & objectList) = 0;

	virtual Type::MSnmpObject getValueList() const = 0;
	virtual void setValueList(const Type::MSnmpObject & valueList) = 0;

	virtual QString getSourceAddress() const = 0;

	virtual Type::ErrorMessage getErrorMsg() const = 0;

protected:
	SnmpData (QObject *parent = 0) : QObject (parent) {}
};
