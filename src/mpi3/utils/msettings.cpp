#include "msettings.h"

#include <QXmlStreamReader>


typedef class MSettingsXmlNode XmlNode;
static const QString RootName = "mpi3config";


MSettingsXml::MSettingsXml(const QString &settingsPath) : QSettings (settingsPath, XmlSettingsFormat)
{

}


const QSettings::Format MSettingsXml::XmlSettingsFormat = QSettings::registerFormat(
            "xml", &MSettingsXml::readSettingsXml, &MSettingsXml::writeSettingsXml);


bool MSettingsXml::readSettingsXml(QIODevice &device, QMap<QString, QVariant> &map)
{
    QXmlStreamReader xml(&device);
    XmlNode *curNode = nullptr;

    while(!xml.atEnd()) {
        switch(xml.readNext()) {
            case QXmlStreamReader::StartElement: {
                if(curNode) {
                    curNode = new XmlNode(xml.name().toString(), QString(), curNode);
                }
                else if(xml.name().toString() == RootName) {
                    curNode = new XmlNode(RootName);
                }
                else {
                    return false;
                }
                break;
            }
            case QXmlStreamReader::EndElement: {
                if(!curNode->parent()) {
                    delete curNode;
                    return true;
                }
                else {
                    curNode = static_cast<XmlNode*>(QScopedPointer<XmlNode>(curNode)->parent());
                }
                break;
            }
            case QXmlStreamReader::Characters: {
                if(!xml.isWhitespace()) {
                    map[curNode->fullPath()] = xml.text().toString();
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    map.clear();
    return false;
}
bool MSettingsXml::writeSettingsXml(QIODevice &device, const QMap<QString, QVariant> &map)
{
    XmlNode *root = new XmlNode(RootName);

    QMap<QString, QVariant>::const_iterator unsplitKey;
    for(unsplitKey = map.begin(); unsplitKey != map.end(); unsplitKey++) {
        QStringList segs = unsplitKey.key().split("/", QString::SkipEmptyParts);
        QString val = unsplitKey.value().toString();

        XmlNode *cur = root;

        for(int i = 0; i < segs.length(); i++) {
            if(i == segs.length() - 1) {
                new XmlNode(segs[i], val, cur);
            }
            else {
                XmlNode *foundItem = nullptr;
                // may cause problems with no foreach
                for(QObject *object : cur->children()) {
                    XmlNode *child = static_cast<XmlNode*>(object);
                    if(0 == QString::compare(child->tagName, segs[i], Qt::CaseInsensitive)) {
                        foundItem = child;
                        break;
                    }
                }

                if(!foundItem) {
                    foundItem = new XmlNode(segs[i], QString(), cur);
                }

                cur = foundItem;
            }
        }
    }

    QXmlStreamWriter xml(&device);
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(-1);
    xml.writeStartDocument();

    QList<XmlNode*> stack;
    stack << root;

    while(true) {
        XmlNode *cur;
        while(!(cur = stack.takeLast())) {
            xml.writeEndElement();

            if(stack.isEmpty()) {
                xml.writeEndDocument();
                delete root;
                return true;
            }
        }

        xml.writeStartElement(cur->tagName);
        stack << nullptr;

        if(cur->children().size() == 0) {
            xml.writeCharacters(cur->subtext);
        }
        else {
            for(int i = 0; i < cur->children().length(); i++) {
                stack << static_cast<XmlNode*>(cur->children()[i]);
            }
        }
    }

    return false;
}


MSettingsXmlNode::MSettingsXmlNode(const QString &name, const QString &text, QObject *parent) : QObject(parent)
{
    tagName = name;
    subtext = text;
}

QString MSettingsXmlNode::fullPath() const
{
    const XmlNode *cur = this;
    QString path = tagName;

    while((cur = static_cast<XmlNode*>(cur->parent()))) {
        path.prepend(cur->tagName + "/");
    }

    return path.mid(RootName.size() + 1);
}
