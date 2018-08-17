#ifndef MPANELLIBRARY_H
#define MPANELLIBRARY_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSplitter;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QComboBox;
class QTreeView;
class QLabel;
QT_END_NAMESPACE



class MPanelLibrary : public QWidget
{
    Q_OBJECT

public:
    explicit MPanelLibrary(QWidget *parent = nullptr);
    ~MPanelLibrary();

private:
    void initializeLayout();

private:
    QSplitter *m_frmSplitter = nullptr;

    QPushButton *m_btnLibImport = nullptr;
    QPushButton *m_btnLibExport = nullptr;
    QPushButton *m_btnLibReset = nullptr;
    QPushButton *m_btnLibOpenFile = nullptr;

    QLineEdit *m_boxLibName = nullptr;
    QLabel *m_lblLibAdded = nullptr;
    QLabel *m_lblPlistCount = nullptr;
    QLabel *m_lblSongCount = nullptr;
    QLabel *m_lblArtistCount = nullptr;

    QCheckBox *m_optOrganizeMedia = nullptr;
    QPushButton *m_btnSetMediaLoc = nullptr;
    QLineEdit *m_boxMediaLoc = nullptr;
    QLabel *m_lblMediaSize = nullptr;
    QLabel *m_lblMediaFolderCount = nullptr;
    QLabel *m_lblMediaFileCount = nullptr;

    QCheckBox *m_optBackupLibrary = nullptr;
    QPushButton *m_btnSetBackupLoc = nullptr;
    QLineEdit *m_boxBackupLoc = nullptr;
    QComboBox *m_cbxBackupFreq = nullptr;
    QTreeView *m_treeBackupHist = nullptr;

private:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);

};

#endif
