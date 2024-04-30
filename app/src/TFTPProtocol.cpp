#include "TFTPProtocol.h"

#include "Utilities.hpp"

namespace tftp {

namespace protocol {

constexpr ReadRequestPacket::ReadRequestPacket(const std::string &filename, const Mode mode)
{
  m_filename = filename;
  m_mode     = mode;
}

constexpr WriteRequestPacket::WriteRequestPacket(const std::string &filename, const Mode mode)
    : ReadRequestPacket(filename, mode)
{
}

constexpr AcknowledgmentPacket::AcknowledgmentPacket(const uint16_t size)
{
  m_size = size;
}

constexpr DataPacket::DataPacket(const uint16_t size, const std::string &data)
    : AcknowledgmentPacket(size)
{
  m_data = data;
}

constexpr ErrorPacket::ErrorPacket(const Error error, const std::string &errorMsg)
{
  m_error    = error;
  m_errorMsg = errorMsg;
}

} // namespace protocol

} // namespace tftp
