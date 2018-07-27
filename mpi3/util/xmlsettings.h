#ifndef XMLSETTINGS_H
#define XMLSETTINGS_H

#include <QSettings>

QT_BEGIN_NAMESPACE
class QIODevice;
QT_END_NAMESPACE


bool readSettingsXml(QIODevice &device, QMap<QString, QVariant> &map);
bool writeSettingsXml(QIODevice &device, const QMap<QString, QVariant> &map);

static const QSettings::Format XmlSettingsFormat = QSettings::registerFormat("xml", &readSettingsXml, &writeSettingsXml);


class XmlNode : public QObject
{
    Q_OBJECT

public:
    explicit XmlNode(const QString &name, const QString &text = QString(), QObject *parent = nullptr);

public:
    QString fullPath() const;
    QString tagName;
    QString subtext;
};


#endif // XMLSETTINGS_H
