#include "version.h"

// The version number is now defined in resource.h, as it now compiled as meta-data too.
// If you want to define your "custom" version number anyway, then set NORMAL_VER_NUM to 0 and modify the LUNALUA_VERSION string.
#define NORMAL_VER_NUM 1

#if NORMAL_VER_NUM
#define LUNALUA_VER_PREFIX   "LUNALUA V"
#define LUNALUA_VER_SUFFIX   " BETA"
const char* LUNALUA_VERSION = LUNALUA_VER_PREFIX LUNALUA_VER_STR LUNALUA_VER_SUFFIX;
#else
const char* LUNALUA_VERSION = "LunaLua SEE Mod";

#endif

// Common PGE Engine and LunaLUA constant to identify which engine is uses to run lunadll.lua script
// This need to have able use same lunadll.lua script with both PGE Engine and LunaLUA.
// PGE Engine going to have similar to LunaLUA API, but some super-special functions like mem() will NOT be made
// at PGE Engine because useless with PGE's opened API.
const char* GAME_ENGINE = "LunaLua";
