#ifndef MPANELLIBRARY_H
#define MPANELLIBRARY_H

#include "mglobal.h"
#include "mpanel.h"


class MPanelLibrary : public MPanel
{
    Q_OBJECT

public:
    explicit MPanelLibrary(QWidget *parent = nullptr);

private:
    void initializeLayout();

public:
    void setLibrary(MMediaLibrary *library);

    void allowCopyMedia(bool allow);
    void allowOrganizeMedia(bool allow);
    void allowAutoBackups(bool allow);

    bool valCopyMedia() const;
    bool valOrganizeMedia() const;
    bool valAutoBackups() const;

private:
    MMediaLibrary *m_mediaLibrary = nullptr;

private:
    MPanelSection *m_sectionLibrary = nullptr;
    MPanelSection *m_sectionMedia = nullptr;
    MPanelSection *m_sectionBackup = nullptr;

    QLineEdit *m_boxLibName = nullptr;
    QLabel *m_lblLibAddedTag = nullptr;
    QLabel *m_lblLibAdded = nullptr;
    QPushButton *m_btnLibImport = nullptr;
    QPushButton *m_btnLibExport = nullptr;
    QLineEdit *m_boxLibPath = nullptr;
    QLabel *m_lblLibPathTag = nullptr;
    QPushButton *m_btnSetLibPath = nullptr;

    QCheckBox *m_optCopyMedia = nullptr;
    QCheckBox *m_optOrganizeMedia = nullptr;
    QLabel *m_lblMediaSizeTag = nullptr;
    QLabel *m_lblMediaSize = nullptr;
    QLabel *m_lblMediaFileCount = nullptr;
    QLabel *m_lblMediaLocTag = nullptr;
    QLineEdit *m_boxMediaLoc = nullptr;
    QPushButton *m_btnSetMediaLoc = nullptr;

    QCheckBox *m_optBackupLibrary = nullptr;
    QLabel *m_lblBackupFreqTag = nullptr;
    QComboBox *m_cbxBackupFreq = nullptr;
    QPushButton *m_btnBackupManual = nullptr;
    QPushButton *m_btnBackupRestore = nullptr;
    QLabel *m_lblBackupLocTag = nullptr;
    QLineEdit *m_boxBackupLoc = nullptr;
    QPushButton *m_btnSetBackupLoc = nullptr;

signals:
    void libImport();
    void libExport();
    void libSetSavePath();
    void libSetMediaPath();
    void libSetBackupPath();
};

#endif













