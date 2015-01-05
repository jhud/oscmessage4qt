#ifndef OSCMS_GLOBAL_H
#define OSCMS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OSCMS_LIBRARY)
#  define OSCMSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define OSCMSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // OSCMS_GLOBAL_H
