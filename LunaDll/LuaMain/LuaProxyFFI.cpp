
#include <cstdlib>
#include <unordered_set>
#include <memory>
#include <vector>
#include <cstring>
#include <Windows.h>
#include <Psapi.h>
#include <filesystem>

#include "../Globals.h"
#include "../GlobalFuncs.h"
#include "../Rendering/LunaImage.h"
#include "../Rendering/FrameCapture.h"
#include "../Rendering/GL/GLTextureStore.h"
#include "../SMBXInternal/NPCs.h"
#include "../SMBXInternal/Blocks.h"
#include "../SMBXInternal/Layer.h"
#include "../SdlMusic/SdlMusPlayer.h"
#include "../FileManager/CustomParamStore.h"
#include "../Misc/TestMode.h"
#include "../Misc/TestModeMenu.h"
#include "../Misc/RuntimeHook.h"
#include "../Misc/MiscFuncs.h"
#include "LunaLuaMain.h"
#include "LuaProxyFFIGraphics.h"
#include "LunaPathValidator.h"

#include "../Rendering/GL/GLEngine.h"
#include "../Rendering/GL/GLEngineProxy.h"

#define FFI_EXPORT(sig) __declspec(dllexport) sig __cdecl

// Prototypes from RuntimeHookCharacterId.cpp
short* getValidCharacterIDArray();
PlayerMOB* getTemplateForCharacter(int id);

