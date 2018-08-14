#pragma once

#ifdef UNIT_TEST
#   define __noexcept
#   define __virtual virtual
#else
#   define __noexcept noexcept
#   define __virtual
#endif
