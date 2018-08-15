/***************************************************************************************************
Copyright (c) 2018, Pawel Maslanka <pawmas@hotmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
***************************************************************************************************/

#ifndef STATES_HPP
#define STATES_HPP

// This project's headers
#include "lib.hpp"

// C++ Standard Library
#include <memory>
#include <vector>

namespace Stp {

/// @brief 17.7
enum class PortRole : u8 {
    Unknown,
    Root,
    Designated,
    Alternate,
    Backup,
    Disabled
};



enum class PtiState : u8 {
    OneSecond,
    Tick,
    Begin
};

enum class PrxState : u8 {
    Discard,
    Receive,
    Begin
};

enum class PpmState : u8 {
    CheckingRstp,
    SelectingStp,
    Sensing,
    Begin
};

enum class BdmState : u8 {
    Edge,
    NotEdge,
    Begin
};

enum class PtxState : u8 {
    TransmitInit,
    Idle,
    TransmitPeriodic,
    TransmitConfig,
    TransmitTcn,
    TransmitRstp,
    Begin
};

enum class PimState : u8 {
    Disabled,
    Aged,
    Update,
    Current,
    Receive,
    SuperiorDesignated,
    RepeatedDesignated,
    InferiorDesignated,
    NotDesignated,
    Other,
    Begin
};

enum class PrsState : u8 {
    InitBridge,
    RoleSelection,
    Begin
};

enum class PrtState : u8 {
    InitPort,
    DisablePort,
    DisabledPort,

    RootPort,
    RootProposed,
    RootAgreed,
    Reroot,
    RootForward,
    RootLearn,
    Rerooted,

    DesignatedPort,
    DesignatedPropose,
    DesignatedSynced,
    DesignatedRetired,
    DesignatedForward,
    DesignatedLearn,
    DesignatedDiscard,

    AlternatePort,
    AlternateProposed,
    AlternateAgreed,
    BlockPort,
    BackupPort,
    Begin
};

enum class PstState : u8 {
    Discarding,
    Learning,
    Forwarding,
    Begin
};

enum class TcmState : u8 {
    Inactive,
    Learning,
    Detected,
    Active,
    NotifiedTcn,
    NotifiedTc,
    Propagating,
    Acknowledged,
    Begin
};

} // namespace Rstp

#endif // STATES_HPP
