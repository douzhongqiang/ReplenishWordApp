#ifndef HPNEARCHARACTER_GLOBAL_H
#define HPNEARCHARACTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HPNEARCHARACTER_LIBRARY)
#  define HPNEARCHARACTER_EXPORT Q_DECL_EXPORT
#else
#  define HPNEARCHARACTER_EXPORT Q_DECL_IMPORT
#endif

#endif // HPNEARCHARACTER_GLOBAL_H
