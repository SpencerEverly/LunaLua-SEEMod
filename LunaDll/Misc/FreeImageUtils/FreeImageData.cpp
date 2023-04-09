#include <Windows.h>
#include "FreeImageData.h"
#include "FreeImageHelper.h"
#include "../../GlobalFuncs.h"

FreeImageData::FreeImageData() : m_bitmap(NULL)
{}


FreeImageData::~FreeImageData()
{
    reset();
}

bool FreeImageData::loadMem(unsigned char* data, unsigned long size, const std::string& filename)
{
    FIMEMORY * mem = FreeImage_OpenMemory(data, size);
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(mem, size);
    if (fif == FIF_UNKNOWN){
        fif = FreeImage_GetFIFFromFilename(filename.c_str());
    }
    if(fif == FIF_UNKNOWN)
        return false;
    m_bitmap = FreeImage_LoadFromMemory(fif, mem);
    FreeImage_CloseMemory(mem);
    return m_bitmap != NULL;
}

bool FreeImageData::loadFile(const std::string& filename)
{
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename.c_str());
    if (fif == FIF_UNKNOWN){
        fif = FreeImage_GetFIFFromFilename(filename.c_str());
    }
    if (fif == FIF_UNKNOWN)
        return false;
    
    m_bitmap = FreeImage_Load(fif, filename.c_str());
    return m_bitmap != NULL;
}

bool FreeImageData::loadFile(const std::wstring &filename)
{
    void* theFile;
    void* theMap;
    void* theAddress;
    theFile = CreateFileW(filename.c_str(), GENERIC_READ, 1, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (theFile == INVALID_HANDLE_VALUE)
        return false;

    DWORD size = GetFileSize(theFile, NULL);

    theMap = CreateFileMappingW(theFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if( theMap == NULL )
    {
        CloseHandle(theFile);
        return false;
    }
    theAddress = MapViewOfFile(theMap, FILE_MAP_READ, 0, 0, size);
    if(theAddress == NULL)
    {
        CloseHandle(theMap);
        CloseHandle(theFile);
        return false;
    }

    bool reply = loadMem((unsigned char*)theAddress, size, WStr2Str(filename));

    try{ UnmapViewOfFile(theAddress); } catch(void * /*e*/) {}
    try{ CloseHandle(theMap); } catch(void * /*e*/) {}
    try{ CloseHandle(theFile);} catch(void* /*e*/) {}

    return reply;
}

bool FreeImageData::saveFile(const std::string& filename) const
{
    if (!m_bitmap)
        return false;

    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filename.c_str());
    if (fif == FIF_UNKNOWN)
        return false;
    if(!FreeImage_Save(fif, m_bitmap, filename.c_str()))
        return false;
    
    return GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES;
}

bool FreeImageData::isValid() const
{
    return m_bitmap != NULL;
}

void FreeImageData::init(int width, int height, BYTE* pData, ColorMode color /*= ColorMode::COLORMODE_BGR*/, int bpp /*= -1*/)
{
    if (m_bitmap)
        reset();

    unsigned int redMask = 0;
    unsigned int greenMask = 0;
    unsigned int blueMask = 0;

    if (bpp == -1) {
        switch (color)
        {
        case FreeImageData::ColorMode::COLORMODE_BGR:
            bpp = 24;
            break;
        case FreeImageData::ColorMode::COLORMODE_RGB:
            bpp = 24;
            break;
        default:
            break;
        }
    }

    switch (color)
    {
    case FreeImageData::ColorMode::COLORMODE_BGR:
        redMask = 0x0000FF;
        greenMask = 0x00FF00;
        blueMask = 0xFF0000;
        break;
    case FreeImageData::ColorMode::COLORMODE_RGB:
        redMask = 0xFF0000;
        greenMask = 0x00FF00;
        blueMask = 0x0000FF;
        break;
    default:
        break;
    }

    m_bitmap = FreeImage_Allocate(width, height, bpp, redMask, greenMask, blueMask);
    if (!m_bitmap)
        return;

    BYTE* bitmapData = FreeImage_GetBits(m_bitmap);
    memcpy(bitmapData, pData, width * height * (bpp / 8));
}

void FreeImageData::reset()
{
    if (m_bitmap)
        FreeImage_Unload(m_bitmap);
}

HBITMAP FreeImageData::toHBITMAP()
{
    if (m_bitmap)
        return FreeImageHelper::FromFreeImage(m_bitmap);
    return nullptr;
}

uint32_t FreeImageData::getWidth()
{
    if (m_bitmap)
        return FreeImage_GetWidth(m_bitmap);
    return 0;
}

uint32_t FreeImageData::getHeight()
{
    if (m_bitmap)
        return FreeImage_GetHeight(m_bitmap);
    return 0;
}

bool FreeImageData::toRawBGRA(void* out)
{
    if (m_bitmap)
        return FreeImageHelper::ToRawBGRA(m_bitmap, out);
    return false;
}