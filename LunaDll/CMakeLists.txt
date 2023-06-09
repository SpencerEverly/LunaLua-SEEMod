
# Set C++11 standard
set(CMAKE_CXX_STANDARD 11)

add_definitions(
    -DUNICODE
    -D_UNICODE
    -DNOMINMAX
)

set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")

include(libs/PGE_File_Formats/pge_file_library.cmake)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/boost
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/luabind-include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/lua/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/sdl/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/freeimage
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/luabind-lib
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/lua/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/sdl/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/lib/Release/Win32
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/freeimage
)

set(LunaLua_LibsSources
    libs/IniProcessor/ini_processing.cpp

    libs/DirManager/dirman.cpp
    libs/DirManager/dirman_winapi.cpp

    libs/Utils/files.cpp

    libs/fmt/fmt_format.cpp
    libs/fmt/fmt_ostream.cpp
    libs/fmt/fmt_posix.cpp
    libs/fmt/fmt_printf.cpp

    libs/luasocket/luasocket.cpp
    libs/luasocket/mime.cpp

    libs/stackwalker/StackWalker.cpp

    libs/luasocket/auxiliar.c
    libs/luasocket/buffer.c
    libs/luasocket/except.c
    libs/luasocket/inet.c
    libs/luasocket/io.c
    libs/luasocket/options.c
    libs/luasocket/select.c
    libs/luasocket/tcp.c
    libs/luasocket/timeout.c
    libs/luasocket/udp.c
    libs/luasocket/wsocket.c
)

