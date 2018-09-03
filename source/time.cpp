/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// This project's headers
#include "stp/time.hpp"

namespace Stp {

SmTimers::SmTimers() noexcept
    : _edgeDelayWhile{ 0 }, _fdWhile{ 0 }, _helloWhen{ 0 }, _mdelayWhile{ 0 }, _rbWhile{ 0 },
      _rcvdInfoWhile{ 0 }, _rrWhile{ 0 }, _tcWhile{ 0 } {
    // Nothing more to do
}

SmTimers& SmTimers::operator--() noexcept {
    dec(_helloWhen);
    dec(_tcWhile);
    dec(_fdWhile);
    dec(_rcvdInfoWhile);
    dec(_rrWhile);
    dec(_rbWhile);
    dec(_mdelayWhile);
    dec(_edgeDelayWhile);

    return *this;
}

} // namespace Rstp
