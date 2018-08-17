#ifndef MPANELDEVICE_H
#define MPANELDEVICE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSplitter;
class QTreeView;
class QPushButton;
class QLineEdit;
class QLabel;
class QAbstractItemModel;
QT_END_NAMESPACE


class MPanelDevice : public QWidget
{
    Q_OBJECT

public:
    explicit MPanelDevice(QWidget *parent = nullptr);
    ~MPanelDevice();
    void setModel(QAbstractItemModel *lib);

private:
    void initializeLayout();

private:
    QSplitter *m_frmSplitter = nullptr;

    QTreeView *m_treeStorageDevices = nullptr;
    QPushButton *m_btnCreateVolume = nullptr;
    QPushButton *m_btnLoadVolume = nullptr;

    QLineEdit *m_boxLibName = nullptr;
    QLabel *m_lblLibAdded = nullptr;

    QLabel *m_lblCurrentDevice = nullptr;
    QWidget *m_frmStorageSpace = nullptr;

private:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
};

#endif
