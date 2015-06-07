#include "xmlreader.h"

#include "utils/logger/utilslogger.h"

#include <qdebug.h>
#include <qdom.h>
#include <qfile.h>
#include <qglobal.h>
#include <qiodevice.h>
#include <qlist.h>
#include <qmap.h>
#include <qstring.h>
#include <qstringlist.h>
#include <string>


XmlReader::XmlReader(QObject *parent)
    : QObject(parent)
{
    LogFuncionBegin;
    LogFuncionEnd;
}

XmlReader::~XmlReader()
{
    LogFuncionBegin;

    LogFuncionEnd;
}

QDomDocument XmlReader::getDomDocument(const QString & strFile)
{
	//The QDomDocument class represents an XML document.
	QDomDocument dom;

	// Load xml file as raw data
	QFile f(strFile);
	if (!f.open(QIODevice::ReadOnly))
	{
	    // Error while loading file
	    LogError << "Error while loading file" << strFile;
	}
	// Set data into the QDomDocument before processing
	dom.setContent(&f);
	f.close();

	return dom;
}

void XmlReader::parsingCfg (const QString & strFile)
{
	LogFuncionBegin;

	QString strProperty, strValue;
	QString strName, strResource;
	QString strObject, strOid, strParent;
	QString strType, strAccess, strDescription;

	//Create DOM document and extract the root markup
	QDomDocument xmlDom = getDomDocument(strFile);
	QDomElement root = xmlDom.documentElement();
	QString rootType = root.tagName();
	LogInfo << "Parsing xml file with root element:" << rootType.toStdString().c_str();

	// Get the first child of the root
	QDomElement domComponent = root.firstChild().toElement();
	while (!domComponent.isNull())
	{
		// Check if the child tag name is COMPONENT
		if (domComponent.tagName() == "property")
		{
	        strName = domComponent.attribute("name", "");
	        strValue = domComponent.text();

	        mapProperties.insert(strName, strValue);
			LogInfo << "Property"
					<< "[Name:" << strName << "]"
	        		<< "[Value:" << strValue << "]";
		}
		else if (domComponent.tagName() == "mapping")
		{
			strResource = domComponent.attribute("resource", "");

			listMappings.append(strResource);
			LogInfo << "Mapping" << "[Resource:" << strName << "]";
		}

	    // Next component
	    domComponent = domComponent.nextSibling().toElement();
	}

	//Mapping resources
	foreach(QString strMibXml, listMappings)
		mappingMib(strMibXml);

	LogFuncionEnd;
}

