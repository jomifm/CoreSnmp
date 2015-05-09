#pragma once

#include "types/pdu/snmppdu.h"

class GetResponsePDU : public ProtocolDataUnit
{
    Q_OBJECT
public:
    explicit GetResponsePDU(QObject *parent = 0);
};
