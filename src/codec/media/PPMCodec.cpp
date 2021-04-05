#pragma once
#include "Image.h"
#include "File.h"

class CPPMCodec
{
public:
    void Encode(const CImage& InImage, CArray<CByteType>& PPMData)
    {
        if(InImage.GetPixelFormat() == EPixelFormat::RGBA8)
        {
            sprintf((char*)PPMData.GetData(), "P3\n%u %u\n%u\n",
                InImage.GetImageWidth(), InImage.GetImageHeight(), 255
            );

            CByteQuadruple Pixel;
            for(CSizeType Y = 0; Y < InImage.GetImageHeight(); ++Y)
            {
                for(CSizeType X = 0; X < InImage.GetImageWidth(); ++X)
                {
                    InImage.GetPixelRGBA8({X, Y}, Pixel);
                    for(int i = 0; i < 3; ++i)
                    {
                        sprintf((char*)PPMData.GetData(), "%u ", (CSizeType)Pixel[i]);
                    }
                    sprintf((char*)PPMData.GetData(), "\n");
                }
            }
        }
    }

    void SaveImageToPPMFile(const CImage &InImage, const char* InFilename)
    {
        CArray<CByteType> Data = {};
        Encode(InImage, Data);
        CFile::SaveDataToFile(InFilename, Data.GetData(), Data.SizeInBytes());
    }
};