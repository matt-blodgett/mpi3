#ifndef XMLSETTINGS_H
#define XMLSETTINGS_H

#include <QSettings>

QT_BEGIN_NAMESPACE
class QIODevice;
QT_END_NAMESPACE


class Mpi3Settings : public QSettings
{
    Q_OBJECT

public:
    explicit Mpi3Settings(const QString &settingsPath);

public:
    static const QSettings::Format XmlSettingsFormat;

    static bool readSettingsXml(QIODevice &device, QMap<QString, QVariant> &map);
    static bool writeSettingsXml(QIODevice &device, const QMap<QString, QVariant> &map);
};


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


#endif
