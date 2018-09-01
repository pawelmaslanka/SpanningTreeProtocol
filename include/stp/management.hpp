#pragma once

// This project's headers
#include "lib.hpp"
#include "logger.hpp"
#include "mac.hpp"
#include "system.hpp"

namespace Stp {

enum class RequestId : u8 {
    AddPort,
    RemovePort,
    ProcessBpdu,
    ChangeLogMsgSeverityLevel
};

class Management {
public:
    static Result AddPort(const u16 portNo, const u32 speed, const bool enabled);
    static Result RemovePort(const u16 portNo);
    static Result ProcessBpdu(ByteStreamH bpdu);
    static Result RunStp(Mac bridgeAddr, SystemH system);
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
    ByteStream& GetBpduData();
    u16 GetRxPortNo() const noexcept;

private:
    ByteStreamH _bpdu;
    u16 _rxPortNo; ///< Port number from which received BPDU
};

class ChangeLogMsgSeverityReq : public Command {
public:
    ChangeLogMsgSeverityReq(LoggingSystem::Logger::LogSeverity msgSeverity);
};

inline Command::Command(const RequestId reqId)
    : _reqId{ reqId } {
    // Nothing more to do
}

inline RequestId Command::Id() const noexcept {
    return _reqId;
}

inline AddPortReq::AddPortReq(const u16 portNo, const u32 speed, const bool enabled)
    : Command{ RequestId::AddPort }, _speed{ speed }, _portNo{ portNo }, _enabled{ enabled } {
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

inline RemovePortReq::RemovePortReq(const u16 portNo)
    : Command{ RequestId::RemovePort }, _portNo{ portNo } {
}

inline u16 RemovePortReq::GetPortNo() const noexcept {
    return _portNo;
}

inline ProcessBpduReq::ProcessBpduReq(ByteStreamH bpdu)
    : Command{ RequestId::ProcessBpdu }, _bpdu{ bpdu } {
}

inline ByteStream& ProcessBpduReq::GetBpduData() {
    return *_bpdu;
}

inline u16 ProcessBpduReq::GetRxPortNo() const noexcept {
    return _rxPortNo;
}

} // namespace Stp
