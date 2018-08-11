#pragma once

#ifdef UNIT_TEST
//#   define __override
#   define __virtual virtual
#else
//#   define __override override
#   define __virtual
#endif