void XmlReader::mappingMib (const QString & strFile)
{
	QString strTag;
	QString strProperty, strValue;
	QString strName, strResource;
	QString strObject;
	QString strOid, strParent;
	QString strType, strAccess, strDescription;

	//The QDomDocument class represents an XML document.
	QDomDocument xmlBOM;

	// Load xml file as raw data
	QFile f(strFile);
	if (!f.open(QIODevice::ReadOnly))
	{
	    // Error while loading file
	    LogError << "Error while loading file" << strFile;
	}
	// Set data into the QDomDocument before processing
	xmlBOM.setContent(&f);
	f.close();

	// Extract the root markup
	QDomElement root = xmlBOM.documentElement();
	QString rootType = root.tagName();
	LogInfo << "Mapping xml file with root element:" << rootType.toStdString().c_str();

	// Get the first child of the root
	QDomElement domComponent = root.firstChild().toElement();
	while (!domComponent.isNull())
	{
		//Getting Tag name
		strTag = domComponent.tagName();

	    // Check if the child tag name is an Object
		if (strTag == QString("object") ||
			strTag == QString("notification") ||
			strTag == QString("trap"))
	    {
	        strName = domComponent.attribute("name", "");
	        strOid = domComponent.attribute("oid", "");
	        strParent = domComponent.attribute("parent", "");
	        strType = domComponent.attribute("type", "identifier");
	        strAccess = domComponent.attribute("access", "read-only");
	        strDescription = domComponent.attribute("description", "");

	        // Display component data
			strObject = strName;

			//Setting object data
			appendObjectData(strTag, strName, strOid, strParent, strType, strAccess, "", strDescription);

	        // Read each child of the component node
	        QDomElement childElements = domComponent.firstChild().toElement();
	        while (!childElements.isNull())
	        {
	    		//Getting Tag name
	    		strTag = childElements.tagName();

	    	    // Check if the child tag name is an Object
	    		if (strTag == QString("object"))
	    	    {
					strName = childElements.attribute("name", "");
					strOid = childElements.attribute("oid", "");
					strParent = childElements.attribute("parent", strObject);
					strType = childElements.attribute("type", "identifier");
					strAccess = childElements.attribute("access", "read-only");
					strDescription = childElements.attribute("description", "");

					//Setting object data
					appendObjectData(strTag, strName, strOid, strParent, strType, strAccess, "", strDescription);
	    	    }

				// Next child
				childElements = childElements.nextSibling().toElement();
	        }
	    }

	    // Next component
	    domComponent = domComponent.nextSibling().toElement();
	}

	//Refresh oids completing numbers
	Type::IMibObject iter, iterObj;
	for (iter = mapObjects.begin(); iter != mapObjects.end(); ++iter)
	{
		Type::MibObject *mibobj = iter.value();
		if (mibobj->getOid().contains(".")) continue;

		//Searching parent oid
		QString strParentOid = searchOids(mibobj->getParent());
		if (!strParentOid.isEmpty())
			mibobj->setOid(QString("%1.%2").arg(strParentOid).arg(mibobj->getOid()));
	}

	//Process Oid values
	for (Type::IMibObject iter = mapObjects.begin(); iter != mapObjects.end(); ++iter)
	{
		// Display component data
		Type::MibObject *obj = iter.value();
		LogInfo << "\"object\""
				<< "[Name:" << obj->getName() << "]"
				<< "[Oid:" << obj->getOid() << "]"
				<< "[Parent:" << obj->getParent() << "]"
				<< "[Type:" << obj->getType() << "]"
				<< "[Access:" << obj->getAccess() << "]"
				<< "[Status:" << obj->getStatus() << "]"
				<< "[Description:" << obj->getDescription() << "]";
	}
}

QString XmlReader::searchOids(QString strParent)
{
	Type::IMibObject iterObj = mapObjects.find(strParent);
	if (iterObj == mapObjects.end()) return QString();
	else
	{
		QString strOid = "";
		if (!iterObj.value()->getOid().contains("."))
		{
			strOid = searchOids(iterObj.value()->getParent());
			if (!strOid.isEmpty())
				iterObj.value()->setOid(QString("%1.%2").arg(strOid).arg(iterObj.value()->getOid()));
		}

		return iterObj.value()->getOid();
	}
}

const QString MapperMibObjectTypeInteger = QString("interger");
const QString MapperMibObjectTypeString = QString("string");
const QString MapperMibObjectTypeIpAddress = QString("ipaddress");

void XmlReader::appendObjectData(const QString & strTab, const QString & strName,
    		const QString & strOid, const QString & strParent, const QString & type,
			const QString & access, const QString & status, const QString & strDesc)
{
	//Check type
	Type::MibObject *mibObject = new Type::MibObject(this);
	mibObject->setName(strName);
	mibObject->setOid(strOid);
	mibObject->setParent(strParent);
	mibObject->setDescription(strDesc);

	if (type == MapperMibObjectTypeInteger) mibObject->setType(Type::Integer);
	else if (type == MapperMibObjectTypeString) mibObject->setType(Type::OctetString);
	else if (type == MapperMibObjectTypeIpAddress) mibObject->setType(Type::IpAddress);
	else mibObject->setType(Type::ObjectIdentifier);

	if (access == "read-write") mibObject->setAccess(Type::ReadWrite);
	else if (access == "write-only") mibObject->setAccess(Type::WriteOnly);
	else if (access == "not-accessible") mibObject->setAccess(Type::NotAccessible);
	else mibObject->setAccess(Type::ReadOnly);

	if (status == "optional") mibObject->setStatus(Type::Optional);
	else if (status == "obsolete") mibObject->setStatus(Type::Obsolete);
	else if (status == "deprecated") mibObject->setStatus(Type::Deprecated);
	else mibObject->setStatus(Type::Mandatory);

	//Append object into map
	mapObjects.insert(strName, mibObject);

	if (strTab == "notification")
	{
		//Append object into list
		mapNotifications.insert(strName, strOid);
	}
	else if (strTab == "trap")
	{
		//Append object into list
		mapTraps.insert(strName, strOid);
	}

	//TODO Comprobar valores incorrectos y genear excepciones
}
