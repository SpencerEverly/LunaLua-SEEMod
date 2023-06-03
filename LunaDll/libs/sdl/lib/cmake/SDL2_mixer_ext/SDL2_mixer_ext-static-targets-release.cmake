#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SDL2_mixer_ext::SDL2_mixer_ext_Static" for configuration "Release"
set_property(TARGET SDL2_mixer_ext::SDL2_mixer_ext_Static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SDL2_mixer_ext::SDL2_mixer_ext_Static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/SDL2_mixer_ext-static.lib"
  )

list(APPEND _cmake_import_check_targets SDL2_mixer_ext::SDL2_mixer_ext_Static )
list(APPEND _cmake_import_check_files_for_SDL2_mixer_ext::SDL2_mixer_ext_Static "${_IMPORT_PREFIX}/lib/SDL2_mixer_ext-static.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
