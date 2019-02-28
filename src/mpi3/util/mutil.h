#pragma once

#ifndef MUTIL_H
#define MUTIL_H


#ifdef MPI3_SHARED_LIBRARY_UTIL
#   define MPI3_EXPORT_UTIL Q_DECL_EXPORT
#else
#   define MPI3_EXPORT_UTIL Q_DECL_IMPORT
#endif


#endif