extern "C" {
    FFI_EXPORT(void*) LunaLuaAlloc(size_t size) {
        CLunaFFILock ffiLock(__FUNCTION__);
        return malloc(size);
    }

    FFI_EXPORT(void) LunaLuaGlDrawTriangles(const float* vert, const float* tex, unsigned int count) {
        CLunaFFILock ffiLock(__FUNCTION__);
        auto obj = std::make_shared<GLEngineCmd_Draw2DArray>();
        obj->mType = GL_TRIANGLES;
        obj->mVert = vert;
        obj->mTex = tex;
        obj->mCount = count;
        Renderer::Get().GLCmd(obj);
    }

    FFI_EXPORT(void) LunaLuaKillPlayer(short playerIndex)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        native_killPlayer(&playerIndex);
    }

    FFI_EXPORT(void) LunaLuaHarmPlayer(short playerIndex)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        native_harmPlayer(&playerIndex);
    }

    FFI_EXPORT(short*) LunaLuaGetValidCharacterIDArray()
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        return getValidCharacterIDArray();
    }

    FFI_EXPORT(unsigned int) LunaLuaGetTemplateAddressForCharacter(int id)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        return (unsigned int)getTemplateForCharacter(id);
    }

    FFI_EXPORT(int) LunaLuaBlocksTestCollision(unsigned int plAddr, unsigned int blAddr)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        return Blocks::TestCollision((PlayerMOB*)plAddr, (Block*)blAddr);
    }

    FFI_EXPORT(void) LunaLuaBlockRemove(unsigned int idx, short playSoundEffect)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        short doPlaySoundAndEffects = COMBOOL(playSoundEffect);
        native_removeBlock(&idx, &doPlaySoundAndEffects);
    }

    FFI_EXPORT(void) LunaLuaBlockHit(unsigned int blockIdx, short fromUpSide, unsigned short playerIdx, int hittingCount)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        short unkFlag1VB = COMBOOL(fromUpSide);
        native_hitBlock(&blockIdx, &unkFlag1VB, &playerIdx);
        if (hittingCount != -1) {
            Blocks::Get(blockIdx)->RepeatingHits = hittingCount;
        }
    }

    #pragma comment(lib, "psapi.lib")
    struct LunaLuaMemUsageData
    {
        uint32_t totalWorking;
        uint32_t imgRawMem;
        uint32_t imgCompMem;
        uint32_t sndMem;
        double   imgGpuMem;
    };
    FFI_EXPORT(const LunaLuaMemUsageData*) LunaLuaGetMemUsage()
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        static thread_local LunaLuaMemUsageData out;
        static thread_local PROCESS_MEMORY_COUNTERS psmemCounters;

        if (GetProcessMemoryInfo(GetCurrentProcess(), &psmemCounters, sizeof(psmemCounters)))
        {
            out.totalWorking = (psmemCounters.WorkingSetSize + 512) / 1024;
        }
        else
        {
            out.totalWorking = 0;
        }

        out.imgRawMem = (LunaImage::totalRawMem + 512) / 1024;
        out.imgCompMem = (LunaImage::totalCompMem + 512) / 1024;
        out.sndMem = (PGE_Sounds::GetMemUsage() + 512) / 1024;
        out.imgGpuMem = ((double)GLTextureStore::GetTextureMemory()) / 1024.0;

        return &out;
    }


    FFI_EXPORT(const char*) LunaLuaMemReadString(unsigned int addr)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        static thread_local std::string tmp;
        void* ptr = ((&(*(byte*)addr)));

        if (ptr == 0)
        {
            return nullptr;
        }

        tmp = *((VB6StrPtr*)ptr);
        return tmp.c_str();
    }

    FFI_EXPORT(void) LunaLuaMemWriteString(unsigned int addr, const char* str)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        static thread_local VB6StrPtr nullStr;
        void* ptr = ((&(*(byte*)addr)));

        if (str != nullptr)
        {
            *((VB6StrPtr*)ptr) = std::string(str);
        }
        else
        {
            *((VB6StrPtr*)ptr) = nullStr;
        }
    }

    FFI_EXPORT(void) LunaLuaSetBGORenderFlag(bool val)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        gRenderBGOFlag = val;
    }

    FFI_EXPORT(void) LunaLuaSetSizableRenderFlag(bool val)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        gRenderSizableFlag = val;
    }

    FFI_EXPORT(void) LunaLuaSetBackgroundRenderFlag(bool val)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        gRenderBackgroundFlag = val;
    }

    FFI_EXPORT(const char*) LunaLuaGetLevelCustomParams(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        return g_LevelCustomParams.getCharPtrArray()[0];
    }

    FFI_EXPORT(const char**) LunaLuaGetSectionCustomParams(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        return g_SectionCustomParams.getCharPtrArray();
    }

    FFI_EXPORT(const char**) LunaLuaGetNpcCustomParams(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        return g_NpcCustomParams.getCharPtrArray();
    }

    FFI_EXPORT(const char**) LunaLuaGetBgoCustomParams(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        return g_BgoCustomParams.getCharPtrArray();
    }

    FFI_EXPORT(const char**) LunaLuaGetBlockCustomParams(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        return g_BlockCustomParams.getCharPtrArray();
    }

    FFI_EXPORT(const char*) LunaLuaGetDefaultLevelCustomParams(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        static std::string ret; // Static so it's temporarially stored after returning
        ret = g_configManager.mergeExtraSettings(ConfigPackMiniManager::X_LEVELFILE, 0, "{}");
        return ret.c_str();
    }

    FFI_EXPORT(const char*) LunaLuaGetDefaultSectionCustomParams(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        static std::string ret; // Static so it's temporarially stored after returning
        ret = g_configManager.mergeExtraSettings(ConfigPackMiniManager::X_SECTIONS, 0, "{}");
        return ret.c_str();
    }

    FFI_EXPORT(const char*) LunaLuaGetDefaultNpcCustomParams(unsigned int id)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        static std::string ret; // Static so it's temporarially stored after returning
        ret = g_configManager.mergeExtraSettings(ConfigPackMiniManager::NPC, id, "{}");
        return ret.c_str();
    }

    FFI_EXPORT(const char*) LunaLuaGetDefaultBgoCustomParams(unsigned int id)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        static std::string ret; // Static so it's temporarially stored after returning
        ret = g_configManager.mergeExtraSettings(ConfigPackMiniManager::BGO, id, "{}");
        return ret.c_str();
    }

    FFI_EXPORT(const char*) LunaLuaGetDefaultBlockCustomParams(unsigned int id)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        static std::string ret; // Static so it's temporarially stored after returning
        ret = g_configManager.mergeExtraSettings(ConfigPackMiniManager::BLOCKS, id, "{}");
        return ret.c_str();
    }

    struct FFITestModeSettings {
        int playerCount;
        bool showFPS;
        bool godMode;
        STestModePlayerSettings players[2];
        unsigned int entranceIndex;
    };
    FFITestModeSettings testModeSettings;

    FFI_EXPORT(FFITestModeSettings*) LunaLuaGetTestModeSettings(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        static FFITestModeSettings testModeSettings;
        static STestModeSettings settings;
        settings = getTestModeSettings();
        testModeSettings.playerCount = settings.playerCount;
        testModeSettings.showFPS = settings.showFPS;
        testModeSettings.godMode = settings.godMode;
        testModeSettings.players[0] = settings.players[0];
        testModeSettings.players[1] = settings.players[1];
        testModeSettings.entranceIndex = settings.entranceIndex;

        return &testModeSettings;
    }

    FFI_EXPORT(void) LunaLuaSetTestModeSettings(const FFITestModeSettings* pTestModeSettings)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        static STestModeSettings settings;
        settings = getTestModeSettings();

        settings.playerCount = pTestModeSettings->playerCount;
        settings.showFPS = pTestModeSettings->showFPS;
        settings.godMode = pTestModeSettings->godMode;
        settings.players[0] = pTestModeSettings->players[0];
        settings.players[1] = pTestModeSettings->players[1];
        settings.entranceIndex = pTestModeSettings->entranceIndex;

        setTestModeSettings(settings);
    }

    FFI_EXPORT(void) LunaLuaTestModeExit(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        testModeClosePauseMenu(false, true);
    }

    FFI_EXPORT(void) LunaLuaTestModeRestart(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        testModeClosePauseMenu(true, false);
    }

    FFI_EXPORT(void) LunaLuaTestModeContinue(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        testModeClosePauseMenu(false, false);
    }

    FFI_EXPORT(void) LunaLuaTestModeSkip(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        testModeMenuSkipTick();
    }
    
    FFI_EXPORT(void) LunaLuaTestModeDisable(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        testModeDisable();
    }
    
    FFI_EXPORT(KeyMap*) LunaLuaGetRawKeymapArray(void)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        return gRawKeymap;
    }

    FFI_EXPORT(void) LunaLuaShowLayer(short layerIndex, bool noSmoke)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        short noSmokeNative = COMBOOL(noSmoke);
        native_showLayer(&::Layer::Get(layerIndex)->ptLayerName, &noSmokeNative);
    }

    FFI_EXPORT(void) LunaLuaHideLayer(short layerIndex, bool noSmoke)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        short noSmokeNative = COMBOOL(noSmoke);
        native_hideLayer(&::Layer::Get(layerIndex)->ptLayerName, &noSmokeNative);
    }

    FFI_EXPORT(unsigned char*) LunaLuaGetKeyStateArray()
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        return gKeyState;
    }

    typedef struct
    {
        int len;
        char data[1];
    } GameDataStruct;

    static GameDataStruct* gameData;
    static std::mutex gameDataMutex;
    FFI_EXPORT(void) LunaLuaSetGameData(const char* dataPtr, int dataLen)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        std::unique_lock<std::mutex> lck(gameDataMutex);
        if (gameData != nullptr)
        {
            free(gameData);
            gameData = nullptr;
        }
        gameData = (GameDataStruct*)malloc(dataLen + sizeof(int));
        if (gameData != nullptr)
        {
            gameData->len = dataLen;
            ::memcpy(gameData->data, dataPtr, dataLen);
        }
    }

    FFI_EXPORT(GameDataStruct*) LunaLuaGetGameData()
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        std::unique_lock<std::mutex> lck(gameDataMutex);
        if (gameData == nullptr)
        {
            return nullptr;
        }
        GameDataStruct* cpy = (GameDataStruct*)malloc(gameData->len + sizeof(int));
        cpy->len = gameData->len;
        ::memcpy(cpy->data, gameData->data, gameData->len);
        return cpy;
    }

    FFI_EXPORT(void) LunaLuaFreeReturnedGameData(GameDataStruct* cpy)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        if (cpy == nullptr)
        {
            return;
        }
        free(cpy);
    }


    FFI_EXPORT(int) LunaLuaGetSelectedControllerPowerLevel(int playerNum)
    {
        if (gGeneralConfig.getControllerBatteryReporting())
        {
            return (int)gLunaGameControllerManager.getSelectedControllerPowerLevel(playerNum);
        }
        return (int)SDL_JOYSTICK_POWER_UNKNOWN;
    }

    FFI_EXPORT(const char*) LunaLuaGetSelectedControllerName(int playerNum)
    {
        static std::string name;
        name = gLunaGameControllerManager.getSelectedControllerName(playerNum);
        return name.c_str();
    }

    FFI_EXPORT(void) LunaLuaRumbleSelectedController(int playerNum, int ms, float strength)
    {
        if (gGeneralConfig.getControllerRumbleEnabled())
        {
            gLunaGameControllerManager.rumbleSelectedController(playerNum, ms, strength);
        }
    }

    FFI_EXPORT(ExtendedNPCFields*) LunaLuaGetNPCExtendedFieldsArray()
    {
        return NPC::GetRawExtended(0);
    }

    FFI_EXPORT(const char*) LunaLuaGetNPCextendedFieldsStruct()
    {
        return "\
typedef struct ExtendedNPCFields_\
{\
    bool noblockcollision;\
} ExtendedNPCFields;";
    }

    FFI_EXPORT(ExtendedBlockFields*) LunaLuaGetBlockExtendedFieldsArray()
    {
        return Blocks::GetRawExtended(0);
    }

    FFI_EXPORT(const char*) LunaLuaGetBlockExtendedFieldsStruct()
    {
        return "\
typedef struct ExtendedBlockFields_\
{\
    double layerSpeedX;\
    double layerSpeedY;\
    double extraSpeedX;\
    double extraSpeedY;\
} ExtendedBlockFields;";
    }

    FFI_EXPORT(void) LunaLuaSetPlayerFilterBounceFix(bool enable)
    {
        gDisablePlayerFilterBounceFix = !enable;
    }

    FFI_EXPORT(void) LunaLuaSetPlayerDownwardClipFix(bool enable)
    {
        if (enable)
        {
            gDisablePlayerDownwardClipFix.Apply();
        }
        else
        {
            gDisablePlayerDownwardClipFix.Unapply();
        }
    }

    FFI_EXPORT(void) LunaLuaSetNPCDownwardClipFix(bool enable)
    {
        if (enable)
        {
            gDisableNPCDownwardClipFix.Apply();
            //gDisableNPCDownwardClipFixSlope.Apply();
        }
        else
        {
            gDisableNPCDownwardClipFix.Unapply();
            //gDisableNPCDownwardClipFixSlope.Unapply();
        }
    }

    FFI_EXPORT(void) LunaLuaSetFenceBugFix(bool enable) {
        if (enable) {
            for (int i = 0; gFenceFixes[i] != nullptr; i++) {
                gFenceFixes[i]->Apply();
            }
        } else {
            for (int i = 0; gFenceFixes[i] != nullptr; i++) {
                gFenceFixes[i]->Unapply();
            }
        }
    }

    FFI_EXPORT(void) LunaLuaSetFrameTiming(double value)
    {
        return SetSMBXFrameTiming(value);
    }

    FFI_EXPORT(double) LunaLuaGetFrameTiming()
    {
        return GetSMBXFrameTiming();
    }

    typedef struct
    {
        int len;
        char data[1];
    } ReadFileStruct;
    static CachedFileDataWeakPtr<std::vector<char>> g_lunaFileCache;
    static std::unordered_set<std::shared_ptr<std::vector<char>>> g_lunaFileCacheSet;
    static std::mutex readFileMutex;
    FFI_EXPORT(ReadFileStruct*) LunaLuaCachedReadFile(const char* path)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        std::unique_lock<std::mutex> lck(readFileMutex);

        LunaPathValidator::Result* ptr = LunaPathValidator::GetForThread().CheckPath(path);
        if (!ptr) return nullptr;
        path = ptr->path;

        std::wstring wpath = Str2WStr(path);
        CachedFileDataWeakPtr<std::vector<char>>::Entry* cacheEntry = g_lunaFileCache.get(wpath);
        if (cacheEntry == nullptr)
        {
            return nullptr;
        }

        std::shared_ptr<std::vector<char>> data = cacheEntry->data.lock();

        if (!data)
        {
            // No data, try to read the file
            FILE* f = _wfopen(wpath.c_str(), L"rb");
            if (!f)
            {
                return nullptr;
            }
            fseek(f, 0, SEEK_END);
            size_t len = ftell(f);
            rewind(f);

            data = std::make_shared<std::vector<char>>();
            if (len > 0)
            {
                data->resize(len);
                fread(&((*data)[0]), 1, len, f);
            }
            fclose(f);
            cacheEntry->data = data;
        }
        g_lunaFileCacheSet.insert(data);

        ReadFileStruct* cpy = (ReadFileStruct*)malloc(data->size() + sizeof(int));
        cpy->len = data->size();
        ::memcpy(cpy->data, &((*data)[0]), data->size());
        return cpy;
    }

    FFI_EXPORT(void) LunaLuaFreeCachedReadFileData(ReadFileStruct* cpy)
    {
        CLunaFFILock ffiLock(__FUNCTION__);
        if (cpy == nullptr)
        {
            return;
        }
        free(cpy);
    }

    FFI_EXPORT(bool) LunaLuaCachedExists(const char* path)
    {
        CLunaFFILock ffiLock(__FUNCTION__);

        LunaPathValidator::Result* ptr = LunaPathValidator::GetForThread().CheckPath(path);
        if (!ptr) return false;

        std::wstring wpath = Str2WStr(path);
        return gCachedFileMetadata.exists(wpath);
    }
    
    
    
    
    FFI_EXPORT(void) LunaLuaSetWindowTitle(const char* newName)
    {
        // Add "Software Rendered" if using it, like the normal window name code does
        std::string fullName = newName;
        if (gStartupSettings.softwareGL)
        {
            fullName += " (Software Renderer)";
        }

        // Main bit for setting window name
        SetWindowTextW(gMainWindowHwnd,Str2WStr(fullName).c_str());
    }

    static HICON lastSmallIcon = NULL;
    static HICON lastBigIcon = NULL;

    FFI_EXPORT(void) LunaLuaSetWindowIcon(LunaImageRef* img, int iconType)
    {
        if (iconType < 0 || iconType > 2) return; // Invalid icon type

        // Convert passed image to a HBITMAP
        HBITMAP asBitmap = (*img)->asHBITMAP();

        if (asBitmap == nullptr) return;

        // Create an icon out of the image
        ICONINFO iconInfo;

        iconInfo.fIcon = TRUE;
        iconInfo.hbmColor = asBitmap;
        iconInfo.hbmMask = asBitmap;
        iconInfo.xHotspot = 0;
        iconInfo.yHotspot = 0;

        // Apply it to the window
        if (iconType == 0 || iconType == 1) // Small icon
        {
            HICON asIcon = CreateIconIndirect(&iconInfo);
            LPARAM asParam = LPARAM(asIcon);

            SendMessage(gMainWindowHwnd,WM_SETICON,ICON_SMALL,asParam);

            // If we were already using a custom icon, delete it, to avoid memory leaks
            if (lastSmallIcon) DestroyIcon(lastSmallIcon);
            lastSmallIcon = asIcon;
        }
        
        if (iconType == 0 || iconType == 2) // Big icon
        {
            HICON asIcon = CreateIconIndirect(&iconInfo);
            LPARAM asParam = LPARAM(asIcon);
            
            SendMessage(gMainWindowHwnd,WM_SETICON,ICON_BIG,asParam);

            // If we were already using a custom icon, delete it, to avoid memory leaks
            if (lastBigIcon) DestroyIcon(lastBigIcon);
            lastBigIcon = asIcon;
        }
    }
    
    // Utility function to check if the cursor is in the client area
    static bool IsCursorInClientArea(HWND hwnd)
    {
        POINT cursorPoint;
        RECT clientRect;
        return ((hwnd != nullptr) &&
                (GetCursorPos(&cursorPoint) != 0) &&
                (ScreenToClient(gMainWindowHwnd, &cursorPoint) != 0) &&
                (GetClientRect(gMainWindowHwnd, &clientRect) != 0) &&
                (PtInRect(&clientRect, cursorPoint) != 0)
               );
    }
    
    FFI_EXPORT(void) LunaLuaSetCursor(LunaImageRef* img, uint32_t xHotspot, uint32_t yHotspot)
    {
        HCURSOR asCursor = nullptr;

        // Convert if what is passed in is not null
        if ((img != nullptr) && (*img != nullptr))
        {
            // Convert passed image to a HBITMAP
            HBITMAP asBitmap = (*img)->asHBITMAP();

            if (asBitmap == nullptr) return;

            // Limit hotspot location
            uint32_t imgW = (*img)->getW();
            uint32_t imgH = (*img)->getH();
            if (xHotspot > imgW) xHotspot = imgW;
            if (yHotspot > imgH) yHotspot = imgH;

            // Create a cursor out of the image
            ICONINFO cursorInfo;

            cursorInfo.fIcon = FALSE;
            cursorInfo.hbmColor = asBitmap;
            cursorInfo.hbmMask = asBitmap;
            cursorInfo.xHotspot = xHotspot;
            cursorInfo.yHotspot = yHotspot;

            asCursor = CreateIconIndirect(&cursorInfo);
        }

        // Store new cursor setting
        HCURSOR lastCursor = gCustomCursor;
        gCustomCursor = asCursor;

        // Get actual cursor to use if we need one now
        // (translate null to the default cursor)
        HCURSOR activeCursor = asCursor;
        if (activeCursor == nullptr)
        {
            static HCURSOR defaultCursor = LoadCursor(nullptr, IDC_ARROW);
            activeCursor = defaultCursor;
        }

        // If the previous cursor was set, deallocate it
        // Deallocate the old cursor
        if (lastCursor)
        {
            if (GetCursor() == lastCursor)
            {
                // If the last cursor was in use
                SetCursor(activeCursor);
            }
            DestroyIcon(lastCursor);
        }

        // Set immediately if in main window client area
        if (IsCursorInClientArea(gMainWindowHwnd))
        {
            POINT cursorPoint;
            RECT clientRect;
            if ((GetCursorPos(&cursorPoint) != 0) &&
                (ScreenToClient(gMainWindowHwnd, &cursorPoint) != 0) &&
                (GetClientRect(gMainWindowHwnd, &clientRect) != 0) &&
                (PtInRect(&clientRect, cursorPoint) != 0)
                )
            {
                SetCursor(activeCursor);
            }
        }
    }

    FFI_EXPORT(void) LunaLuaSetCursorHide(void)
    {
        gCustomCursorHide = true;

        // Set immediately if in main window client area
        if (IsCursorInClientArea(gMainWindowHwnd))
        {
            SetCursor(nullptr);
        }
    }
    
    
    
    FFI_EXPORT(void) LunaLuaSetWindowPosition(int x, int y)
    {
        // Main bit for setting window position
        SetWindowPos(gMainWindowHwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
    FFI_EXPORT(void) LunaLuaToggleWindowFocus(bool enable)
    {
        // Setting the focus to the window
        if ((enable == true) && (gMainWindowFocused || gStartupSettings.runWhenUnfocused)) {
            gStartupSettings.runWhenUnfocused = true;
        } else if ((enable == false) && (gMainWindowFocused || gStartupSettings.runWhenUnfocused)) {
            gStartupSettings.runWhenUnfocused = false;
        }
    }
    FFI_EXPORT(bool) LunaLuaIsSetToRunWhenUnfocused()
    {
        // Checks to see if the setting to run when unfocused is true or not
        return (bool)gStartupSettings.runWhenUnfocused;
    }
    FFI_EXPORT(void) LunaLuaCenterWindow()
    {
        // This will center the window to the screen that it detects. Useful for auto-moving the window for misc reasons
        RECT rectClient, rectWindow;
        HWND hWnd = gMainWindowHwnd;
        GetClientRect(hWnd, &rectClient);
        GetWindowRect(hWnd, &rectWindow);
        int posx, posy;
        posx = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2,
        posy = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2,

        // When getting everything set, center the window!
        SetWindowPos(hWnd, NULL, posx, posy, 800, 600, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
    FFI_EXPORT(void) LunaLuaSetFullscreen(bool enable)
    {
        // Toggling fullscreen without maximizing the window/double clicking the window.
        if (gMainWindowHwnd != NULL)
        {
            WINDOWPLACEMENT wndpl;
            wndpl.length = sizeof(WINDOWPLACEMENT);
            if (GetWindowPlacement(gMainWindowHwnd, &wndpl))
            {
                if ((wndpl.showCmd == SW_MAXIMIZE) && !enable)
                    ShowWindow(gMainWindowHwnd, SW_RESTORE);
                else if (enable)
                    ShowWindow(gMainWindowHwnd, SW_MAXIMIZE);
            }
        }
    }
    FFI_EXPORT(bool) LunaLuaIsFullscreen()
    {
        if (gMainWindowHwnd != NULL)
        {
            WINDOWPLACEMENT wndpl;
            wndpl.length = sizeof(WINDOWPLACEMENT);
            if (GetWindowPlacement(gMainWindowHwnd, &wndpl))
            {
                if (wndpl.showCmd == SW_MAXIMIZE) {
                    return (bool)true;
                } else {
                    return (bool)false;
                }
            }
        }
    }
    FFI_EXPORT(double) LunaLuaGetXWindowPosition()
    {
        // Get the window position and return it
        RECT rect;
        GetWindowRect(gMainWindowHwnd, &rect);
        int x = rect.left;
        int y = rect.top;
        return (double)x;
    }
    FFI_EXPORT(double) LunaLuaGetYWindowPosition()
    {
        // Get the window position and return it
        RECT rect;
        GetWindowRect(gMainWindowHwnd, &rect);
        int x = rect.left;
        int y = rect.top;
        return (double)y;
    }
    FFI_EXPORT(double) LunaLuaGetXWindowPositionCenter()
    {
        // Get the window position and return it
        RECT rectClient, rectWindow;
        GetClientRect(gMainWindowHwnd, &rectClient);
        GetWindowRect(gMainWindowHwnd, &rectWindow);
        int posx = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2;
        int posy = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2;
        return (double)posx;
    }
    FFI_EXPORT(double) LunaLuaGetYWindowPositionCenter()
    {
        // Get the window position and return it
        RECT rectClient, rectWindow;
        GetClientRect(gMainWindowHwnd, &rectClient);
        GetWindowRect(gMainWindowHwnd, &rectWindow);
        int posx = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2;
        int posy = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2;
        return (double)posy;
    }
    FFI_EXPORT(bool) LunaLuaIsRecordingGIF()
    {
        if (g_GLEngine.GifRecorderIsRunning())
        {
            return (bool)true;
        } else {
            return (bool)false;
        }
    }
    FFI_EXPORT(bool) LunaLuaIsFocused()
    {
        if (gMainWindowFocused) {
            return (bool)true;
        } else {
            return (bool)false;
        }
    }
    FFI_EXPORT(void) LunaLuaSetThreePlayerInputs(bool enable)
    {
        // Toggling on/off 1st player inputs on 3 or more players
        {
            if (!enable)
                gStartupSettings.threePlayerOrMoreSupport = false;
            else if (enable)
                gStartupSettings.threePlayerOrMoreSupport = true;
        }
    }
    FFI_EXPORT(double) LunaLuaGetScreenResolutionWidth()
    {
        // Get the width of the current screen from the monitor and return it
        HMONITOR monitor = MonitorFromWindow(gMainWindowHwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO info;
        info.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(monitor, &info);
        int monitor_width = info.rcMonitor.right - info.rcMonitor.left;
        int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;
        return (double)monitor_width;
    }
    FFI_EXPORT(double) LunaLuaGetScreenResolutionHeight()
    {
        // Get the height of the current screen from the monitor and return it
        HMONITOR monitor = MonitorFromWindow(gMainWindowHwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO info;
        info.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(monitor, &info);
        int monitor_width = info.rcMonitor.right - info.rcMonitor.left;
        int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;
        return (double)monitor_height;
    }
    FFI_EXPORT(void) LunaLuaTestModeEditLevel(const char* filename)
    {
        std::string fullName = filename;
        testModeSetupNewLevel(fullName);
    }
    //FFI_EXPORT(void) LunaLuaDoScreenshotToEpisodePath(const char* filename)
    //{
        //std::wstring fullName = Str2WStr(filename).c_str();
        //std::wstring finalName = (std::wstring)GM_FULLDIR + L"\\" + fullName + std::wstring(L".png");
        //if (g_GLEngine.IsEnabled())
        //{
            //g_GLEngine.TriggerScreenshot([](HGLOBAL globalMem, const BITMAPINFOHEADER* header, void* pData, HWND curHwnd) {
                //::GenerateScreenshot(finalName, *header, pData);
                //GlobalFree(globalMem);
                //return true;
            //});
        //}
    //}
    FFI_EXPORT(bool) LunaLuaInSMASPlusPlus()
    {
        EpisodeListItem* ep = EpisodeListItem::GetRaw(0);
        const std::wstring firstLevel1 = Str2WStr(GM_FULLDIR + "SMB1 - W-1, L-1.lvlx");
        const std::wstring firstLevel2 = Str2WStr(GM_FULLDIR + "SMBLL - W-1, L-1.lvlx");
        const std::wstring firstLevel3 = Str2WStr(GM_FULLDIR + "SMB2 - W-1, L-1.lvlx");
        const std::wstring firstLevel4 = Str2WStr(GM_FULLDIR + "SMB3 - W-1, L-1.lvlx");
        const std::wstring firstLevel5 = Str2WStr(GM_FULLDIR + "SMW - W-1, L-YI1.lvlx");
        const std::wstring firstLevel6 = Str2WStr(GM_FULLDIR + "SMBS - W-1, L-1.lvlx");
        const std::wstring worldFilename = Str2WStr(GM_FULLDIR + "__World Map.wld");
        if( ep->episodeName == "Super Mario All-Stars++"
            && fileExists(firstLevel1)
            && fileExists(firstLevel2)
            && fileExists(firstLevel3)
            && fileExists(firstLevel4)
            && fileExists(firstLevel5)
            && fileExists(firstLevel6)
            && fileExists(worldFilename)
        ) {
            return (bool)true;
        } else {
            return (bool)false;
        }
    }
    FFI_EXPORT(void) LunaLuaSetSMASPlusPlusEpisodeName()
    {
        EpisodeListItem* ep = EpisodeListItem::GetRaw(0);
        ep->episodeName = "Super Mario All-Stars++";
    }
}

void CachedReadFile::clearData()
{
    std::unique_lock<std::mutex> lck(readFileMutex);
    g_lunaFileCacheSet.clear();
}

void CachedReadFile::holdCached(bool isWorld)
{
    std::unique_lock<std::mutex> lck(readFileMutex);
    g_lunaFileCache.hold(isWorld);
}

void CachedReadFile::releaseCached(bool isWorld)
{
    std::unique_lock<std::mutex> lck(readFileMutex);
    g_lunaFileCache.release(isWorld);
}

extern "C" {
    FFI_EXPORT(LunaPathValidator::Result*) LunaLuaMakeSafeAbsolutePath(const char* path)
    {
        if (!path) return nullptr;
        return LunaPathValidator::GetForThread().CheckPath(path);
    }
}
