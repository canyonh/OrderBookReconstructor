#pragma once
#include <optional>

namespace OrderbookReconstructor
{

class BitStreamReader;

struct ItchMsg
{

};

class ItchMsgParser
{
public:
    ItchMsgParser(BitStreamReader* reader_in)
    : reader(reader_in)
    {}

    std::optional<ItchMsg> Read()
    {
        return {};
    }

private:
    BitStreamReader* reader = nullptr;
};

} // namespace OrderbookReconstructor
