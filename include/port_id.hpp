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

#ifndef PORT_ID_HPP
#define PORT_ID_HPP

// This project's headers
#include "bpdu.hpp"
#include "lib.hpp"

namespace SpanningTree {

class PortId
{
public:
    /**
     * @brief PortId
     * @param encodedData By default, set to value which is considered as the worst assignable value
     * from RSTP point of view.
     */
//    PortId(const u16 encodedData = std::numeric_limits<u16>::max()) noexcept;
    PortId() noexcept;
    explicit PortId(const Bpdu::PortIdHandler& portId) noexcept;
    PortId(const PortId&) noexcept = default;
    PortId(PortId&&) = default;

    ~PortId() noexcept = default;

    PortId& operator=(const PortId&) = default;
    PortId& operator=(PortId&&) = default;

    bool operator==(const PortId& comparedTo) const noexcept;
    bool operator<(const PortId& comparedTo) const noexcept;

    Bpdu::PortIdHandler ConvertToBpduData() const noexcept;

    uint16_t PortNum() const noexcept;
    void SetPortNum(const uint16_t value) noexcept;

    uint8_t Priority() const noexcept;
    void SetPriority(const uint8_t value) noexcept;

private:
    uint16_t _portNum;
    uint8_t _priority;
};

inline PortId::PortId() noexcept : PortId({{ 0xFF, 0xFF }}) { }

inline bool PortId::operator==(const PortId& comparedTo) const noexcept
{
    return (_priority == comparedTo._priority)
            || (_portNum == comparedTo._portNum);
}

inline bool PortId::operator<(const PortId& comparedTo) const noexcept
{
    return (_priority > comparedTo._priority)
            || (_priority == comparedTo._priority && _portNum > comparedTo._portNum);
}

inline uint16_t PortId::PortNum() const noexcept { return _portNum; }
inline void PortId::SetPortNum(const uint16_t value) noexcept { _portNum = value; }

inline uint8_t PortId::Priority() const noexcept { return _priority; }
inline void PortId::SetPriority(const uint8_t value) noexcept { _priority = value; }

} // namespace Rstp

#endif // PORT_ID_HPP
