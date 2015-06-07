#pragma once

#include <qglobal.h>
#include <qmap.h>
#include <qobjectdefs.h>
#include <qstring.h>
#include <qstringlist.h>

#include "snmp/snmpdata.h"
#include "snmp/snmpdatatypes.h"

class SnmpDataPriv : public SnmpData
{
    Q_OBJECT

private:
    //Request variables
    quint32 idRequest_;
    Type::Version intVersion_;
    QString strCommunity_;
    Type::AbstractType idType_;
    quint32 intTimeout_;
    quint32 intRetries_;
    QStringList peerList_;
    QString strObject_;
    QStringList objectList_;

    //Common variables
    Type::MSnmpObject valueList_;

    //Response variables
    QString strSourceAddress_;
    Type::ErrorMessage idErrorMsg_;

public:
    SnmpDataPriv(QObject *parent = 0);
    SnmpDataPriv(Type::ErrorMessage errMsg, QObject *parent = 0);
    ~SnmpDataPriv();

    quint32 getIdRequest() const { return idRequest_; }
    void setIdRequest(const quint32 & idRequest) { idRequest_ = idRequest; }

    Type::Version getVersion() const { return intVersion_; }
    void setVersion(const Type::Version & intVersion) { intVersion_ = intVersion; }

    QString getCommunity() const { return strCommunity_; }
    void setCommunity(const QString & strCommunity) { strCommunity_ = strCommunity; }

    Type::AbstractType getType() const { return idType_; }
    void setType(const Type::AbstractType & idType) { idType_ = idType; }

    quint32 getTimeout() const { return intTimeout_; }
    void setTimeout(const quint32 & intTimeout) { intTimeout_ = intTimeout; }

    quint32 getRetries() const { return intRetries_; }
    void setRetries(const quint32 & intRetries) { intRetries_ = intRetries; }

    QStringList getPeerList() const { return peerList_; }
    void setPeerList(const QStringList & peerList) { peerList_ = peerList; }

    QString getObject() const { return strObject_; }
    void setObject(const QString & strObject) { strObject_ = strObject; }

    QStringList getObjectList() const { return objectList_; }
    void setObjectList(const QStringList & objectList) { objectList_ = objectList; }

    //Common variables
    Type::MSnmpObject getValueList() const { return valueList_; }
    void setValueList(const Type::MSnmpObject & valueList) {
    	valueList_.clear();
    	insertValueList(valueList);
    }
    void insertValueList(const QString & strKey, QObject *object) {
    	object->setParent(this);
    	valueList_.insert(strKey, object);
    }
    void insertValueList(const Type::MSnmpObject & valueList) {
    	for (Type::ISnmpObject iter = valueList.begin();
    			iter != valueList.end(); ++iter) {
    		iter.value()->setParent(this);
    		valueList_.insert(iter.key(), iter.value());
    	}
    }

    //Response variables
    QString getSourceAddress() const { return strSourceAddress_; }
    void setSourceAddress(const QString & strSourceAddress) { strSourceAddress_ = strSourceAddress; }

    Type::ErrorMessage getErrorMsg() const { return idErrorMsg_; }
    void setErrorMsg(const Type::ErrorMessage & idErrorMsg) { idErrorMsg_ = idErrorMsg; }

protected:
    void timerEvent(QTimerEvent *event);

protected:
    int snmpTimerId_;
    quint32 snmpTimerRetries_;

public:
    void snmpTimerStart();
    void snmpTimerStop();

Q_SIGNALS:
	void eventSnmpRetry();
	void eventSnmpTimeout();
};

class SnmpTrapDataPriv : public SnmpTrapData
{
    Q_OBJECT

private:
    //Trap variables
	Type::Version intVersion_;
    QString strCommunity_;
    QString strEnterpriseOid_;
    QString strAgentAddr_;
    quint32 intSpecificTrap_;
    QStringList peerList_;

    //Common variables
    Type::MSnmpObject valueList_;

    //Response variables
    QString strSourceAddress_;

public:
    SnmpTrapDataPriv(QObject *parent = 0);
    ~SnmpTrapDataPriv();

    Type::Version getVersion() const { return intVersion_; }
    void setVersion(const Type::Version & intVersion) { intVersion_ = intVersion; }

    QString getCommunity() const { return strCommunity_; }
    void setCommunity(const QString & strCommunity) { strCommunity_ = strCommunity; }

    QString getEnterpriseOid() const { return strEnterpriseOid_; }
    void setEnterpriseOid(const QString & strEnterpriseOid) { strEnterpriseOid_ = strEnterpriseOid; }

    QString getAgentAddr() const { return strAgentAddr_; }
    void setAgentAddr(const QString & strAgentAddr) { strAgentAddr_ = strAgentAddr; }

    quint32 getSpecificTrap() const { return intSpecificTrap_; }
    void setSpecificTrap(const quint32 & intSpecificTrap) { intSpecificTrap_ = intSpecificTrap; }

    QStringList getPeerList() const { return peerList_; }
    void setPeerList(const QStringList & peerList) { peerList_ = peerList; }

    //Common variables
    Type::MSnmpObject getValueList() const { return valueList_; }
    void setValueList(const Type::MSnmpObject & valueList) {
    	valueList_.clear();
    	insertValueList(valueList);
    }
    void insertValueList(const QString & strKey, QObject *object) {
    	object->setParent(this);
    	valueList_.insert(strKey, object);
    }
    void insertValueList(const Type::MSnmpObject & valueList) {
    	for (Type::ISnmpObject iter = valueList.begin();
    			iter != valueList.end(); ++iter) {
    		iter.value()->setParent(this);
    		valueList_.insert(iter.key(), iter.value());
    	}
    }

    //Response variables
    QString getSourceAddress() const { return strSourceAddress_; }
    void setSourceAddress(const QString & strSourceAddress) { strSourceAddress_ = strSourceAddress; }
};
