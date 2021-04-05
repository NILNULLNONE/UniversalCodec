#pragma once

class CMediaCodec
{

};

class CPNGCodec : public CMediaCodec
{
public:
    void DecodeFile(const char* Filename);
    void EncodeToFile();
};

