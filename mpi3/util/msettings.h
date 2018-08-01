#ifndef MSETTINGS_H
#define MSETTINGS_H

#include <QSettings>

QT_BEGIN_NAMESPACE
class QIODevice;
QT_END_NAMESPACE


namespace Mpi3
{

class MSettingsXml : public QSettings
{
    Q_OBJECT

public:
    explicit MSettingsXml(const QString &settingsPath);

public:
    static const QSettings::Format XmlSettingsFormat;

    static bool readSettingsXml(QIODevice &device, QMap<QString, QVariant> &map);
    static bool writeSettingsXml(QIODevice &device, const QMap<QString, QVariant> &map);
};


class MSettingsXmlNode : public QObject
{
    Q_OBJECT

public:
    explicit MSettingsXmlNode(
            const QString &name, const QString
            &text = QString(), QObject *parent = nullptr);

public:
    QString fullPath() const;
    QString tagName;
    QString subtext;
};

};

#endif
