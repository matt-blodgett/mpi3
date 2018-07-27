#include "xmlsettings.h"

#include <QXmlStreamReader>


static const QString rootName = "config";

XmlNode::XmlNode(const QString &name, const QString &text, QObject *parent) : QObject(parent){
    tagName = name;
    subtext = text;
}

QString XmlNode::fullPath() const {
    const XmlNode *cur = this;
    QString path = tagName;

    while((cur = static_cast<XmlNode*>(cur->parent())) != nullptr){
        path.prepend(cur->tagName + "\\");
    }
    return path.mid(rootName.size() + 1);
}


bool readSettingsXml(QIODevice &device, QMap<QString, QVariant> &map) {
    QXmlStreamReader xml(&device);
    XmlNode *curNode = nullptr;

    while(!xml.atEnd()) {
        switch(xml.readNext()) {
            case QXmlStreamReader::StartElement:
                if(curNode != nullptr){
                    curNode = new XmlNode(xml.name().toString(), QString(), curNode);
                }
                else if(xml.name().toString() == rootName){
                    curNode = new XmlNode(rootName);
                }
                else{
                    return false;
                }
                break;

            case QXmlStreamReader::EndElement:
                if(!curNode->parent()) {
                    delete curNode;
                    return true;
                }
                else{
                    curNode = static_cast<XmlNode*>(QScopedPointer<XmlNode>(curNode)->parent());
                }
                break;

            case QXmlStreamReader::Characters:
                if(!xml.isWhitespace()){
                    map[curNode->fullPath()] = xml.text().toString();
                }
                break;
            case QXmlStreamReader::NoToken: break;
            case QXmlStreamReader::Invalid: break;
            case QXmlStreamReader::StartDocument: break;
            case QXmlStreamReader::EndDocument: break;
            case QXmlStreamReader::Comment: break;
            case QXmlStreamReader::DTD: break;
            case QXmlStreamReader::EntityReference: break;
            case QXmlStreamReader::ProcessingInstruction: break;
        }
    }

    map.clear();
    return false;
}
bool writeSettingsXml(QIODevice &device, const QMap<QString, QVariant> &map) {
    XmlNode *root = new XmlNode(rootName);

    foreach(const QString &unsplitKey, map.keys()) {
        QStringList segs = unsplitKey.split("/", QString::SkipEmptyParts);
        QString val = map[unsplitKey].toString();

        XmlNode *cur = root;

        for(int i = 0; i < segs.length(); i++) {
            if(i == segs.length() - 1) {
                new XmlNode(segs[i], val, cur);
            }
            else {
                XmlNode *foundItem = nullptr;
                foreach(QObject *object, cur->children()) {
                    XmlNode *child = static_cast<XmlNode*>(object);
                    if(0 == QString::compare(child->tagName, segs[i], Qt::CaseInsensitive)) {
                        foundItem = child;
                        break;
                    }
                }

                if(!foundItem){
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
        while((cur = stack.takeLast()) == nullptr) {
            xml.writeEndElement();

            if(stack.isEmpty()) {
                xml.writeEndDocument();
                delete root;
                return true;
            }
        }

        xml.writeStartElement(cur->tagName);
        stack << nullptr;

        if(cur->children().size() == 0){
            xml.writeCharacters(cur->subtext);
        }
        else {
            for(int i = 0; i < cur->children().length(); i++){
                stack << static_cast<XmlNode*>(cur->children()[i]);
            }
        }
    }

    return false;
}
