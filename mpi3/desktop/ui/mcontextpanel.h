#pragma once

#ifndef MPANEL_H
#define MPANEL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QGridLayout;
class QSplitter;
class QScrollArea;
class QRadioButton;
class QPushButton;
class QTreeView;
class QCheckBox;
class QComboBox;
class QLineEdit;
class QLabel;
QT_END_NAMESPACE


class MPanelSection : public QWidget
{
    Q_OBJECT

public:
    explicit MPanelSection(QWidget *parent = nullptr);

public:
    QGridLayout *gridNorth();
    QGridLayout *gridSouth();
    QGridLayout *gridEast();
    QGridLayout *gridWest();
    QGridLayout *gridBody();
    QGridLayout *gridSection();

private:
    QGridLayout *m_gridNorth = nullptr;
    QGridLayout *m_gridSouth = nullptr;
    QGridLayout *m_gridEast = nullptr;
    QGridLayout *m_gridWest = nullptr;
    QGridLayout *m_gridBody = nullptr;
    QGridLayout *m_gridSection = nullptr;

    QWidget *m_frmBody = nullptr;
    QLabel *m_lblHeader = nullptr;

public:
    QString header() const;
    void setHeader(const QString &header);
};


class MPanelContext : public QWidget
{
    Q_OBJECT

public:
    explicit MPanelContext(QWidget *parent = nullptr);

public:
    void initializeLayoutType(bool sectioned);

public:
    QGridLayout *gridMain();
    QGridLayout *gridControl();
    QGridLayout *gridDisplay();

private:
    QGridLayout *m_gridMain = nullptr;
    QGridLayout *m_gridControl = nullptr;
    QGridLayout *m_gridDisplay = nullptr;

public:
    QString title() const;
    void setTitle(const QString &title);

    MPanelSection *addSection();
    MPanelSection *addSection(const QString &header);

    QLabel *addLabelTag();
    QLabel *addLabelValue();
    QLineEdit *addLineEdit();
    QLineEdit *addLineEditHidden();
    QPushButton *addPushButton();
    QTreeView *addTreeView();
    QCheckBox *addCheckBox();
    QComboBox *addComboBox();
    QRadioButton *addRadioButton();

    void addTreeView(QTreeView *tree);

private:
    QSplitter *m_frmSplitter = nullptr;
    QWidget *m_frmControl = nullptr;
    QWidget *m_frmDisplay = nullptr;
    QScrollArea *m_frmScrollArea = nullptr;
    QLabel *m_lblPanelTitle = nullptr;

protected:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
};



#endif








































