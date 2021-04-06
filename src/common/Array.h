#pragma once
#include <initializer_list>
#include "Exception.h"
#include "Memory.h"
#include "TypeDef.h"
#include "ConstantDef.h"
#include "Math.h"
#include "common/Log.h"
template <typename ElementType>
class CArray
{
public:
    CArray()
    {

    }

    CArray(CSizeType Count, const ElementType& Element)
    {
        CException::Check(Count > 0);
        Reserve(Count);
        while(Count--)Add(Element);
    }

    CArray(const std::initializer_list<ElementType>& Initializer)
    {
        AddElements(Initializer);
    }

    CArray(const ElementType* InData, CSizeType ElementCount)
    {
        Resize(ElementCount);
        CMemory::Copy(DataPtr, InData, ElementCount * sizeof(ElementType));
    }

    CArray(const CArray<ElementType>& Other)
    {
        Resize(Other.Count());
        CMemory::Copy(GetData(), Other.GetData(), Other.Count());
    }

    CArray(CArray<ElementType>&& Other)
    {
        CMemory::Copy(this, &Other, sizeof(CArray<ElementType>));
        CMemory::Zero(&Other, sizeof(CArray<ElementType>));      
    }

    ~CArray()
    {
        CMemory::SafeFree(DataPtr);
    }

    void Resize(CSizeType Size)
    {
        while(Size > Count())
            Add(ElementType());

        if(Size < Count())
        {
            Reserve(Size);
            ArraySize = Size;
        }
    }

    CSizeType Count() const
    {
        return ArraySize;
    }

    ElementType &At(CSizeType Index)
    {
        return const_cast<ElementType&>((const_cast<const CArray*>(this)->At(Index)));
    }

    const ElementType &At(CSizeType Index) const
    {
        CException::Check(IsValidIndex(Index));
        return DataPtr[Index];
    }

    void Add(const ElementType& Element)
    {
        if(ArraySize + 1> DataElementCount)
        {
            Reserve( (DataElementCount+1) << 1);
        }
        ArraySize++;
        DataPtr[ArraySize - 1] = Element;
    }

    void AddElements(const std::initializer_list<ElementType>& Initializer)
    {
        Reserve(Count() + Initializer.size());
        auto Beg = Initializer.begin();
        while(Beg != Initializer.end())
        {
            Add(*Beg);
            Beg++;
        }
    }

    void Concat(const CArray<ElementType>& Other)
    {
        Concat(Other.GetData(), Other.Count());
    }

    void Concat(const ElementType* Other, const CSizeType OtherLen)
    {
        if(OtherLen <= 0)return;
        Resize(Count() + OtherLen);
        CMemory::Copy(EndPtr() - OtherLen, Other, OtherLen);
    }

    void RemoveAt(CSizeType Index)
    {
        CException::Check(IsValidIndex(Index));
        for (int i = Index; i < ArraySize - 1; ++i)
        {
            DataPtr[i] = DataPtr[i+1];
        }
        ArraySize--;
        // if(ArraySize < (DataElementCount >> 2))Shrink();
    }

    void RemoveLast()
    {
        RemoveAt(Count() - 1);
    }

    CSizeType IndexOfElement(const ElementType &Element)
    {
        for (CSizeType i = 0; i < ArraySize; ++i)
        {
            if(Element == DataPtr[i])return i;
        }
        return -1;
    }

    void RemoveElement(const ElementType& Element)
    {
        RemoveAt(IndexOfElement(Element));
    }

    void Reserve(CSizeType InElementCount)
    {
        if(InElementCount <= 0)return;
        if(!DataPtr)
        {
            DataElementCount = InElementCount;
            DataPtr = CMemory::Malloc<ElementType>(InElementCount);
            CException::Check(DataPtr != nullptr);
        }
        else if(DataElementCount < InElementCount)
        {
             DataElementCount = InElementCount;
             DataPtr = CMemory::Realloc<ElementType>(DataPtr, DataElementCount);
             CException::Check(DataPtr != nullptr);
        }
    }

    void Shrink()
    {
        if(!DataPtr)return;
        auto OldElementCount = DataElementCount;
        DataElementCount = CMath::Max(static_cast<CSizeType>(1), ArraySize);
        if(OldElementCount == DataElementCount)return;
        DataPtr = CMemory::Realloc<ElementType>(DataPtr, DataElementCount);
        CException::Check(DataPtr != nullptr);
    }

