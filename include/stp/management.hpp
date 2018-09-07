/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "lib.hpp"
#include "logger.hpp"
#include "mac.hpp"
#include "system.hpp"

namespace Stp {

/**
 * @brief The Management class represents set of user's interface to manage STP
 */
class Management {
public:
    /**
     * @brief AddPort adds port to the RSTP
     * @param portNo port number to add to the RSTP
     * @param speed of port in Megabits [Mb]
     * @param enabled indicates if port is enabled (true) or disabled (false)
     * @return Result::Success if operation completed with success, otherwise Result::Fail
     */
    static Result AddPort(const u16 portNo, const u32 speed, const bool enabled);
    /**
     * @brief RemovePort removes port from the RSTP
     * @param portNo port number to add to the RSTP
     * @return Result::Success if operation completed with success, otherwise Result::Fail
     */
    static Result RemovePort(const u16 portNo);
    /**
     * @brief ProcessBpdu passes the BPDU data to process by the RSTP
     * @param portNo port number from which received BPDU
     * @param bpdu data unit
     * @return Result::Success if operation completed with success, otherwise Result::Fail
     */
    static Result ProcessBpdu(const u16 rxPortNo, ByteStreamH bpdu);
    /**
     * @brief SetLogSeverity sets which messages from RSTP should be logged
     * @param logSeverity represents ID of logged message from RSTP
     * @return Result::Success if operation completed with success, otherwise Result::Fail
     */
    static Result SetLogSeverity(const LoggingSystem::Logger::LogSeverity logSeverity);
    /**
     * @brief RunStp starts the RSTP
     * @param bridgeAddr MAC address of bridge on which run STP
     * @param system includes functionality used by STP to cooperate with bridge's system/OS
     * @return Result::Success if operation completed with success, otherwise Result::Fail
     */
    static Result RunStp(Mac bridgeAddr, SystemH system);
};

/**
 * @brief The RequestId enum represents unique ID of management commands
 */
enum class RequestId : u8 {
    AddPort,
    RemovePort,
    ProcessBpdu,
    SetLogSeverity
};

/**
 * @brief The Command class abstract for classes represent user's management commands
 */
class Command {
public:
    virtual ~Command() = default;
    RequestId Id() const noexcept;

protected:
    Command(const RequestId reqId);

private:
    RequestId _reqId;
};

/**
 * @brief The AddPortReq class represents user's request for add port to the RSTP
 */
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

/**
 * @brief The RemovePortReq class represents user's request for remove port from the RSTP
 */
class RemovePortReq : public Command {
public:
    RemovePortReq(const u16 portNo);
    u16 GetPortNo() const noexcept;

private:
    u16 _portNo;
};

/**
 * @brief The ProcessBpduReq class represents user's request for process BPDU data by the RSTP
 */
class ProcessBpduReq : public Command {
public:
    ProcessBpduReq(const u16 rxPortNo, ByteStreamH bpdu);
    ByteStream& GetBpduData();
    u16 GetRxPortNo() const noexcept;

private:
    u16 _rxPortNo; ///< Port number from which received BPDU
    ByteStreamH _bpdu; ///< Received BPDU data
};

/**
 * @brief The SetLogSeverityReq class represents user's request for set logged particular
 *        messages from the RSTP
 */
class SetLogSeverityReq : public Command {
public:
    SetLogSeverityReq(LoggingSystem::Logger::LogSeverity logSeverity);
    LoggingSystem::Logger::LogSeverity GetLogSeverity() const noexcept;

private:
    LoggingSystem::Logger::LogSeverity _logSeverity;
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

inline ProcessBpduReq::ProcessBpduReq(const u16 rxPortNo, ByteStreamH bpdu)
    : Command{ RequestId::ProcessBpdu }, _rxPortNo{ rxPortNo }, _bpdu{ bpdu } {
}

inline ByteStream& ProcessBpduReq::GetBpduData() {
    return *_bpdu;
}

inline u16 ProcessBpduReq::GetRxPortNo() const noexcept {
    return _rxPortNo;
}

inline SetLogSeverityReq::SetLogSeverityReq(LoggingSystem::Logger::LogSeverity logSeverity)
    : Command{ RequestId::SetLogSeverity }, _logSeverity{ logSeverity } {
}

inline LoggingSystem::Logger::LogSeverity SetLogSeverityReq::GetLogSeverity() const noexcept {
    return _logSeverity;
}

} // namespace Stp
