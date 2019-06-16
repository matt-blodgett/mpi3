#pragma once

#ifndef MPANELLIBRARY_H
#define MPANELLIBRARY_H


#include "mpanel.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QCheckBox;
class QComboBox;
class QLineEdit;
QT_END_NAMESPACE


class MPanelLibrary : public MPanel
{
    Q_OBJECT

public:
    explicit MPanelLibrary(QWidget *parent = nullptr);
    void setLibrary(MMediaLibrary *library) override;
    void load(QSettings *settings) override;
    void save(QSettings *settings) override;

private:
    void setLibraryInfo();
    MMediaLibrary *m_mediaLibrary = nullptr;

public:
    void allowCopyMedia(bool allow);
    void allowOrganizeMedia(bool allow);
    void allowAutoBackups(bool allow);

    bool valCopyMedia() const;
    bool valOrganizeMedia() const;
    bool valAutoBackups() const;

public:
    void askLibraryImport();
    void askLibraryExport();
    void askLibrarySavePath();
    void askLibraryMediaPath();
    void askLibraryBackupPath();

    void backupLibrary();
    void resetLibrary();

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
};


#endif
