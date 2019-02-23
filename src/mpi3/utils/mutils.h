#pragma once

#ifndef MUTILS_H
#define MUTILS_H

#ifdef MPI3_SHARED_LIBRARY_UTILS
#   define MPI3_EXPORT_UTILS Q_DECL_EXPORT
#else
#   define MPI3_EXPORT_UTILS Q_DECL_IMPORT
#endif

#endif
