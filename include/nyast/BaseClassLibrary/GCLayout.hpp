#ifndef NYAST_BASE_CLASS_LIBRARY_GC_LAYOUT_HPP
#define NYAST_BASE_CLASS_LIBRARY_GC_LAYOUT_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT GCLayout : SubclassWithVariableDataOfType<Object, GCLayout, uintptr_t>
{
    static constexpr char const __className__[] = "GCLayout";

    static SlotDefinitions __slots__();
    
    static constexpr size_t BytesPerWord = sizeof(uintptr_t);
    static constexpr size_t BitsPerWord = BytesPerWord*8;
    static constexpr size_t LayoutsPerWord = BitsPerWord / GCReferenceTypeBitCount;

    static GCLayout* forInstanceSize(size_t instanceSize, GCReferenceType variableDataLayout);

    std::string asString() const;
    std::string printString() const;

    void setGCReferenceTypeAtWordIndex(size_t wordIndex, GCReferenceType value)
    {
        if(wordIndex*BytesPerWord > instanceSize)
            return;

        auto fixedLayoutData = variableData();
        auto layoutDataBitPosition = wordIndex * GCReferenceTypeBitCount;
        auto layoutDataWordPosition = layoutDataBitPosition / BitsPerWord;
        auto layoutDataWordBitPosition = layoutDataBitPosition % BitsPerWord;

        auto &word = fixedLayoutData[layoutDataWordPosition];
        word &= ~(GCReferenceTypeBitMask << layoutDataWordBitPosition);
        word |= uintptr_t(value) << layoutDataWordBitPosition;
    }

    void setGCReferenceTypeAtOffsetSize(size_t offset, size_t valueSize, GCReferenceType value)
    {
        if(valueSize < BytesPerWord || offset % BytesPerWord != 0)
            return;

        setGCReferenceTypeAtWordIndex(offset / BytesPerWord, value);
    }

    GCReferenceType getGCReferenceTypeAtWordIndex(size_t wordIndex) const
    {
        if(wordIndex*BytesPerWord > instanceSize)
            return GCReferenceType::Value;

        auto fixedLayoutData = variableData();
        auto layoutDataBitPosition = wordIndex * GCReferenceTypeBitCount;
        auto layoutDataWordPosition = layoutDataBitPosition / BitsPerWord;
        auto layoutDataWordBitPosition = layoutDataBitPosition % BitsPerWord;

        return GCReferenceType((fixedLayoutData[layoutDataWordPosition] >> layoutDataWordBitPosition) & GCReferenceTypeBitMask);
    }

    GCReferenceType getGCReferenceTypeAtOffset(size_t offset)
    {
        if(offset % BytesPerWord != 0)
            return GCReferenceType::Value;

        return getGCReferenceTypeAtWordIndex(offset / BytesPerWord);
    }

    template<typename FT>
    void fixedLayoutDo(const FT &f) const
    {
        size_t wordCount = instanceSize / BytesPerWord;
        for(size_t i = 0; i < wordCount; ++i)
        {
            f(getGCReferenceTypeAtWordIndex(i));
        }
    }

    size_t instanceSize;
    GCReferenceType variableDataLayout;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_GC_LAYOUT_HPP
