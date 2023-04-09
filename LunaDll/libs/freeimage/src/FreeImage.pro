TEMPLATE = lib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG -= dll
CONFIG += static

DEFINES += VER_MAJOR=3 VER_MINOR=17.0

QMAKE_CFLAGS += -std=c99 -O3 -fPIC -fexceptions -fvisibility=hidden

QMAKE_CFLAGS += -Wno-missing-field-initializers -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable \
                -Wno-sign-compare -Wno-unused-function -Wno-implicit-function-declaration -Wno-pointer-sign \
                -Wno-missing-field-initializers -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable \
                -Wno-parentheses -Wno-switch -Wno-unused-result -Wno-format -Wno-sign-compare -Wno-unused-value \
                -Wno-type-limits -Wno-maybe-uninitialized -Wno-old-style-declaration
QMAKE_CXXFLAGS += -Wno-missing-field-initializers -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable \
                -Wno-sign-compare -Wno-unused-function \
                -Wno-missing-field-initializers -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable \
                -Wno-parentheses -Wno-switch -Wno-unused-result -Wno-format -Wno-unused-value \
                -Wno-type-limits -Wno-maybe-uninitialized -Wno-reorder
macx: QMAKE_CXXFLAGS += -Wno-header-guard
!macx:{
QMAKE_LFLAGS += -Wl,-rpath=\'\$\$ORIGIN\'
}

include (../../_common/lib_destdir.pri)
TARGET = freeimage
include(../../_common/build_props.pri)
DESTDIR = ../_builds/$$TARGETOS/lib

MAKEFILE = Makefile.FreeImageFULL

RC_FILE = FreeImage.rc

INCLUDEPATH += $$PWD/Source

win32:{
    LIBS += -L../_builds/win32/lib -lws2_32
    INCLUDEPATH += ../_builds/win32/include
    DEFINES += WINVER=0x0500 OPJ_STATIC LIBRAW_NODLL FREEIMAGE_LIB FREEIMAGE_EXPORTS DISABLE_PERF_MEASUREMENT
    QMAKE_CXXFLAGS += -include stdexcept
    QMAKE_LFLAGS += -Wl,--subsystem,windows:5.0,--major-os-version,5
    QMAKE_CFLAGS += -O3 -fexceptions
    QMAKE_CXXFLAGS += -O3 -fexceptions -Wno-ctor-dtor-privacy
}
linux-g++||unix:!macx:!android:{
    LIBS += -L../_builds/linux/lib
    INCLUDEPATH += ../_builds/linux/include
    CONFIG += unversioned_libname
    QMAKE_CFLAGS += -O3 -fPIC -fexceptions -fvisibility=hidden
    QMAKE_CXXFLAGS += -O3 -fPIC -fexceptions -fvisibility=hidden -Wno-ctor-dtor-privacy
    DEFINES += OPJ_STATIC NO_LCMS NO_JASPER DISABLE_PERF_MEASUREMENT __ANSI__
}
android:{
    LIBS += -L../_builds/android/lib
    INCLUDEPATH += ../_builds/android/include
    warning("NOT PORTED YET!")
}
macx:{
    LIBS += -L../_builds/macos/lib
    INCLUDEPATH += ../_builds/macos/include
    QMAKE_CFLAGS += -Os -fexceptions -fvisibility=hidden
    QMAKE_CXXFLAGS += -Os -fexceptions -fvisibility=hidden
    DEFINES += NO_LCMS
} else {
    #LIBS += -lSDL2
}

LIBS += -L../_builds/$$TARGETOS/lib

#!android:{
#    win32:{
#        LIBS += -lvorbisfile.dll -lvorbis.dll -lmodplug.dll -lFLAC.dll -logg.dll -static-libgcc -static-libstdc++ -static -lpthread
#    } else {
#        LIBS += -lvorbisfile -lvorbis -lmodplug -lFLAC -logg
#    }
#} else {
#    LIBS += -lvorbisfile -lvorbis -lmodplug -logg #-lvorbisidec
#}

FreeImageH.path =  ../_builds/$$TARGETOS/include
FreeImageH.files += Source/FreeImage.h

