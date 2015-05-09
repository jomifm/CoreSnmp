#pragma once

#include <snmp/snmpdatatypes.h>
#include <snmp/snmprequest.h>

#include "snmpdata_p.h"

#include <QSharedPointer>

class QUdpSocket;

class SnmpRequestPriv : public SnmpRequest
{
    Q_OBJECT

public:
    typedef QMap<quint32, QSharedPointer<SnmpData> > MRequestObject;
    typedef MRequestObject::iterator IRequestObject;

public:
    SnmpRequestPriv(QObject *parent = 0);

private:
    QUdpSocket *socket_;
    MRequestObject mapRequests;

    void get(const QStringList &peerList, const QStringList &objectList);
    void set(const QStringList &peerList, const Type::MSnmpObject &objectMap);
    void bulk(const QStringList &peerList, const QStringList &objectList);
    void walk(const QStringList &peerList, const QString &objectId);

    void request (QSharedPointer<SnmpData> snmpData);

private Q_SLOTS:
    void readPendingDatagram();

};

