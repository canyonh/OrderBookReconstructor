#pragma once
#include <cassert>
#include <cstdint>

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

} // namespace Orderbook

