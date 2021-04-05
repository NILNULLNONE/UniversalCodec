#pragma once
#include "TypeDef.h"
#include "Array.h"

// template<CByteType ChannelNum, typename SampleType>
// struct CPixel
// {
//     SampleType Samples[ChannelNum];
//     SampleType& GetRed() { return *Samples; }
//     SampleType& GetGreen() {return *(Samples+1);}
//     SampleType& GetBlue() { return *(Samples+2); }
//     SampleType& GetAlpha() { return *(Samples+3); }
// };

enum class EPixelFormat
{
    RGBA8
};

// template <EPixelFormat Format>
// struct CPixel
// {
// };

// template<>
// struct CPixel<EPixelFormat::RGBA8>
// {
//     CPixel(const CByteType InR, const CByteType InG, const CByteType InB, const CByteType InA)
//     {

//     }
// };

// using CPixelRGBA8 = CPixel<4u, CByteType>;
// using CPixelRGB8 = CPixel<3u, CByteType>;

class CImage
{
public:
    static CSizeType GetPixelSizeInByte(EPixelFormat InFormat)
    {
        switch (InFormat)
        {
        case EPixelFormat::RGBA8:
            return 4;
            break;
        default:
            break;
        }
        CException::Check(0);
        return 0;
    }

    static void FillRGBA8(CImage &InImg, const CByteQuadruple &InRGBA8)
    {
        for (CSizeType i = 0; i < InImg.Width; ++i)
        {
            for (CSizeType j = 0; j < InImg.Height; ++j)
            {
                InImg.SetPixelRGBA8({i, j}, InRGBA8);
            }
        }
    }
public:
    CImage() : CImage(0, 0) {}
    CImage(CSizeType InWidth, CSizeType InHeight) : CImage(InWidth, InHeight, EPixelFormat::RGBA8) {}
    CImage(CSizeType InWidth, CSizeType InHeight, EPixelFormat InFormat)
        : Format(InFormat), Width(InWidth), Height(InHeight)
    {
        CSizeType Bytes = GetPixelSizeInByte(Format) * GetPixelNum();
        if(Bytes > 0)
        {
            Data.Resize(Bytes);
            CMemory::Zero(Data.GetData(), Data.SizeInBytes());
        }
    }

    CSizeType GetPixelNum() const
    {
        return Width * Height;
    }

    EPixelFormat GetPixelFormat() const
    {
        return Format;
    }


    void SetPixelRGBA8(const CSizeTuple &InXY, const CByteQuadruple &InRGBA8)
    {
        CException::Check(IsValidPos(InXY) && Format == EPixelFormat::RGBA8);
        CSizeType Index = ToIndex(InXY);
        CMemory::Copy(Data.GetData() + Index, InRGBA8, sizeof(InRGBA8));
    }

    void GetPixelRGBA8(const CSizeTuple &InXY, CByteQuadruple &InRGBA8) const
    {
        CException::Check(IsValidPos(InXY) && Format == EPixelFormat::RGBA8);
        CSizeType Index = ToIndex(InXY);
        CMemory::Copy(InRGBA8, Data.GetData() + Index, sizeof(InRGBA8));
    }

    CSizeType GetImageWidth() const { return Width; }

    CSizeType GetImageHeight() const { return Height; }

    bool IsValidPos(const CSizeTuple &InXY) const
    {
        return Width > 0 && Height > 0 
            && CMath::InRange(InXY[0], 0u, Width-1)
            && CMath::InRange(InXY[1], 0u, Height-1);
    }

    void Resize(const CSizeTuple &Size)
    {
        Width = Size[0];
        Height = Size[1];
        Data.Resize(GetPixelSizeInByte(Format) * GetPixelNum());
    }

private:
    CSizeType ToIndex(const CSizeTuple &InXY) const
    {
        return InXY[1] * Width + InXY[0];
    }
private:
    EPixelFormat Format;
    CArray<CByteType> Data;
    CSizeType Width, Height;
};


class CImageFactory
{
    void CreateChessboardRGBA8(const CSizeTuple& ImgSize, const CSizeTuple& BlockSize, CImage& Output)
    {
        Output.Resize(ImgSize);
        for(CSizeType X = 0; X < Output.GetImageWidth(); ++X)
        {
            for(CSizeType Y = 0; Y < Output.GetImageHeight(); ++Y)
            {
                bool IsBlack = (((X / BlockSize[0]) + (Y/BlockSize[1])) & 1) != 0;
                if(IsBlack)
                {
                    Output.SetPixelRGBA8({X, Y}, {0, 0, 0, 255});
                }
                else
                {
                    Output.SetPixelRGBA8({X, Y}, {255, 255, 255, 255});
                }
            }
        }
    }
};