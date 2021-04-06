#pragma once
#include <cstdio>
#include "Memory.h"
#include "String.h"
#include "TypeDef.h"

class CFilePath
{
public:
    static CString Join(const CString& InPrefix, const CString& InSuffix)
    {
        
    }
};

class CFile
{
private:
    
public:
    static CSizeType64 GetFileSize(const char* InFilename)
    {
        CSizeType64 Size = 0;
        FILE* File = fopen(InFilename, "rb");
        if(!File)return Size;
        fseek(File, 0, SEEK_END);
        Size = ftell(File);
        fclose(File);
        return Size;
    }

    static bool LoadDataFromFile(const char *InFilename, CByteType *&OutData, CSizeType &OutLen)
    {
        OutLen = static_cast<CSizeType>(GetFileSize(InFilename));
        if(OutLen == 0)return false;
        OutData = CMemory::Malloc<CByteType>(OutLen);
        FILE* File = fopen(InFilename, "rb");
        if(!File)return false;
        fread(OutData, sizeof(CByteType), OutLen, File);
        fclose(File);
        return true;
    }

    static bool SaveDataToFile(const char* InFilename, const CByteType* InData, const CSizeType InLen)
    {
        FILE* File = fopen(InFilename, "wb");
        if(!File)return false;
        fwrite(InData, sizeof(CByteType), InLen, File);
        fclose(File);
        return true;
    }
};