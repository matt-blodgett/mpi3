#ifndef MPANELLIBRARY_H
#define MPANELLIBRARY_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QSplitter;
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
    QSplitter *m_frmLibrary = nullptr;

    QWidget *m_frmControl = nullptr;
    QPushButton *m_btnLibImport = nullptr;

    QWidget *m_frmDisplay = nullptr;

    QLabel *m_lblPanelTitle = nullptr;

    QLabel *m_lblPofileHeader = nullptr;
    QWidget *m_frmProfileSection = nullptr;

    QLabel *m_lblMediaHeader = nullptr;
    QWidget *m_frmMediaSection = nullptr;

    QLabel *m_lblBackupHeader = nullptr;
    QWidget *m_frmBackupSection = nullptr;

private:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);

};

#endif