    bool IsValidIndex(CSizeType Index) const
    {
        return Index >= 0 && Index < ArraySize;
    }

    CSizeType ReserveCount()
    {
        return DataElementCount;
    }

    CSizeType ReserveSizeInBytes()
    {
        return DataElementCount * sizeof(ElementType);
    }

    CSizeType SizeInBytes() const 
    {
        return ArraySize * sizeof(ElementType);
    }

    const ElementType &operator[](CSizeType Index) const
    {
        return At(Index);
    }

    ElementType &operator[](CSizeType Index)
    {
        return At(Index);
    }

    ElementType* GetData()
    {
        return DataPtr;
    }

    const ElementType* GetData() const
    {
        return DataPtr;
    }

    bool IsEmpty() const {return ArraySize == 0;}

    const ElementType& First() const 
    {
        CException::Check(!IsEmpty());
        return DataPtr[0];
    }

    const ElementType& Second() const
    {
        CException::Check(Count() >= 2);
        return DataPtr[1];
    }

    const ElementType& Last() const 
    {
        CException::Check(!IsEmpty());
        return DataPtr[ArraySize-1];
    }

    ElementType& First()
    {
        return const_cast<ElementType &>((const_cast<const CArray<ElementType> *>(this))->First());
    }

    ElementType& Second()
    {
        return const_cast<ElementType &>((const_cast<const CArray<ElementType> *>(this))->Second());
    }

    ElementType& Last()
    {
        return const_cast<ElementType &>((const_cast<const CArray<ElementType> *>(this))->Last());
    }

    CArray<ElementType>& operator=(const CArray<ElementType>& Other)
    {
        Resize(Other.Count());
        CMemory::CopyObjects(GetData(), Other.GetData(), Count());
        return *this;
    }

    CArray<ElementType>& operator=(CArray<ElementType>&& Other)
    {
        CMemory::Copy(this, &Other, sizeof(CArray<ElementType>));
        CMemory::Zero(&Other, sizeof(CArray<ElementType>));
    }

    template <typename HandlerType>
    void ForEach(const HandlerType& InHandler) const
    {
        for(int i = 0; i < Count(); ++i)
        {
            InHandler(i, DataPtr[i]);
        }
    }

    bool IsLastIndex(CSizeType InIndex) const
    {
        return InIndex == Count() - 1;
    }

    virtual CSizeType GetSerializeSizeInByte() const
    {
        return sizeof(Count()) + SizeInBytes();
    }

    virtual CSizeType Serialize(CByteType *Dst) const
    {
        auto Step = CMemory::Write<CSizeType>(Dst, Count());
        if(Count() > 0)
        {
            Step += CMemory::Copy(Dst + Step, DataPtr, SizeInBytes());
        }        
        return Step;
    }

    virtual CSizeType Deserialize(const CByteType *Src)
    {
        auto Step = CMemory::Read<CSizeType>(Src, ArraySize);
        Reserve(ArraySize);
        Step += CMemory::Copy(GetData(), Src + Step, SizeInBytes());
        return Step;
    }

    void DiscardTo(ElementType*& OutData, CSizeType& OutLen)
    {
        OutData = DataPtr;
        OutLen = ArraySize;
        CMemory::Zero(this, sizeof(*this));
    }

    const ElementType* BeginPtr() const {return DataPtr;}
    ElementType* BeginPtr(){return DataPtr;}
    const ElementType* EndPtr() const {return DataPtr + ArraySize;}
    ElementType* EndPtr() {return DataPtr + ArraySize;}

public:
    struct Iterator
    {
        Iterator(ElementType* InPtr) : Ptr(InPtr)
        {

        }

        Iterator& operator=(const Iterator& Iter)
        {
            Ptr = Iter.Ptr;
            return *this;
        }

        bool operator!=(const Iterator& Iter) const
        {
            return Ptr != Iter.Ptr;
        }

        Iterator& operator++()
        {
            Ptr++;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator Ret = *this;
            (*this)++;
            return Ret;
        }

        Iterator& operator--()
        {
            Ptr--;
            return *this;
        }

        Iterator operator--(int)
        {
            Iterator Ret = *this;
            (*this)--;
            return Ret;
        }

        ElementType& operator*()
        {
            return *Ptr;
        }

