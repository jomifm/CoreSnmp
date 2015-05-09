#include "types/pdu/snmpgetresponsepdu.h"

GetResponsePDU::GetResponsePDU(QObject *parent) :
    ProtocolDataUnit(Type::GetResponsePDU, parent)
{
}
