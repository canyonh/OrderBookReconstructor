#pragma once
#include <cassert>
#include "itch.h"

namespace Orderbook {

class IDataSource
{
public:
    virtual ~IDataSource() {}
    virtual bool Read(char* buffer, std::ptrdiff_t& size_read) = 0;
};

class IFileDataSource : public IDataSource
{
public:
    IFileDataSource(const char* path_in) {}
    virtual ~IFileDataSource() {}

    virtual bool Read(char* buffer, std::ptrdiff_t& size_read) override 
    {
        assert(!"@writeme");
        return false;
    }
};

class ISocketDataSource : public IDataSource
{
public:
    ISocketDataSource() {};
    virtual ~ISocketDataSource() {}

    virtual bool Read(char* buffer, std::ptrdiff_t& size_read)
    {
        assert(!"@writeme");
        return false;
    }
};

class IBinStreamReader
{
public:
    IBinStreamReader(IDataSource* data_src_in)
    : data_src(data_src_in)
    {}
    virtual ~IBinStreamReader() {};

    // @todo error handling?
    Itch::MsgSizeType ReadMsgSize() const noexcept;

    Itch::Int2 ReadInt2() const noexcept;
    Itch::Int2 ReadInt4() const noexcept;
    Itch::Int2 ReadInt6() const noexcept;
    Itch::Int2 ReadInt8() const noexcept;
    Itch::Alpha ReadAlpha() const noexcept;
private:
    IDataSource* data_src = nullptr;
};

} // namespace Orderbook

