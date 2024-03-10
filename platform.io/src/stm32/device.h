/*
 * Rad Pro
 * STM32 device definitions
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(FS2011)
#include "fs2011.h"
#elif defined(FS600) || defined(FS1000)
#include "fs600_fs1000.h"
#elif defined(GC01)
#include "gc01.h"
#endif
