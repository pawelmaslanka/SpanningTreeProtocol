#pragma once

// This project's headers
#include "lib.hpp"
#include "mac.hpp"

namespace Stp {

enum class RequestId : u8 {
    AddPort,
    RemovePort,
    ProcessBpdu
};

class OutInterface {
public:
    virtual Result FlushFdb(const u16 portNo) = 0;
    virtual Result SetForwarding(const u16 portNo, const bool enable) = 0;
    virtual Result SetLearning(const u16 portNo, const bool enable) = 0;
    virtual Result SendOutBpdu(const u16 portNo, ByteStreamH data) = 0;

protected:
    virtual ~OutInterface() = default;
};

using OutInterfaceH = Sptr<OutInterface>;
using LoggerH = Sptr<std::ostream>;

class System {
public:
    System(Mac bridgeAddr, LoggerH logger);
    Mac& GetBridgeAddr() noexcept;
    LoggerH& GetLog() noexcept;
private:
    Mac _bridgeAddr;
    LoggerH _logger;
};

using SystemH = Sptr<System>;

inline System::System(Mac bridgeAddr, LoggerH logger)
    : _bridgeAddr{ bridgeAddr }, _logger{ logger } {
}

inline Mac& System::GetBridgeAddr() noexcept {
    return _bridgeAddr;
}

inline LoggerH& System::GetLog() noexcept {
    return _logger;
}

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

inline Command::Command(const RequestId reqId)
    : _reqId{ reqId } {

}

inline RequestId Command::Id() const noexcept {
    return _reqId;
}

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

inline u16 RemovePortReq::GetPortNo() const noexcept {
    return _portNo;
}

class ProcessBpduReq : public Command {
public:
    ProcessBpduReq(ByteStreamH bpdu);
//    __virtual Result Execute() override;
};

class RunStp : public Command {
public:
    RunStp(SystemH system, OutInterfaceH outInterface);
//    __virtual Result Execute() override;
private:
    SystemH _system;
    OutInterfaceH _outInterface;
};

inline u16 AddPortReq::GetPortNo() const noexcept {
    return _portNo;
}

inline u32 AddPortReq::GetPortSpeed() const noexcept {
    return _speed;
}

inline bool AddPortReq::GetPortEnabled() const noexcept {
    return _enabled;
}

} // namespace Stp
