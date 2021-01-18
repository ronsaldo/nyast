#ifndef NYAST_SMALLTAK_COMPILER_SOURCE_POSITION_HPP
#define NYAST_SMALLTAK_COMPILER_SOURCE_POSITION_HPP
#pragma once

#include "SourceCollection.hpp"
#include "CollectionRange.hpp"
#include <ostream>
#include <algorithm>

namespace nyast
{
namespace SmalltalkCompiler
{


/**
 * I represent the string data from a source collection.
 */
struct SourcePosition : CollectionRange<SourcePosition, SourceCollectionPtr, std::string, int>
{
    typedef int peek_type;

    int eofValue() const
    {
        return -1;
    }

    std::string text() const
    {
        return content();
    }

    const std::string &sourceName() const
    {
        return collection->name;
    }

    SourceLineColumn startLineColumn() const
    {
        return collection->lineAndColumnForIndex(startPosition);
    }

    SourceLineColumn endLineColumn() const
    {
        return collection->lineAndColumnForIndex(startPosition);
    }

    friend std::ostream &operator<<(std::ostream &out, SourcePosition &position)
    {
        auto lineColumn = position.startLineColumn();
        return out << position.sourceName() << ':' << lineColumn.line << ':' << lineColumn.column;
    }
};

} // End of namespace SmalltalkCompiler
} // End of namespace nyast

#endif //NYAST_SMALLTAK_COMPILER_SOURCE_POSITION_HPP
