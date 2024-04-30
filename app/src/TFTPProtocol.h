#ifndef TFTPPROTOCOL_H
#define TFTPPROTOCOL_H

#include <cstdint>
#include <string>
#include <type_traits>

namespace tftp {

namespace protocol {

static constexpr std::size_t PacketMaxSize  = 1024; // bytes
static constexpr std::size_t PacketDataSize = 512;  // bytes

enum class OperationType : uint16_t
{
  Undefined      = 0,
  Read           = 1,
  Write          = 2,
  Data           = 3,
  Acknowledgment = 4,
  Error          = 5,
  EnumCount
};

enum class Mode : uint8_t
{
  Undefined,
  Netascii,
  Octet,
  Mail,
  EnumCount
};

enum class Error : uint16_t
{
  Undefined,
  FileNotFound,
  AccessViolation,
  DiskFull,
  IllegalOperation,
  UnknownTransferID,
  FileAlreadyExist,
  NoSuchUser,
  UnsupportedOption,
  EnumCount
};

/* Abstact base class of tftp packet
 *
 * Need for implementation factory method of dynamic creating packets in runtime
 */
class Packet
{
protected:
  constexpr Packet() = default;

  static constexpr OperationType m_operationType = OperationType::Undefined;
};

class ReadRequestPacket : public Packet
{
public:
  constexpr ReadRequestPacket(const std::string& filename, const Mode mode);

protected:
  static constexpr OperationType m_operationType = OperationType::Read;

  std::string m_filename;
  Mode m_mode = Mode::Undefined;
};

class WriteRequestPacket : public ReadRequestPacket
{
public:
  constexpr WriteRequestPacket(const std::string& filename, const Mode mode);

protected:
  static constexpr OperationType m_operationType = OperationType::Write;
};

class AcknowledgmentPacket : public Packet
{
public:
  constexpr AcknowledgmentPacket(const uint16_t size);

protected:
  static constexpr OperationType m_operationType = OperationType::Acknowledgment;

  uint16_t m_size;
};

class DataPacket : public AcknowledgmentPacket
{
public:
  constexpr DataPacket(const uint16_t size, const std::string& data);

protected:
  static constexpr OperationType m_operationType = OperationType::Data;

  std::string m_data;
};

class ErrorPacket : public Packet
{
public:
  constexpr ErrorPacket(const Error error, const std::string& errorMsg);

private:
  static constexpr OperationType m_operationType = OperationType::Error;

  Error m_error = Error::Undefined;
  std::string m_errorMsg;
};

namespace generator {

template <class Request, typename... Args>
auto create_request(Args... args) -> std::enable_if_t<std::is_base_of_v<Packet, Request>>
{
  static_assert(std::is_same_v<ReadRequestPacket, Request> ||
                  std::is_same_v<WriteRequestPacket, Request> ||
                  std::is_same_v<AcknowledgmentPacket, Request>,
                "create_request supports only RRQ, WRQ and ACK packets");
}

template <class Response, typename... Args>
auto create_response(Args... args) -> std::enable_if_t<std::is_base_of_v<Packet, Response>>
{
  static_assert(std::is_same_v<DataPacket, Response> ||
                std::is_same_v<ErrorPacket, Response> ||
                "create_response supports only DATA and ERROR packets");
}

} // namespace generator

} // namespace protocol

} // namespace tftp

#endif // TFTPPROTOCOL_H