        template<typename Type>
        Iterator operator+(Type Value)
        {
            return Iterator(Ptr + Value);
        }

        template<typename Type>
        Iterator operator-(Type Value)
        {
            return Iterator(Ptr - Value);
        }

        template<typename Type>
        Iterator& operator+=(Type Value)
        {
            Ptr += Value;
            return *this;
        }

        template<typename Type>
        Iterator& operator-=(Type Value)
        {
            Ptr -= Value;
            return *this;
        }
    private:
        ElementType* Ptr;
    };
    Iterator begin() {return Iterator(DataPtr);}
    Iterator end() {return Iterator(DataPtr + ArraySize);}
private:
    ElementType* DataPtr = nullptr;
    CSizeType ArraySize = 0;
    CSizeType DataElementCount = 0;
};

class CBitSequence : public CArray<CBitType>
{
public:
    void AddBit(CBitType Bit) { Add(Bit); }
    void PopBit(){RemoveLast();}

    CSizeType GetNeededBytes() const
    {
        return CMath::DivideAndRoundUp(Count(), CConstant::BitsPerByte);
    }

    virtual CSizeType GetSerializeSizeInByte() const override
    {
        return sizeof(Count()) + GetNeededBytes();
    }

    virtual CSizeType Serialize(CByteType *Dst) const override
    {
        auto Step = CMemory::Write<CSizeType>(Dst, Count());
        auto BitCount = Count();
        if (BitCount > 0)
        {
            for (int i = 0; i < BitCount; i += 8)
            {
                CByteType Byte = 0;
                for (int j = 0; j < 8 && i + j < BitCount; ++j)
                {
                    Byte |= (At(i+j)<<j);
                }
                Step += CMemory::Write<CByteType>(Dst+Step, Byte);
            }
        }
        return Step;
    }

    virtual CSizeType Deserialize(const CByteType *Src) override
    {
        CSizeType BitCount = 0;
        auto Step = CMemory::Read<CSizeType>(Src, BitCount);
        Reserve(BitCount);
        auto NeededBytes = CMath::DivideAndRoundUp(BitCount, CConstant::BitsPerByte);
        if (NeededBytes > 0)
        {
            for (int i = 0; i < NeededBytes; ++i)
            {
                CByteType Byte = Src[Step + i];
                for(int j = 0; j < 8 && i * 8 + j < BitCount; ++j)
                {
                    auto Shift = (1 << j);
                    CBitType Bit = (Byte & Shift) != 0;
                    AddBit( Bit );
                }
            }
        }
        return Step + NeededBytes;
    }
};

class CBitStream 
{
private:
    CArray<CByteType> Data;
    CSizeType Pos = 0;
    CSizeType BitCount = 0;
public:
    CBitStream()
    {
        Data.Add(0);
    }

    void PutBits(const CBitSequence& InSequence)
    {
        for(int i = 0; i < InSequence.Count(); ++i)
        {
            auto BitIndex = BitCount % 8;
            auto& LastByte = Data.Last();
            LastByte |= (InSequence[i] << BitIndex);
            if(BitIndex == 7)
            {
                Data.Add(0);
            }
            BitCount++;
        }
    }

    CBitType NextBit()
    {
        auto ElementIndex = Pos / 8;
        auto BitIndex = Pos % 8;
        Pos++;
        return (Data[ElementIndex] & (1 << BitIndex)) != 0;
    }

    CSizeType GetBitCount(){return BitCount;}

    CSizeType Serialize(CByteType *Dst) const 
    {
        auto Step = CMemory::Write<CSizeType>(Dst, BitCount);
        if (BitCount > 0)
        {
            Step += CMemory::Copy(Dst + Step, Data.GetData(), Data.SizeInBytes());
        }
        return Step;
    }

    CSizeType Deserialize(const CByteType *Src)
    {
        auto Step =CMemory::Read<CSizeType>(Src, BitCount);
        if(BitCount > 0)
        {
            Data.Resize(CMath::DivideAndRoundUp(BitCount, CConstant::BitsPerByte));
            Step += CMemory::Copy(Data.GetData(), Src + Step, Data.SizeInBytes());
        }
        return Step;
    }

    CSizeType GetSerializeSizeInByte() const
    {
        return sizeof(CSizeType) + Data.SizeInBytes();
    }
};
