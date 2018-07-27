#ifndef XMLSETTINGS_H
#define XMLSETTINGS_H

#include <QtCore/QtCore>
#include <QtXml/QtXml>

bool readSettingsXml(QIODevice &device, QMap<QString, QVariant> &map);
bool writeSettingsXml(QIODevice &device, const QMap<QString, QVariant> &map);

static const QSettings::Format xmlFormat = QSettings::registerFormat("xml", &readSettingsXml, &writeSettingsXml);
static const QString rootName = "config";


class XmlNode : public QObject
{

public:
    QString tagName, subtext;
    XmlNode(const QString &name, const QString &text = QString(), QObject *parent = nullptr);
    QString fullPath() const;
};


#endif // XMLSETTINGS_H
