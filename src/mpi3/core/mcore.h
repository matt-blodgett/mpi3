#pragma once

#ifndef MCORE_H
#define MCORE_H


#ifdef MPI3_SHARED_LIBRARY_CORE
#   define MPI3_EXPORT_CORE Q_DECL_EXPORT
#else
#   define MPI3_EXPORT_CORE Q_DECL_IMPORT
#endif


#endif
