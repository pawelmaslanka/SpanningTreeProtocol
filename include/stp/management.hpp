#pragma once

// This project's headers
#include "lib.hpp"
#include "logger.hpp"
#include "mac.hpp"

namespace Stp {

enum class RequestId : u8 {
    AddPort,
    RemovePort,
    ProcessBpdu,
    CHangeLogMsgSeverityLevel
};

class OutInterface {
public:
    virtual Result FlushFdb(const u16 portNo) __noexcept = 0;
    virtual Result SetForwarding(const u16 portNo, const bool enable) __noexcept = 0;
    virtual Result SetLearning(const u16 portNo, const bool enable) __noexcept = 0;
    virtual Result SendOutBpdu(const u16 portNo, ByteStreamH data) __noexcept = 0;

protected:
    virtual ~OutInterface() = default;
};

using OutInterfaceH = Sptr<OutInterface>;
using LoggerH = Sptr<Logger>;

class System {
public:
    System(Mac bridgeAddr, LoggerH logger);
    Mac& GetBridgeAddr() noexcept;
    LoggerH& LogInstance() noexcept;
private:
    Mac _bridgeAddr;
    LoggerH _logger;
};

using SystemH = Sptr<System>;

class Management {
public:
    static Result AddPort(const u16 portNo, const u32 speed, const bool enabled);
    static Result RemovePort(const u16 portNo);
    static Result ProcessBpdu(ByteStreamH bpdu);
    static Result RunStp(SystemH system, OutInterfaceH outInterface);
};

class Command {
public:
    virtual ~Command() = default;
    RequestId Id() const noexcept;

protected:
    Command(const RequestId reqId);

private:
    RequestId _reqId;
};

class AddPortReq : public Command {
public:
    AddPortReq(const u16 portNo, const u32 speed, const bool enabled);
    u16 GetPortNo() const noexcept;
    u32 GetPortSpeed() const noexcept;
    bool GetPortEnabled() const noexcept;

private:
    u32 _speed;
    u16 _portNo;
    bool _enabled;
};

class RemovePortReq : public Command {
public:
    RemovePortReq(const u16 portNo);
    u16 GetPortNo() const noexcept;

private:
    u16 _portNo;
};

class ProcessBpduReq : public Command {
public:
    ProcessBpduReq(ByteStreamH bpdu);
};

class RunStp : public Command {
public:
    RunStp(SystemH system, OutInterfaceH outInterface);
private:
    SystemH _system;
    OutInterfaceH _outInterface;
};

class ChangeLogMsgSeverityReq : public Command {
public:
    ChangeLogMsgSeverityReq(Logger::MsgSeverity msgSeverity);
};

inline System::System(Mac bridgeAddr, LoggerH logger)
    : _bridgeAddr{ bridgeAddr }, _logger{ logger } {
}

inline Mac& System::GetBridgeAddr() noexcept {
    return _bridgeAddr;
}

inline LoggerH& System::LogInstance() noexcept {
    return _logger;
}

inline Command::Command(const RequestId reqId)
    : _reqId{ reqId } {
    // Nothing more to do
}

inline RequestId Command::Id() const noexcept {
    return _reqId;
}

inline u16 AddPortReq::GetPortNo() const noexcept {
    return _portNo;
}

inline u32 AddPortReq::GetPortSpeed() const noexcept {
    return _speed;
}

inline bool AddPortReq::GetPortEnabled() const noexcept {
    return _enabled;
}

inline u16 RemovePortReq::GetPortNo() const noexcept {
    return _portNo;
}

} // namespace Stp
