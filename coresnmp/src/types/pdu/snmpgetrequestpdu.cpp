#include "types/pdu/snmpgetrequestpdu.h"

SnmpGetRequestPdu::SnmpGetRequestPdu(QObject *parent) :
    ProtocolDataUnit(Type::GetRequestPDU, parent)
{
}

SnmpGetRequestPdu::SnmpGetRequestPdu(Integer *requestId,
		Integer *error, Integer *errorIndex, VarbindList *varbindList) :
    ProtocolDataUnit(Type::GetRequestPDU, requestId, error, errorIndex, varbindList)
{
}

SnmpGetNextRequestPdu::SnmpGetNextRequestPdu(QObject *parent) :
    ProtocolDataUnit(Type::GetNextRequestPDU, parent)
{
}

SnmpGetNextRequestPdu::SnmpGetNextRequestPdu(Integer *requestId,
		Integer *error, Integer *errorIndex, VarbindList *varbindList) :
    ProtocolDataUnit(Type::GetNextRequestPDU, requestId, error, errorIndex, varbindList)
{
}

SnmpGetBulkRequestPdu::SnmpGetBulkRequestPdu(QObject *parent) :
		ProtocolDataUnit(Type::GetBulkRequestPDU, parent)
{
}

SnmpGetBulkRequestPdu::SnmpGetBulkRequestPdu(Integer *requestId,
		Integer *nonRepeaters, Integer *maxRepetitions, VarbindList *varbindList) :
		ProtocolDataUnit(Type::GetBulkRequestPDU, requestId, nonRepeaters, maxRepetitions, varbindList)
{
}
