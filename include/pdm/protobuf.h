#pragma once

#include "pdm.pb.h"

#if _WIN32
#define DllExport __declspec( dllexport )
#elif __APPLE__
#define DllExport __attribute__((visibility("default")))
#endif

namespace pdm_proto
{
    DllExport platform::Information GetData();
}
