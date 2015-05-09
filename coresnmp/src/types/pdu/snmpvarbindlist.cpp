#include "types/pdu/snmpvarbindlist.h"

#include "types/basic/snmpobjectid.h"
#include "types/pdu/snmpvarbind.h"

#include <QStringList>

VarbindList::VarbindList(QObject *parent) :
    Sequence(parent)
{
}

VarbindList::VarbindList(const SequenceData &sequenceData, QObject *parent) :
    Sequence(sequenceData, parent)
{
}

QList<Varbind *> VarbindList::getVarbinds() const
{
    QList<Varbind *> varbinds;
    foreach (SnmpBasicAbstractType *asnData, getSequenceData())
        varbinds << static_cast<Varbind *>(asnData);

    return varbinds;
}

QString VarbindList::toString()
{
	QString str;

	str.append(QString("      variable-bindings: %1 items\n").arg(getSequenceData().size()));
    foreach (SnmpBasicAbstractType *asnData, getSequenceData()) {
    	Varbind *varbind = static_cast<Varbind *>(asnData);
    	QString strObjectName = varbind->getObjectIdentifier()->toString(); //TODO Falta obtener el object name
    	QString strOid = varbind->getObjectIdentifier()->toString();
    	QString strValue = varbind->getValue()->toString();
    	str.append(QString("        %1 (%2): %3\n").arg(strObjectName).arg(strOid).arg(strValue));
    	str.append(QString("           Object Name: %1 (%2)\n").arg(strOid).arg(strObjectName));
    	if (strOid.contains("."))
    		str.append(QString("              Scalar Instance Index: %1\n").arg(strOid.split(".").last()));
    	str.append(QString("           %1: %2\n").arg(strObjectName).arg(strValue));
	}

	return str;
}