linux-g++||unix:!macx:!android:{
    FreeImageSO.path = ../_builds/linux/lib
    FreeImageSO.files += ../_builds/sdl2_mixer_mod/*.so*
    INSTALLS =  FreeImageSO
}
macx:{
    FreeImageSO.path = ../_builds/macos/lib
    FreeImageSO.files += ../_builds/sdl2_mixer_mod/*.dylib*
}
android:{
    FreeImageSO.path = ../_builds/android/lib
    FreeImageSO.files += ../_builds/sdl2_mixer_mod/*.so*
    INSTALLS =  FreeImageSO
}
win32: {
    FreeImageSO.path = ../_builds/win32/bin
    FreeImageSO.files += ../_builds/win32/lib/*.dll
    INSTALLS =  FreeImageSO
}
INSTALLS = FreeImageH FreeImageSO

INCLUDEPATH +=  $$PWD/Source \
                $$PWD/Source/Metadata \
                $$PWD/Source/FreeImageToolkit \
                $$PWD/Source/LibJPEG \
                $$PWD/Source/LibPNG \
                $$PWD/Source/LibTIFF4 \
                $$PWD/Source/ZLib \
                $$PWD/Source/LibOpenJPEG \
                $$PWD/Source/OpenEXR \
                $$PWD/Source/OpenEXR/Half \
                $$PWD/Source/OpenEXR/Iex \
                $$PWD/Source/OpenEXR/IlmImf \
                $$PWD/Source/OpenEXR/IlmThread \
                $$PWD/Source/OpenEXR/Imath \
                $$PWD/Source/OpenEXR/IexMath \
                $$PWD/Source/LibRawLite \
                $$PWD/Source/LibRawLite/dcraw \
                $$PWD/Source/LibRawLite/internal \
                $$PWD/Source/LibRawLite/libraw \
                $$PWD/Source/LibRawLite/src \
                $$PWD/Source/LibWebP \
                $$PWD/Source/LibJXR \
                $$PWD/Source/LibJXR/common/include \
                $$PWD/Source/LibJXR/image/sys \
                $$PWD/Source/LibJXR/jxrgluelib

HEADERS += \
    Source/DeprecationManager/DeprecationMgr.h \
    Source/FreeImage/J2KHelper.h \
    Source/FreeImage/PSDParser.h \
    Source/FreeImageToolkit/Filters.h \
    Source/FreeImageToolkit/Resize.h \
    Source/LibJPEG/cderror.h \
    Source/LibJPEG/cdjpeg.h \
    Source/LibJPEG/jconfig.h \
    Source/LibJPEG/jdct.h \
    Source/LibJPEG/jerror.h \
    Source/LibJPEG/jinclude.h \
    Source/LibJPEG/jmemsys.h \
    Source/LibJPEG/jmorecfg.h \
    Source/LibJPEG/jpegint.h \
    Source/LibJPEG/jpeglib.h \
    Source/LibJPEG/jversion.h \
    Source/LibJPEG/transupp.h \
    Source/LibJXR/common/include/guiddef.h \
    Source/LibJXR/common/include/wmsal.h \
    Source/LibJXR/common/include/wmspecstring.h \
    Source/LibJXR/common/include/wmspecstrings_adt.h \
    Source/LibJXR/common/include/wmspecstrings_strict.h \
    Source/LibJXR/common/include/wmspecstrings_undef.h \
    Source/LibJXR/image/decode/decode.h \
    Source/LibJXR/image/encode/encode.h \
    Source/LibJXR/image/sys/ansi.h \
    Source/LibJXR/image/sys/common.h \
    Source/LibJXR/image/sys/perfTimer.h \
    Source/LibJXR/image/sys/strcodec.h \
    Source/LibJXR/image/sys/strTransform.h \
    Source/LibJXR/image/sys/windowsmediaphoto.h \
    Source/LibJXR/image/sys/xplatform_image.h \
    Source/LibJXR/image/x86/x86.h \
    Source/LibJXR/jxrgluelib/JXRGlue.h \
    Source/LibJXR/jxrgluelib/JXRMeta.h \
    Source/LibOpenJPEG/bio.h \
    Source/LibOpenJPEG/cidx_manager.h \
    Source/LibOpenJPEG/cio.h \
    Source/LibOpenJPEG/dwt.h \
    Source/LibOpenJPEG/event.h \
    Source/LibOpenJPEG/function_list.h \
    Source/LibOpenJPEG/image.h \
    Source/LibOpenJPEG/indexbox_manager.h \
    Source/LibOpenJPEG/invert.h \
    Source/LibOpenJPEG/j2k.h \
    Source/LibOpenJPEG/jp2.h \
    Source/LibOpenJPEG/mct.h \
    Source/LibOpenJPEG/mqc.h \
    Source/LibOpenJPEG/openjpeg.h \
    Source/LibOpenJPEG/opj_clock.h \
    Source/LibOpenJPEG/opj_codec.h \
    Source/LibOpenJPEG/opj_config.h \
    Source/LibOpenJPEG/opj_config_private.h \
    Source/LibOpenJPEG/opj_includes.h \
    Source/LibOpenJPEG/opj_intmath.h \
    Source/LibOpenJPEG/opj_inttypes.h \
    Source/LibOpenJPEG/opj_malloc.h \
    Source/LibOpenJPEG/opj_stdint.h \
    Source/LibOpenJPEG/pi.h \
    Source/LibOpenJPEG/raw.h \
    Source/LibOpenJPEG/t1.h \
    Source/LibOpenJPEG/t1_luts.h \
    Source/LibOpenJPEG/t2.h \
    Source/LibOpenJPEG/tcd.h \
    Source/LibOpenJPEG/tgt.h \
    Source/LibPNG/png.h \
    Source/LibPNG/pngconf.h \
    Source/LibPNG/pngdebug.h \
    Source/LibPNG/pnginfo.h \
    Source/LibPNG/pnglibconf.h \
    Source/LibPNG/pngpriv.h \
    Source/LibPNG/pngstruct.h \
    Source/LibRawLite/internal/defines.h \
    Source/LibRawLite/internal/libraw_internal_funcs.h \
    Source/LibRawLite/internal/var_defines.h \
    Source/LibRawLite/libraw/libraw.h \
    Source/LibRawLite/libraw/libraw_alloc.h \
    Source/LibRawLite/libraw/libraw_const.h \
    Source/LibRawLite/libraw/libraw_datastream.h \
    Source/LibRawLite/libraw/libraw_internal.h \
    Source/LibRawLite/libraw/libraw_types.h \
    Source/LibRawLite/libraw/libraw_version.h \
    Source/LibTIFF4/t4.h \
    Source/LibTIFF4/tif_config.h \
    Source/LibTIFF4/tif_config.vc.h \
    Source/LibTIFF4/tif_config.wince.h \
    Source/LibTIFF4/tif_dir.h \
    Source/LibTIFF4/tif_fax3.h \
    Source/LibTIFF4/tif_predict.h \
    Source/LibTIFF4/tiff.h \
    Source/LibTIFF4/tiffconf.h \
    Source/LibTIFF4/tiffconf.vc.h \
    Source/LibTIFF4/tiffconf.wince.h \
    Source/LibTIFF4/tiffio.h \
    Source/LibTIFF4/tiffio.hxx \
    Source/LibTIFF4/tiffiop.h \
    Source/LibTIFF4/tiffvers.h \
    Source/LibTIFF4/uvcode.h \
    Source/LibWebP/src/dec/alphai.h \
    Source/LibWebP/src/dec/decode_vp8.h \
    Source/LibWebP/src/dec/vp8i.h \
    Source/LibWebP/src/dec/vp8li.h \
    Source/LibWebP/src/dec/webpi.h \
    Source/LibWebP/src/dsp/dsp.h \
    Source/LibWebP/src/dsp/lossless.h \
    Source/LibWebP/src/dsp/mips_macro.h \
    Source/LibWebP/src/dsp/neon.h \
    Source/LibWebP/src/dsp/yuv.h \
    Source/LibWebP/src/dsp/yuv_tables_sse2.h \
    Source/LibWebP/src/enc/backward_references.h \
    Source/LibWebP/src/enc/cost.h \
    Source/LibWebP/src/enc/histogram.h \
    Source/LibWebP/src/enc/vp8enci.h \
    Source/LibWebP/src/enc/vp8li.h \
    Source/LibWebP/src/mux/muxi.h \
    Source/LibWebP/src/utils/bit_reader.h \
    Source/LibWebP/src/utils/bit_reader_inl.h \
    Source/LibWebP/src/utils/bit_writer.h \
    Source/LibWebP/src/utils/color_cache.h \
    Source/LibWebP/src/utils/endian_inl.h \
    Source/LibWebP/src/utils/filters.h \
    Source/LibWebP/src/utils/huffman.h \
    Source/LibWebP/src/utils/huffman_encode.h \
    Source/LibWebP/src/utils/quant_levels.h \
    Source/LibWebP/src/utils/quant_levels_dec.h \
    Source/LibWebP/src/utils/random.h \
    Source/LibWebP/src/utils/rescaler.h \
    Source/LibWebP/src/utils/thread.h \
    Source/LibWebP/src/utils/utils.h \
    Source/LibWebP/src/webp/decode.h \
    Source/LibWebP/src/webp/demux.h \
    Source/LibWebP/src/webp/encode.h \
    Source/LibWebP/src/webp/format_constants.h \
    Source/LibWebP/src/webp/mux.h \
    Source/LibWebP/src/webp/mux_types.h \
    Source/LibWebP/src/webp/types.h \
    Source/Metadata/FIRational.h \
    Source/Metadata/FreeImageTag.h \
    Source/OpenEXR/Half/eLut.h \
    Source/OpenEXR/Half/half.h \
    Source/OpenEXR/Half/halfExport.h \
    Source/OpenEXR/Half/halfFunction.h \
    Source/OpenEXR/Half/halfLimits.h \
    Source/OpenEXR/Half/toFloat.h \
    Source/OpenEXR/Iex/Iex.h \
    Source/OpenEXR/Iex/IexBaseExc.h \
    Source/OpenEXR/Iex/IexErrnoExc.h \
    Source/OpenEXR/Iex/IexExport.h \
    Source/OpenEXR/Iex/IexForward.h \
    Source/OpenEXR/Iex/IexMacros.h \
    Source/OpenEXR/Iex/IexMathExc.h \
    Source/OpenEXR/Iex/IexNamespace.h \
    Source/OpenEXR/Iex/IexThrowErrnoExc.h \
    Source/OpenEXR/IexMath/IexMathFloatExc.h \
    Source/OpenEXR/IexMath/IexMathFpu.h \
    Source/OpenEXR/IexMath/IexMathIeeeExc.h \
    Source/OpenEXR/IlmImf/b44ExpLogTable.h \
    Source/OpenEXR/IlmImf/dwaLookups.h \
    Source/OpenEXR/IlmImf/ImfAcesFile.h \
    Source/OpenEXR/IlmImf/ImfArray.h \
    Source/OpenEXR/IlmImf/ImfAttribute.h \
    Source/OpenEXR/IlmImf/ImfAutoArray.h \
    Source/OpenEXR/IlmImf/ImfB44Compressor.h \
    Source/OpenEXR/IlmImf/ImfBoxAttribute.h \
    Source/OpenEXR/IlmImf/ImfChannelList.h \
    Source/OpenEXR/IlmImf/ImfChannelListAttribute.h \
    Source/OpenEXR/IlmImf/ImfCheckedArithmetic.h \
    Source/OpenEXR/IlmImf/ImfChromaticities.h \
    Source/OpenEXR/IlmImf/ImfChromaticitiesAttribute.h \
    Source/OpenEXR/IlmImf/ImfCompositeDeepScanLine.h \
    Source/OpenEXR/IlmImf/ImfCompression.h \
    Source/OpenEXR/IlmImf/ImfCompressionAttribute.h \
    Source/OpenEXR/IlmImf/ImfCompressor.h \
    Source/OpenEXR/IlmImf/ImfConvert.h \
    Source/OpenEXR/IlmImf/ImfCRgbaFile.h \
    Source/OpenEXR/IlmImf/ImfDeepCompositing.h \
    Source/OpenEXR/IlmImf/ImfDeepFrameBuffer.h \
    Source/OpenEXR/IlmImf/ImfDeepImageState.h \
    Source/OpenEXR/IlmImf/ImfDeepImageStateAttribute.h \
    Source/OpenEXR/IlmImf/ImfDeepScanLineInputFile.h \
    Source/OpenEXR/IlmImf/ImfDeepScanLineInputPart.h \
    Source/OpenEXR/IlmImf/ImfDeepScanLineOutputFile.h \
    Source/OpenEXR/IlmImf/ImfDeepScanLineOutputPart.h \
    Source/OpenEXR/IlmImf/ImfDeepTiledInputFile.h \
    Source/OpenEXR/IlmImf/ImfDeepTiledInputPart.h \
    Source/OpenEXR/IlmImf/ImfDeepTiledOutputFile.h \
    Source/OpenEXR/IlmImf/ImfDeepTiledOutputPart.h \
    Source/OpenEXR/IlmImf/ImfDoubleAttribute.h \
    Source/OpenEXR/IlmImf/ImfDwaCompressor.h \
    Source/OpenEXR/IlmImf/ImfDwaCompressorSimd.h \
    Source/OpenEXR/IlmImf/ImfEnvmap.h \
    Source/OpenEXR/IlmImf/ImfEnvmapAttribute.h \
    Source/OpenEXR/IlmImf/ImfExport.h \
    Source/OpenEXR/IlmImf/ImfFastHuf.h \
    Source/OpenEXR/IlmImf/ImfFloatAttribute.h \
    Source/OpenEXR/IlmImf/ImfFloatVectorAttribute.h \
    Source/OpenEXR/IlmImf/ImfForward.h \
    Source/OpenEXR/IlmImf/ImfFrameBuffer.h \
    Source/OpenEXR/IlmImf/ImfFramesPerSecond.h \
    Source/OpenEXR/IlmImf/ImfGenericInputFile.h \
    Source/OpenEXR/IlmImf/ImfGenericOutputFile.h \
    Source/OpenEXR/IlmImf/ImfHeader.h \
    Source/OpenEXR/IlmImf/ImfHuf.h \
    Source/OpenEXR/IlmImf/ImfInputFile.h \
    Source/OpenEXR/IlmImf/ImfInputPart.h \
    Source/OpenEXR/IlmImf/ImfInputPartData.h \
    Source/OpenEXR/IlmImf/ImfInputStreamMutex.h \
    Source/OpenEXR/IlmImf/ImfInt64.h \
    Source/OpenEXR/IlmImf/ImfIntAttribute.h \
    Source/OpenEXR/IlmImf/ImfIO.h \
    Source/OpenEXR/IlmImf/ImfKeyCode.h \
    Source/OpenEXR/IlmImf/ImfKeyCodeAttribute.h \
    Source/OpenEXR/IlmImf/ImfLineOrder.h \
    Source/OpenEXR/IlmImf/ImfLineOrderAttribute.h \
    Source/OpenEXR/IlmImf/ImfLut.h \
    Source/OpenEXR/IlmImf/ImfMatrixAttribute.h \
    Source/OpenEXR/IlmImf/ImfMisc.h \
    Source/OpenEXR/IlmImf/ImfMultiPartInputFile.h \
    Source/OpenEXR/IlmImf/ImfMultiPartOutputFile.h \
    Source/OpenEXR/IlmImf/ImfMultiView.h \
    Source/OpenEXR/IlmImf/ImfName.h \
    Source/OpenEXR/IlmImf/ImfNamespace.h \
    Source/OpenEXR/IlmImf/ImfOpaqueAttribute.h \
    Source/OpenEXR/IlmImf/ImfOptimizedPixelReading.h \
    Source/OpenEXR/IlmImf/ImfOutputFile.h \
    Source/OpenEXR/IlmImf/ImfOutputPart.h \
    Source/OpenEXR/IlmImf/ImfOutputPartData.h \
    Source/OpenEXR/IlmImf/ImfOutputStreamMutex.h \
    Source/OpenEXR/IlmImf/ImfPartHelper.h \
    Source/OpenEXR/IlmImf/ImfPartType.h \
    Source/OpenEXR/IlmImf/ImfPixelType.h \
    Source/OpenEXR/IlmImf/ImfPizCompressor.h \
    Source/OpenEXR/IlmImf/ImfPreviewImage.h \
    Source/OpenEXR/IlmImf/ImfPreviewImageAttribute.h \
    Source/OpenEXR/IlmImf/ImfPxr24Compressor.h \
    Source/OpenEXR/IlmImf/ImfRational.h \
    Source/OpenEXR/IlmImf/ImfRationalAttribute.h \
    Source/OpenEXR/IlmImf/ImfRgba.h \
    Source/OpenEXR/IlmImf/ImfRgbaFile.h \
    Source/OpenEXR/IlmImf/ImfRgbaYca.h \
    Source/OpenEXR/IlmImf/ImfRle.h \
    Source/OpenEXR/IlmImf/ImfRleCompressor.h \
    Source/OpenEXR/IlmImf/ImfScanLineInputFile.h \
    Source/OpenEXR/IlmImf/ImfSimd.h \
    Source/OpenEXR/IlmImf/ImfStandardAttributes.h \
    Source/OpenEXR/IlmImf/ImfStdIO.h \
    Source/OpenEXR/IlmImf/ImfStringAttribute.h \
    Source/OpenEXR/IlmImf/ImfStringVectorAttribute.h \
    Source/OpenEXR/IlmImf/ImfSystemSpecific.h \
    Source/OpenEXR/IlmImf/ImfTestFile.h \
    Source/OpenEXR/IlmImf/ImfThreading.h \
    Source/OpenEXR/IlmImf/ImfTileDescription.h \
    Source/OpenEXR/IlmImf/ImfTileDescriptionAttribute.h \
    Source/OpenEXR/IlmImf/ImfTiledInputFile.h \
    Source/OpenEXR/IlmImf/ImfTiledInputPart.h \
    Source/OpenEXR/IlmImf/ImfTiledMisc.h \
    Source/OpenEXR/IlmImf/ImfTiledOutputFile.h \
    Source/OpenEXR/IlmImf/ImfTiledOutputPart.h \
    Source/OpenEXR/IlmImf/ImfTiledRgbaFile.h \
    Source/OpenEXR/IlmImf/ImfTileOffsets.h \
    Source/OpenEXR/IlmImf/ImfTimeCode.h \
    Source/OpenEXR/IlmImf/ImfTimeCodeAttribute.h \
    Source/OpenEXR/IlmImf/ImfVecAttribute.h \
    Source/OpenEXR/IlmImf/ImfVersion.h \
    Source/OpenEXR/IlmImf/ImfWav.h \
    Source/OpenEXR/IlmImf/ImfXdr.h \
    Source/OpenEXR/IlmImf/ImfZip.h \
    Source/OpenEXR/IlmImf/ImfZipCompressor.h \
    Source/OpenEXR/IlmThread/IlmThread.h \
    Source/OpenEXR/IlmThread/IlmThreadExport.h \
    Source/OpenEXR/IlmThread/IlmThreadForward.h \
    Source/OpenEXR/IlmThread/IlmThreadMutex.h \
    Source/OpenEXR/IlmThread/IlmThreadNamespace.h \
    Source/OpenEXR/IlmThread/IlmThreadPool.h \
    Source/OpenEXR/IlmThread/IlmThreadSemaphore.h \
    Source/OpenEXR/Imath/ImathBox.h \
    Source/OpenEXR/Imath/ImathBoxAlgo.h \
    Source/OpenEXR/Imath/ImathColor.h \
    Source/OpenEXR/Imath/ImathColorAlgo.h \
    Source/OpenEXR/Imath/ImathEuler.h \
    Source/OpenEXR/Imath/ImathExc.h \
    Source/OpenEXR/Imath/ImathExport.h \
    Source/OpenEXR/Imath/ImathForward.h \
    Source/OpenEXR/Imath/ImathFrame.h \
    Source/OpenEXR/Imath/ImathFrustum.h \
    Source/OpenEXR/Imath/ImathFrustumTest.h \
    Source/OpenEXR/Imath/ImathFun.h \
    Source/OpenEXR/Imath/ImathGL.h \
    Source/OpenEXR/Imath/ImathGLU.h \
    Source/OpenEXR/Imath/ImathHalfLimits.h \
    Source/OpenEXR/Imath/ImathInt64.h \
    Source/OpenEXR/Imath/ImathInterval.h \
    Source/OpenEXR/Imath/ImathLimits.h \
    Source/OpenEXR/Imath/ImathLine.h \
    Source/OpenEXR/Imath/ImathLineAlgo.h \
    Source/OpenEXR/Imath/ImathMath.h \
    Source/OpenEXR/Imath/ImathMatrix.h \
    Source/OpenEXR/Imath/ImathMatrixAlgo.h \
    Source/OpenEXR/Imath/ImathNamespace.h \
    Source/OpenEXR/Imath/ImathPlane.h \
    Source/OpenEXR/Imath/ImathPlatform.h \
    Source/OpenEXR/Imath/ImathQuat.h \
    Source/OpenEXR/Imath/ImathRandom.h \
    Source/OpenEXR/Imath/ImathRoots.h \
    Source/OpenEXR/Imath/ImathShear.h \
    Source/OpenEXR/Imath/ImathSphere.h \
    Source/OpenEXR/Imath/ImathVec.h \
    Source/OpenEXR/Imath/ImathVecAlgo.h \
    Source/OpenEXR/IlmBaseConfig.h \
    Source/OpenEXR/OpenEXRConfig.h \
    Source/ZLib/crc32.h \
    Source/ZLib/deflate.h \
    Source/ZLib/gzguts.h \
    Source/ZLib/inffast.h \
    Source/ZLib/inffixed.h \
    Source/ZLib/inflate.h \
    Source/ZLib/inftrees.h \
    Source/ZLib/trees.h \
    Source/ZLib/zconf.h \
    Source/ZLib/zlib.h \
    Source/ZLib/zutil.h \
    Source/CacheFile.h \
    Source/FreeImage.h \
    Source/FreeImageIO.h \
    Source/MapIntrospector.h \
    Source/Plugin.h \
    Source/Quantizers.h \
    Source/ToneMapping.h \
    Source/Utilities.h

SOURCES += \
    Source/FreeImage/BitmapAccess.cpp \
    Source/FreeImage/ColorLookup.cpp \
    Source/FreeImage/FreeImage.cpp \
    Source/FreeImage/FreeImageC.c \
    Source/FreeImage/FreeImageIO.cpp \
    Source/FreeImage/GetType.cpp \
    Source/FreeImage/MemoryIO.cpp \
    Source/FreeImage/PixelAccess.cpp \
    Source/FreeImage/J2KHelper.cpp \
    Source/FreeImage/MNGHelper.cpp \
    Source/FreeImage/Plugin.cpp \
    Source/FreeImage/PluginBMP.cpp \
    Source/FreeImage/PluginCUT.cpp \
    Source/FreeImage/PluginDDS.cpp \
    Source/FreeImage/PluginEXR.cpp \
    Source/FreeImage/PluginG3.cpp \
    Source/FreeImage/PluginGIF.cpp \
    Source/FreeImage/PluginHDR.cpp \
    Source/FreeImage/PluginICO.cpp \
    Source/FreeImage/PluginIFF.cpp \
    Source/FreeImage/PluginJ2K.cpp \
    Source/FreeImage/PluginJNG.cpp \
    Source/FreeImage/PluginJP2.cpp \
    Source/FreeImage/PluginJPEG.cpp \
    Source/FreeImage/PluginJXR.cpp \
    Source/FreeImage/PluginKOALA.cpp \
    Source/FreeImage/PluginMNG.cpp \
    Source/FreeImage/PluginPCD.cpp \
    Source/FreeImage/PluginPCX.cpp \
    Source/FreeImage/PluginPFM.cpp \
    Source/FreeImage/PluginPICT.cpp \
    Source/FreeImage/PluginPNG.cpp \
    Source/FreeImage/PluginPNM.cpp \
    Source/FreeImage/PluginPSD.cpp \
    Source/FreeImage/PluginRAS.cpp \
    Source/FreeImage/PluginRAW.cpp \
    Source/FreeImage/PluginSGI.cpp \
    Source/FreeImage/PluginTARGA.cpp \
    Source/FreeImage/PluginTIFF.cpp \
    Source/FreeImage/PluginWBMP.cpp \
    Source/FreeImage/PluginWebP.cpp \
    Source/FreeImage/PluginXBM.cpp \
    Source/FreeImage/PluginXPM.cpp \
    Source/FreeImage/PSDParser.cpp \
    Source/FreeImage/TIFFLogLuv.cpp \
    Source/FreeImage/Conversion.cpp \
    Source/FreeImage/Conversion16_555.cpp \
    Source/FreeImage/Conversion16_565.cpp \
    Source/FreeImage/Conversion24.cpp \
    Source/FreeImage/Conversion32.cpp \
    Source/FreeImage/Conversion4.cpp \
    Source/FreeImage/Conversion8.cpp \
    Source/FreeImage/ConversionFloat.cpp \
    Source/FreeImage/ConversionRGB16.cpp \
    Source/FreeImage/ConversionRGBA16.cpp \
    Source/FreeImage/ConversionRGBAF.cpp \
    Source/FreeImage/ConversionRGBF.cpp \
    Source/FreeImage/ConversionType.cpp \
    Source/FreeImage/ConversionUINT16.cpp \
    Source/FreeImage/Halftoning.cpp \
    Source/FreeImage/tmoColorConvert.cpp \
    Source/FreeImage/tmoDrago03.cpp \
    Source/FreeImage/tmoFattal02.cpp \
    Source/FreeImage/tmoReinhard05.cpp \
    Source/FreeImage/ToneMapping.cpp \
    Source/FreeImage/LFPQuantizer.cpp \
    Source/FreeImage/NNQuantizer.cpp \
    Source/FreeImage/WuQuantizer.cpp \
    Source/DeprecationManager/Deprecated.cpp \
    Source/DeprecationManager/DeprecationMgr.cpp \
    Source/FreeImage/CacheFile.cpp \
    Source/FreeImage/MultiPage.cpp \
    Source/FreeImage/ZLibInterface.cpp \
    Source/Metadata/Exif.cpp \
    Source/Metadata/FIRational.cpp \
    Source/Metadata/FreeImageTag.cpp \
    Source/Metadata/IPTC.cpp \
    Source/Metadata/TagConversion.cpp \
    Source/Metadata/TagLib.cpp \
    Source/Metadata/XTIFF.cpp \
    Source/FreeImageToolkit/Background.cpp \
    Source/FreeImageToolkit/BSplineRotate.cpp \
    Source/FreeImageToolkit/Channels.cpp \
    Source/FreeImageToolkit/ClassicRotate.cpp \
    Source/FreeImageToolkit/Colors.cpp \
    Source/FreeImageToolkit/CopyPaste.cpp \
    Source/FreeImageToolkit/Display.cpp \
    Source/FreeImageToolkit/Flip.cpp \
    Source/FreeImageToolkit/JPEGTransform.cpp \
    Source/FreeImageToolkit/MultigridPoissonSolver.cpp \
    Source/FreeImageToolkit/Rescale.cpp \
    Source/FreeImageToolkit/Resize.cpp \
    Source/LibJPEG/jaricom.c \
    Source/LibJPEG/jcapimin.c \
    Source/LibJPEG/jcapistd.c \
    Source/LibJPEG/jcarith.c \
    Source/LibJPEG/jccoefct.c \
    Source/LibJPEG/jccolor.c \
    Source/LibJPEG/jcdctmgr.c \
    Source/LibJPEG/jchuff.c \
    Source/LibJPEG/jcinit.c \
    Source/LibJPEG/jcmainct.c \
    Source/LibJPEG/jcmarker.c \
    Source/LibJPEG/jcmaster.c \
    Source/LibJPEG/jcomapi.c \
    Source/LibJPEG/jcparam.c \
    Source/LibJPEG/jcprepct.c \
    Source/LibJPEG/jcsample.c \
    Source/LibJPEG/jctrans.c \
    Source/LibJPEG/jdapimin.c \
    Source/LibJPEG/jdapistd.c \
    Source/LibJPEG/jdarith.c \
    Source/LibJPEG/jdatadst.c \
    Source/LibJPEG/jdatasrc.c \
    Source/LibJPEG/jdcoefct.c \
    Source/LibJPEG/jdcolor.c \
    Source/LibJPEG/jddctmgr.c \
    Source/LibJPEG/jdhuff.c \
    Source/LibJPEG/jdinput.c \
    Source/LibJPEG/jdmainct.c \
    Source/LibJPEG/jdmarker.c \
    Source/LibJPEG/jdmaster.c \
    Source/LibJPEG/jdmerge.c \
    Source/LibJPEG/jdpostct.c \
    Source/LibJPEG/jdsample.c \
    Source/LibJPEG/jdtrans.c \
    Source/LibJPEG/jerror.c \
    Source/LibJPEG/jfdctflt.c \
    Source/LibJPEG/jfdctfst.c \
    Source/LibJPEG/jfdctint.c \
    Source/LibJPEG/jidctflt.c \
    Source/LibJPEG/jidctfst.c \
    Source/LibJPEG/jidctint.c \
    Source/LibJPEG/jmemmgr.c \
    Source/LibJPEG/jmemnobs.c \
    Source/LibJPEG/jquant1.c \
    Source/LibJPEG/jquant2.c \
    Source/LibJPEG/jutils.c \
    Source/LibJPEG/transupp.c \
    Source/LibPNG/png.c \
    Source/LibPNG/pngerror.c \
    Source/LibPNG/pngget.c \
    Source/LibPNG/pngmem.c \
    Source/LibPNG/pngpread.c \
    Source/LibPNG/pngread.c \
    Source/LibPNG/pngrio.c \
    Source/LibPNG/pngrtran.c \
    Source/LibPNG/pngrutil.c \
    Source/LibPNG/pngset.c \
    Source/LibPNG/pngtrans.c \
    Source/LibPNG/pngwio.c \
    Source/LibPNG/pngwrite.c \
    Source/LibPNG/pngwtran.c \
    Source/LibPNG/pngwutil.c \
    Source/LibTIFF4/tif_aux.c \
    Source/LibTIFF4/tif_close.c \
    Source/LibTIFF4/tif_codec.c \
    Source/LibTIFF4/tif_color.c \
    Source/LibTIFF4/tif_compress.c \
    Source/LibTIFF4/tif_dir.c \
    Source/LibTIFF4/tif_dirinfo.c \
    Source/LibTIFF4/tif_dirread.c \
    Source/LibTIFF4/tif_dirwrite.c \
    Source/LibTIFF4/tif_dumpmode.c \
    Source/LibTIFF4/tif_error.c \
    Source/LibTIFF4/tif_extension.c \
    Source/LibTIFF4/tif_fax3.c \
    Source/LibTIFF4/tif_fax3sm.c \
    Source/LibTIFF4/tif_flush.c \
    Source/LibTIFF4/tif_getimage.c \
    Source/LibTIFF4/tif_jpeg.c \
    Source/LibTIFF4/tif_luv.c \
    Source/LibTIFF4/tif_lzma.c \
    Source/LibTIFF4/tif_lzw.c \
    Source/LibTIFF4/tif_next.c \
    Source/LibTIFF4/tif_ojpeg.c \
    Source/LibTIFF4/tif_open.c \
    Source/LibTIFF4/tif_packbits.c \
    Source/LibTIFF4/tif_pixarlog.c \
    Source/LibTIFF4/tif_predict.c \
    Source/LibTIFF4/tif_print.c \
    Source/LibTIFF4/tif_read.c \
    Source/LibTIFF4/tif_strip.c \
    Source/LibTIFF4/tif_swab.c \
    Source/LibTIFF4/tif_thunder.c \
    Source/LibTIFF4/tif_tile.c \
    Source/LibTIFF4/tif_version.c \
    Source/LibTIFF4/tif_warning.c \
    Source/LibTIFF4/tif_write.c \
    Source/LibTIFF4/tif_zip.c \
    Source/ZLib/adler32.c \
    Source/ZLib/compress.c \
    Source/ZLib/crc32.c \
    Source/ZLib/deflate.c \
    Source/ZLib/gzclose.c \
    Source/ZLib/gzlib.c \
    Source/ZLib/gzread.c \
    Source/ZLib/gzwrite.c \
    Source/ZLib/infback.c \
    Source/ZLib/inffast.c \
    Source/ZLib/inflate.c \
    Source/ZLib/inftrees.c \
    Source/ZLib/trees.c \
    Source/ZLib/uncompr.c \
    Source/ZLib/zutil.c \
    Source/LibOpenJPEG/bio.c \
    Source/LibOpenJPEG/cio.c \
    Source/LibOpenJPEG/dwt.c \
    Source/LibOpenJPEG/event.c \
    Source/LibOpenJPEG/function_list.c \
    Source/LibOpenJPEG/oimage.c \
    Source/LibOpenJPEG/invert.c \
    Source/LibOpenJPEG/j2k.c \
    Source/LibOpenJPEG/jp2.c \
    Source/LibOpenJPEG/mct.c \
    Source/LibOpenJPEG/mqc.c \
    Source/LibOpenJPEG/openjpeg.c \
    Source/LibOpenJPEG/opj_clock.c \
    Source/LibOpenJPEG/pi.c \
    Source/LibOpenJPEG/raw.c \
    Source/LibOpenJPEG/t1.c \
    Source/LibOpenJPEG/t2.c \
    Source/LibOpenJPEG/tcd.c \
    Source/LibOpenJPEG/tgt.c \
    Source/OpenEXR/IlmImf/b44ExpLogTable.cpp \
    Source/OpenEXR/IlmImf/ImfAcesFile.cpp \
    Source/OpenEXR/IlmImf/ImfAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfB44Compressor.cpp \
    Source/OpenEXR/IlmImf/ImfBoxAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfChannelList.cpp \
    Source/OpenEXR/IlmImf/ImfChannelListAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfChromaticities.cpp \
    Source/OpenEXR/IlmImf/ImfChromaticitiesAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfCompositeDeepScanLine.cpp \
    Source/OpenEXR/IlmImf/ImfCompressionAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfCompressor.cpp \
    Source/OpenEXR/IlmImf/ImfConvert.cpp \
    Source/OpenEXR/IlmImf/ImfCRgbaFile.cpp \
    Source/OpenEXR/IlmImf/ImfDeepCompositing.cpp \
    Source/OpenEXR/IlmImf/ImfDeepFrameBuffer.cpp \
    Source/OpenEXR/IlmImf/ImfDeepImageStateAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfDeepScanLineInputFile.cpp \
    Source/OpenEXR/IlmImf/ImfDeepScanLineInputPart.cpp \
    Source/OpenEXR/IlmImf/ImfDeepScanLineOutputFile.cpp \
    Source/OpenEXR/IlmImf/ImfDeepScanLineOutputPart.cpp \
    Source/OpenEXR/IlmImf/ImfDeepTiledInputFile.cpp \
    Source/OpenEXR/IlmImf/ImfDeepTiledInputPart.cpp \
    Source/OpenEXR/IlmImf/ImfDeepTiledOutputFile.cpp \
    Source/OpenEXR/IlmImf/ImfDeepTiledOutputPart.cpp \
    Source/OpenEXR/IlmImf/ImfDoubleAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfDwaCompressor.cpp \
    Source/OpenEXR/IlmImf/ImfEnvmap.cpp \
    Source/OpenEXR/IlmImf/ImfEnvmapAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfFastHuf.cpp \
    Source/OpenEXR/IlmImf/ImfFloatAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfFloatVectorAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfFrameBuffer.cpp \
    Source/OpenEXR/IlmImf/ImfFramesPerSecond.cpp \
    Source/OpenEXR/IlmImf/ImfGenericInputFile.cpp \
    Source/OpenEXR/IlmImf/ImfGenericOutputFile.cpp \
    Source/OpenEXR/IlmImf/ImfHeader.cpp \
    Source/OpenEXR/IlmImf/ImfHuf.cpp \
    Source/OpenEXR/IlmImf/ImfInputFile.cpp \
    Source/OpenEXR/IlmImf/ImfInputPart.cpp \
    Source/OpenEXR/IlmImf/ImfInputPartData.cpp \
    Source/OpenEXR/IlmImf/ImfIntAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfIO.cpp \
    Source/OpenEXR/IlmImf/ImfKeyCode.cpp \
    Source/OpenEXR/IlmImf/ImfKeyCodeAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfLineOrderAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfLut.cpp \
    Source/OpenEXR/IlmImf/ImfMatrixAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfMisc.cpp \
    Source/OpenEXR/IlmImf/ImfMultiPartInputFile.cpp \
    Source/OpenEXR/IlmImf/ImfMultiPartOutputFile.cpp \
    Source/OpenEXR/IlmImf/ImfMultiView.cpp \
    Source/OpenEXR/IlmImf/ImfOpaqueAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfOutputFile.cpp \
    Source/OpenEXR/IlmImf/ImfOutputPart.cpp \
    Source/OpenEXR/IlmImf/ImfOutputPartData.cpp \
    Source/OpenEXR/IlmImf/ImfPartType.cpp \
    Source/OpenEXR/IlmImf/ImfPizCompressor.cpp \
    Source/OpenEXR/IlmImf/ImfPreviewImage.cpp \
    Source/OpenEXR/IlmImf/ImfPreviewImageAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfPxr24Compressor.cpp \
    Source/OpenEXR/IlmImf/ImfRational.cpp \
    Source/OpenEXR/IlmImf/ImfRationalAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfRgbaFile.cpp \
    Source/OpenEXR/IlmImf/ImfRgbaYca.cpp \
    Source/OpenEXR/IlmImf/ImfRle.cpp \
    Source/OpenEXR/IlmImf/ImfRleCompressor.cpp \
    Source/OpenEXR/IlmImf/ImfScanLineInputFile.cpp \
    Source/OpenEXR/IlmImf/ImfStandardAttributes.cpp \
    Source/OpenEXR/IlmImf/ImfStdIO.cpp \
    Source/OpenEXR/IlmImf/ImfStringAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfStringVectorAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfSystemSpecific.cpp \
    Source/OpenEXR/IlmImf/ImfTestFile.cpp \
    Source/OpenEXR/IlmImf/ImfThreading.cpp \
    Source/OpenEXR/IlmImf/ImfTileDescriptionAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfTiledInputFile.cpp \
    Source/OpenEXR/IlmImf/ImfTiledInputPart.cpp \
    Source/OpenEXR/IlmImf/ImfTiledMisc.cpp \
    Source/OpenEXR/IlmImf/ImfTiledOutputFile.cpp \
    Source/OpenEXR/IlmImf/ImfTiledOutputPart.cpp \
    Source/OpenEXR/IlmImf/ImfTiledRgbaFile.cpp \
    Source/OpenEXR/IlmImf/ImfTileOffsets.cpp \
    Source/OpenEXR/IlmImf/ImfTimeCode.cpp \
    Source/OpenEXR/IlmImf/ImfTimeCodeAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfVecAttribute.cpp \
    Source/OpenEXR/IlmImf/ImfVersion.cpp \
    Source/OpenEXR/IlmImf/ImfWav.cpp \
    Source/OpenEXR/IlmImf/ImfZip.cpp \
    Source/OpenEXR/IlmImf/ImfZipCompressor.cpp \
    Source/OpenEXR/Imath/ImathBox.cpp \
    Source/OpenEXR/Imath/ImathColorAlgo.cpp \
    Source/OpenEXR/Imath/ImathFun.cpp \
    Source/OpenEXR/Imath/ImathMatrixAlgo.cpp \
    Source/OpenEXR/Imath/ImathRandom.cpp \
    Source/OpenEXR/Imath/ImathShear.cpp \
    Source/OpenEXR/Imath/ImathVec.cpp \
    Source/OpenEXR/Iex/IexBaseExc.cpp \
    Source/OpenEXR/Iex/IexThrowErrnoExc.cpp \
    Source/OpenEXR/Half/half.cpp \
    Source/OpenEXR/IlmThread/IlmThread.cpp \
    Source/OpenEXR/IlmThread/IlmThreadMutex.cpp \
    Source/OpenEXR/IlmThread/IlmThreadPool.cpp \
    Source/OpenEXR/IlmThread/IlmThreadSemaphore.cpp \
    Source/OpenEXR/IexMath/IexMathFloatExc.cpp \
    Source/OpenEXR/IexMath/IexMathFpu.cpp \
    Source/LibRawLite/internal/dcraw_common.cpp \
    Source/LibRawLite/internal/dcraw_fileio.cpp \
    Source/LibRawLite/internal/demosaic_packs.cpp \
    Source/LibRawLite/src/libraw_c_api.cpp \
    Source/LibRawLite/src/libraw_cxx.cpp \
    Source/LibRawLite/src/libraw_datastream.cpp \
    Source/LibWebP/src/dec/dec.alpha.c \
    Source/LibWebP/src/dec/dec.buffer.c \
    Source/LibWebP/src/dec/dec.frame.c \
    Source/LibWebP/src/dec/dec.idec.c \
    Source/LibWebP/src/dec/dec.io.c \
    Source/LibWebP/src/dec/dec.quant.c \
    Source/LibWebP/src/dec/dec.tree.c \
    Source/LibWebP/src/dec/dec.vp8.c \
    Source/LibWebP/src/dec/dec.vp8l.c \
    Source/LibWebP/src/dec/dec.webp.c \
    Source/LibWebP/src/dsp/dsp.alpha_processing.c \
    Source/LibWebP/src/dsp/dsp.alpha_processing_mips_dsp_r2.c \
    Source/LibWebP/src/dsp/dsp.alpha_processing_sse2.c \
    Source/LibWebP/src/dsp/dsp.argb.c \
    Source/LibWebP/src/dsp/dsp.argb_mips_dsp_r2.c \
    Source/LibWebP/src/dsp/dsp.argb_sse2.c \
    Source/LibWebP/src/dsp/dsp.cost.c \
    Source/LibWebP/src/dsp/dsp.cost_mips32.c \
    Source/LibWebP/src/dsp/dsp.cost_mips_dsp_r2.c \
    Source/LibWebP/src/dsp/dsp.cost_sse2.c \
    Source/LibWebP/src/dsp/dsp.cpu.c \
    Source/LibWebP/src/dsp/dsp.dec.c \
    Source/LibWebP/src/dsp/dsp.dec_clip_tables.c \
    Source/LibWebP/src/dsp/dsp.dec_mips32.c \
    Source/LibWebP/src/dsp/dsp.dec_mips_dsp_r2.c \
    Source/LibWebP/src/dsp/dsp.dec_neon.c \
    Source/LibWebP/src/dsp/dsp.dec_sse2.c \
    Source/LibWebP/src/dsp/dsp.enc.c \
    Source/LibWebP/src/dsp/dsp.enc_avx2.c \
    Source/LibWebP/src/dsp/dsp.enc_mips32.c \
    Source/LibWebP/src/dsp/dsp.enc_mips_dsp_r2.c \
    Source/LibWebP/src/dsp/dsp.enc_neon.c \
    Source/LibWebP/src/dsp/dsp.enc_sse2.c \
    Source/LibWebP/src/dsp/dsp.filters.c \
    Source/LibWebP/src/dsp/dsp.filters_mips_dsp_r2.c \
    Source/LibWebP/src/dsp/dsp.filters_sse2.c \
    Source/LibWebP/src/dsp/dsp.lossless.c \
    Source/LibWebP/src/dsp/dsp.lossless_mips32.c \
    Source/LibWebP/src/dsp/dsp.lossless_mips_dsp_r2.c \
    Source/LibWebP/src/dsp/dsp.lossless_neon.c \
    Source/LibWebP/src/dsp/dsp.lossless_sse2.c \
    Source/LibWebP/src/dsp/dsp.rescaler.c \
    Source/LibWebP/src/dsp/dsp.rescaler_mips32.c \
    Source/LibWebP/src/dsp/dsp.rescaler_mips_dsp_r2.c \
    Source/LibWebP/src/dsp/dsp.upsampling.c \
    Source/LibWebP/src/dsp/dsp.upsampling_mips_dsp_r2.c \
    Source/LibWebP/src/dsp/dsp.upsampling_neon.c \
    Source/LibWebP/src/dsp/dsp.upsampling_sse2.c \
    Source/LibWebP/src/dsp/dsp.yuv.c \
    Source/LibWebP/src/dsp/dsp.yuv_mips32.c \
    Source/LibWebP/src/dsp/dsp.yuv_mips_dsp_r2.c \
    Source/LibWebP/src/dsp/dsp.yuv_sse2.c \
    Source/LibWebP/src/enc/enc.alpha.c \
    Source/LibWebP/src/enc/enc.analysis.c \
    Source/LibWebP/src/enc/enc.backward_references.c \
    Source/LibWebP/src/enc/enc.config.c \
    Source/LibWebP/src/enc/enc.cost.c \
    Source/LibWebP/src/enc/enc.filter.c \
    Source/LibWebP/src/enc/enc.frame.c \
    Source/LibWebP/src/enc/enc.histogram.c \
    Source/LibWebP/src/enc/enc.iterator.c \
    Source/LibWebP/src/enc/enc.near_lossless.c \
    Source/LibWebP/src/enc/enc.picture.c \
    Source/LibWebP/src/enc/enc.picture_csp.c \
    Source/LibWebP/src/enc/enc.picture_psnr.c \
    Source/LibWebP/src/enc/enc.picture_rescale.c \
    Source/LibWebP/src/enc/enc.picture_tools.c \
    Source/LibWebP/src/enc/enc.quant.c \
    Source/LibWebP/src/enc/enc.syntax.c \
    Source/LibWebP/src/enc/enc.token.c \
    Source/LibWebP/src/enc/enc.tree.c \
    Source/LibWebP/src/enc/enc.vp8l.c \
    Source/LibWebP/src/enc/enc.webpenc.c \
    Source/LibWebP/src/utils/utils.bit_reader.c \
    Source/LibWebP/src/utils/utils.bit_writer.c \
    Source/LibWebP/src/utils/utils.color_cache.c \
    Source/LibWebP/src/utils/utils.filters.c \
    Source/LibWebP/src/utils/utils.huffman.c \
    Source/LibWebP/src/utils/utils.huffman_encode.c \
    Source/LibWebP/src/utils/utils.quant_levels.c \
    Source/LibWebP/src/utils/utils.quant_levels_dec.c \
    Source/LibWebP/src/utils/utils.random.c \
    Source/LibWebP/src/utils/utils.rescaler.c \
    Source/LibWebP/src/utils/utils.thread.c \
    Source/LibWebP/src/utils/utils.utils.c \
    Source/LibWebP/src/mux/mux.anim_encode.c \
    Source/LibWebP/src/mux/mux.muxedit.c \
    Source/LibWebP/src/mux/mux.muxinternal.c \
    Source/LibWebP/src/mux/mux.muxread.c \
    Source/LibWebP/src/demux/demux.demux.c \
    Source/LibJXR/image/decode/decode.c \
    Source/LibJXR/image/decode/JXRTranscode.c \
    Source/LibJXR/image/decode/postprocess.c \
    Source/LibJXR/image/decode/segdec.c \
    Source/LibJXR/image/decode/strdec.c \
    Source/LibJXR/image/decode/strdec_x86.c \
    Source/LibJXR/image/decode/strInvTransform.c \
    Source/LibJXR/image/decode/strPredQuantDec.c \
    Source/LibJXR/image/encode/encode.c \
    Source/LibJXR/image/encode/segenc.c \
    Source/LibJXR/image/encode/strenc.c \
    Source/LibJXR/image/encode/strenc_x86.c \
    Source/LibJXR/image/encode/strFwdTransform.c \
    Source/LibJXR/image/encode/strPredQuantEnc.c \
    Source/LibJXR/image/sys/adapthuff.c \
    Source/LibJXR/image/sys/image.c \
    Source/LibJXR/image/sys/strcodec.c \
    Source/LibJXR/image/sys/strPredQuant.c \
    Source/LibJXR/image/sys/strTransform.c \
    Source/LibJXR/jxrgluelib/JXRGlue.c \
    Source/LibJXR/jxrgluelib/JXRGlueJxr.c \
    Source/LibJXR/jxrgluelib/JXRGluePFC.c \
    Source/LibJXR/jxrgluelib/JXRMeta.c