set(LunaLua_Sources
    Autocode/Autocode.cpp
    Autocode/AutocodeManager.cpp
    Autocode/Commands/AC_HeartSystem.cpp
    Autocode/Commands/AC_LunaControl.cpp
    Autocode/Commands/EnumCmd.cpp
    Autocode/Commands/GenComp.cpp
    CellManager/CellManager.cpp
    CustomSprites/CSprite.cpp
    CustomSprites/CSpriteManager.cpp
    CustomSprites/Hitbox/Hitbox.cpp
    CustomSprites/SpriteComponent.cpp
    CustomSprites/SpritesFuncs/SpriteBehaviorFuncs.cpp
    CustomSprites/SpritesFuncs/SpriteDrawFuncs.cpp
    EventStateMachine.cpp
    FileManager/config_manager.cpp
    FileManager/CustomParamStore.cpp
    FileManager/LoadFile_Level.cpp
    FileManager/SMBXFileManager.cpp
    GameConfig/GameAutostart.cpp
    GameConfig/GameConfiguration.cpp
    GameConfig/GeneralLunaConfig.cpp
    GlobalFuncs.cpp
    Globals.cpp
    HardcodedGraphics/HardcodedGraphicsManager.cpp
    HardcodedGraphics/HardocodeGFXMap.cpp
    Input/Input.cpp
    IPC/IPCPipeServer.cpp
    LevelCodes/dlltestlvlCode.cpp
    LevelCodes/Docopoper-AbstractAssault.lvl.cpp
    LevelCodes/Docopoper-Calleoca.cpp
    LevelCodes/Docopoper-TheFloorisLava.lvl..cpp
    LevelCodes/EuroShellRandD.lvl.cpp
    LevelCodes/JosephStaleknight-CurtainTortoise.lvl.cpp
    LevelCodes/KilArmoryCode.cpp
    LevelCodes/Kil-DemosBrain.cpp
    LevelCodes/SAJewers-QraestoliaCaverns.lvl.cpp
    LevelCodes/SAJewers-Snowboardin.cpp
    LevelCodes/Talkhaus-Science_Final_Battle.cpp
    LuaMain/LuaHelper.cpp
    LuaMain/LuaProxyComponent/LuaProxyAnimation.cpp
    LuaMain/LuaProxyComponent/LuaProxyAsyncHTTPRequest.cpp
    LuaMain/LuaProxyComponent/LuaProxyAudio.cpp
    LuaMain/LuaProxyComponent/LuaProxyBlock.cpp
    LuaMain/LuaProxyComponent/LuaProxyConsole.cpp
    LuaMain/LuaProxyComponent/LuaProxyData.cpp
    LuaMain/LuaProxyComponent/LuaProxyFileFormats.cpp
    LuaMain/LuaProxyComponent/LuaProxyGlobalFunctions.cpp
    LuaMain/LuaProxyComponent/LuaProxyGlobalFunctions/LuaProxyGlobalFuncEffects.cpp
    LuaMain/LuaProxyComponent/LuaProxyGlobalFunctions/LuaProxyGlobalFuncGraphics.cpp
    LuaMain/LuaProxyComponent/LuaProxyGlobalFunctions/LuaProxyGlobalFuncLevel.cpp
    LuaMain/LuaProxyComponent/LuaProxyGlobalFunctions/LuaProxyGlobalFuncMisc.cpp
    LuaMain/LuaProxyComponent/LuaProxyGlobalFunctions/LuaProxyGlobalFuncNative.cpp
    LuaMain/LuaProxyComponent/LuaProxyGlobalFunctions/LuaProxyGlobalFuncText.cpp
    LuaMain/LuaProxyComponent/LuaProxyLayer.cpp
    LuaMain/LuaProxyComponent/LuaProxyLogger.cpp
    LuaMain/LuaProxyComponent/LuaProxyMusicbox.cpp
    LuaMain/LuaProxyComponent/LuaProxyNativeInput.cpp
    LuaMain/LuaProxyComponent/LuaProxyNPC.cpp
    LuaMain/LuaProxyComponent/LuaProxyPath.cpp
    LuaMain/LuaProxyComponent/LuaProxyPlayerSettings.cpp
    LuaMain/LuaProxyComponent/LuaProxySaveBank.cpp
    LuaMain/LuaProxyComponent/LuaProxyScenery.cpp
    LuaMain/LuaProxyComponent/LuaProxySection.cpp
    LuaMain/LuaProxyComponent/LuaProxyShader.cpp
    LuaMain/LuaProxyComponent/LuaProxyTile.cpp
    LuaMain/LuaProxyComponent/LuaProxyWarp.cpp
    LuaMain/LuaProxyComponent/LuaProxyWorld.cpp
    LuaMain/LuaProxyComponent/LuaProxyWorldLevel.cpp
    LuaMain/LuaProxy.cpp
    LuaMain/LuaProxyFFI.cpp
    LuaMain/LuaProxyFFIGraphics.cpp
    LuaMain/LuaSharedProxy.cpp
    LuaMain/LunaLuaMain.cpp
    Main.cpp
    MciEmulator/mciEmulator.cpp
    Minigames/CGUI/CGUIContainer.cpp
    Minigames/GameboyRPG.cpp
    Minigames/Minigames.cpp
    Misc/AsyncHTTPClient.cpp
    Misc/ErrorReporter.cpp
    Misc/FreeImageUtils/FreeImageData.cpp
    Misc/FreeImageUtils/FreeImageGifData.cpp
    Misc/FreeImageUtils/FreeImageHelper.cpp
    Misc/FreeImageUtils/FreeImageInit.cpp
    Misc/Gui/GuiCrashNotify.cpp
    Misc/Gui/RichTextDialog.cpp
    Misc/LoadScreen.cpp
    Misc/MemoryScanner/MemoryScanner.cpp
    Misc/MiscFuncs.cpp
    Misc/NpcIdExtender.cpp
    Misc/PerfTracker.cpp
    Misc/PGEEditorCmdSender.cpp
    Misc/Playground.cpp
    Misc/ResourceFileMapper.cpp
    Misc/RuntimeHookComponents/RuntimeHookCharacterId.cpp
    Misc/RuntimeHookComponents/RuntimeHookFixups.cpp
    Misc/RuntimeHookComponents/RuntimeHookGeneral.cpp
    Misc/RuntimeHookComponents/RuntimeHookHooks.cpp
    Misc/RuntimeHookComponents/RuntimeHookNpcHarm.cpp
    Misc/RuntimeHookComponents/RuntimeHookPublicFunctions.cpp
    Misc/RuntimeHook.cpp
    Misc/RuntimeHookManagers/LevelHUDController.cpp
    Misc/RuntimeHookUtils/APIHook.cpp
    Misc/SafeFPUControl.cpp
    Misc/SHMemServer.cpp
    Misc/TestMode.cpp
    Misc/TestModeMenu.cpp
    Misc/TypeLib.cpp
    Misc/UniPath.cpp
    Misc/VariantHelper.cpp
    Misc/VB6StrPtr.cpp
    Misc/WaitForTickEnd.cpp
    Misc/win32_Unicode.cpp
    Rendering/AsyncGifRecorder.cpp
    Rendering/BitBltEmulation.cpp
    Rendering/FrameCapture.cpp
    Rendering/GL/GLContextManager.cpp
    Rendering/GL/GLDraw.cpp
    Rendering/GL/GLEngineCmds.cpp
    Rendering/GL/GLEngine.cpp
    Rendering/GL/GLEngineProxy.cpp
    Rendering/GL/GLFramebuffer.cpp
    Rendering/GL/GLInitTest.cpp
    Rendering/GL/GLSplitSprite.cpp
    Rendering/GL/GLSprite.cpp
    Rendering/GL/GLTextureStore.cpp
    Rendering/ImageLoader.cpp
    Rendering/LunaImage.cpp
    Rendering/Rendering.cpp
    Rendering/RenderOps/RenderBitmapOp.cpp
    Rendering/RenderOps/RenderEffectOp.cpp
    Rendering/RenderOps/RenderGLOp.cpp
    Rendering/RenderOps/RenderRectOp.cpp
    Rendering/RenderUtils.cpp
    Rendering/Shaders/GLShaderAttributeInfo.cpp
    Rendering/Shaders/GLShader.cpp
    Rendering/Shaders/GLShaderUniformInfo.cpp
    Rendering/Shaders/GLShaderVariableEntry.cpp
    Rendering/Shaders/GLShaderVariableInfo.cpp
    SdlMusic/MusicDefList.cpp
    SdlMusic/MusicManager.cpp
    SdlMusic/SdlMusPlayer.cpp
    SMBXInternal/Animation.cpp
    SMBXInternal/Blocks.cpp
    SMBXInternal/CustomGraphics.cpp
    SMBXInternal/HardcodedGraphicsAccess.cpp
    SMBXInternal/Layer.cpp
    SMBXInternal/Level.cpp
    SMBXInternal/NPCs.cpp
    SMBXInternal/Overworld.cpp
    SMBXInternal/PlayerMOB.cpp
    SMBXInternal/Reconstructed/Util/NpcToCoins.cpp
    SMBXInternal/SMBXEvents.cpp
    SMBXInternal/Sound.cpp
    SMBXInternal/WorldLevel.cpp
    UserSaves/UserSaving.cpp
    VersionString.cpp
    LunaDll.rc
)

add_library(LunaLuaLibShared MODULE
    ${LunaLua_Sources}
    ${LunaLua_LibsSources}
    ${PGE_FILE_LIBRARY_SRCS}
)

set_target_properties(LunaLuaLibShared PROPERTIES
    OUTPUT_NAME LunaDll
    MSVC_RUNTIME_LIBRARY "MultiThreaded"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:>"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:>"
    PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:>"
)

target_compile_definitions(LunaLuaLibShared PRIVATE
    -DWIN32
    -DNDEBUG
    -DWINDOWS
    -D_USRDLL
    -DLUNADLL_EXPORTS
    -D_CRT_SECURE_NO_WARNINGS
    -D_WINSOCK_DEPRECATED_NO_WARNINGS
    -DGLEW_STATIC
)

target_link_libraries(LunaLuaLibShared
    FreeImageLite
    SDL2
    SDL2main
    SDL2_mixer_ext
    glew32s
    kernel32
    user32
    gdi32
    gdiplus
    version
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32
    winmm
    shlwapi
    libluabind
    lua51
    Dbghelp
    Ws2_32
    setupapi
    comsuppw
    opengl32
)
