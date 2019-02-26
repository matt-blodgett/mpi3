#pragma once

#ifndef MDESKTOP_H
#define MDESKTOP_H


#ifdef MPI3_SHARED_LIBRARY_UI
#   define MPI3_EXPORT_UI Q_DECL_EXPORT
#else
#   define MPI3_EXPORT_UI Q_DECL_IMPORT
#endif


#include <QStandardPaths>
static const QString PathDesktop2 = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
static const QString PathAppData2 = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/mpi3desktop";


#endif
