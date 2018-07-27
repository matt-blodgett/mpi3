#include "xmlsettings.h"


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


bool readSettingsXml(QIODevice &device, QMap<QString, QVariant> &map)
{
    QXmlStreamReader xml(&device);
    XmlNode *curNode = nullptr;

    while(!xml.atEnd())
    {
        switch(xml.readNext())
        {
            case QXmlStreamReader::StartElement:
                if(curNode != nullptr)
                    //we're already processing the file if there already is a current node
                    curNode = new XmlNode(xml.name().toString(), QString(), curNode);
                else if(xml.name().toString() == rootName)
                    //no current node? this must be the first one: the root
                    curNode = new XmlNode(rootName);
                else
                    return false; // invalid format: first element *must* be root tag

                break;

            case QXmlStreamReader::EndElement:
                //if current node has no parent, that means we just closed the root tag
                //we're done!
                if(!curNode->parent())
                {
                    delete curNode;
                    return true;
                }

                //otherwise, we just closed the current category.
                //on the next loop iteration, we should get either the start of the next category or the closing tag of the parent (either the parent category or the "parent" leaf name)
                else
                    curNode = static_cast<XmlNode*>(QScopedPointer<XmlNode>(curNode)->parent());
//                    curNode = (XmlNode*) QScopedPointer<XmlNode>(curNode)->parent();

                break;

            case QXmlStreamReader::Characters:
                if(!xml.isWhitespace())
                    map[curNode->fullPath()] = xml.text().toString();
                break;
        }
    }

    map.clear();
    return false;
}

bool writeSettingsXml(QIODevice &device, const QMap<QString, QVariant> &map)
{
    XmlNode *root = new XmlNode(rootName);

    foreach(const QString &unsplitKey, map.keys())
    {
        QStringList segs = unsplitKey.split("/", QString::SkipEmptyParts);
        QString val = map[unsplitKey].toString();

        XmlNode *cur = root;

        for(int i = 0; i < segs.length(); i++)
        {
            if(i == segs.length() - 1)
            {
                new XmlNode(segs[i], val, cur);
            }
            else
            {
                XmlNode *foundItem = nullptr;
                foreach(QObject *object, cur->children())
                {
                    XmlNode *child = static_cast<XmlNode*>(object);
//                    XmlNode *child = (XmlNode*) object;
                    if(0 == QString::compare(child->tagName, segs[i], Qt::CaseInsensitive))
                    {
                        foundItem = child;
                        break;
                    }
                }

                if(!foundItem)
                    foundItem = new XmlNode(segs[i], QString(), cur);

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

    while(true)
    {
        //see step 1
        XmlNode *cur;
        while((cur = stack.takeLast()) == nullptr)
        {
            xml.writeEndElement();

            if(stack.isEmpty())
            {
                xml.writeEndDocument();
                delete root;
                return true;
            }
        }

        //see step 2
        xml.writeStartElement(cur->tagName);
        stack << nullptr; // required to close text-only elements as well as for nodes with children to go back up a level when children are processed.

        //see step 3
        if(cur->children().size() == 0)
            xml.writeCharacters(cur->subtext);
        else
            for(int i = 0; i < cur->children().length(); i++)
                stack << static_cast<XmlNode*>(cur->children()[i]);
//                stack << (XmlNode*) cur->children()[i];
    }

    //should *never* get here
    return false;
}
